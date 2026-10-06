/*************************************************************************
SKIP TRACER: a small, header-only C++20 library that hands out safe,
type-aware handles (Tracker) to objects that may move or be destroyed
at any time.

Design summary
---------------
- Trackable is polymorphic and non-template.
- relocate() runs automatically from Trackable's own move constructor and
  move assignment - a derived class never has to call it itself.
- Tracker<T>::locate<U>() uses static_cast when possible (compile-time,
  free), falling back to dynamic_cast only when static_cast can't resolve
  the relation (virtual inheritance, or a crosscast between branches).
- Slots live in a stable pool (SlotPool) and are recycled through an
  intrusive free-list. Validity is tracked with a generation counter
  instead of a reference count: locate() costs a single atomic load, no
  read-modify-write, no allocation, no lock on the hot path.

Thread-safety
--------------
- Slot::ptr is a std::atomic<Trackable*>; its visibility across threads is
  established by Slot::generation (release on write, acquire on read).
- SlotPool::acquire()/release() are lock-free operations using a lock-free
  Treiber stack with ABA tag protection.
- Moving/destroying the same Trackable instance from two threads at once
  still needs external synchronization, like any ordinary C++ object.

Repository: https://github.com/agemo-dev/skiptracer
Version: 1.1.0
License: MIT (see end of file).
*************************************************************************/

#ifndef SKIP_TRACER_HPP_
#define SKIP_TRACER_HPP_

#include <new>
#include <array>
#include <mutex>
#include <memory>
#include <atomic>
#include <cstdint>
#include <cassert>
#include <concepts>
#include <type_traits>

namespace skip {

class Trackable;

constexpr const char *ERROR_NULL_PTR_DEREF  = "skiptracer: dereferencing an expired Tracker";
constexpr const char *ERROR_BUFFER_OVERFLOW = "skiptracer: SlotPool max capacity reached";

// ============================================================================
// Concepts
// ============================================================================

template<typename T>
concept IsTracker = std::is_base_of_v<class TrackerBase, T>;

template<typename T>
concept IsTrackable = std::derived_from<T, Trackable>;

template<typename U, typename T>
concept IsRelatedTo =  std::same_as<U, T>        ||
                       std::derived_from<T, U>   ||
                       std::derived_from<U, T>   ||
                       std::same_as<U, void>;

// ============================================================================
// Slot / SlotPool
// ============================================================================

// Stays at a stable address for as long as any Tracker<T> may reference it.
struct Slot {

public:
    std::atomic<Trackable*>    ptr{nullptr};
    std::atomic<std::uint32_t> index{0};
    std::atomic<std::uint32_t> next_free{0};
    std::atomic<std::uint32_t> generation{0};

public:
    Slot(void) = default;

public:
    Slot(Slot&& other) noexcept
        : ptr(other.ptr.load(std::memory_order_relaxed)),
          index(other.index.load(std::memory_order_relaxed)),
          next_free(other.next_free.load(std::memory_order_relaxed)),
          generation(other.generation.load(std::memory_order_relaxed))
    {
        other.ptr.store(nullptr, std::memory_order_relaxed);
    }

public:
    // Slots are never copied between each other: two objects must never share a Slot.
    Slot(const Slot&) : ptr(nullptr), index(0), next_free(0), generation(0) {}
    Slot& operator=(const Slot&) { return *this; }

public:
    ~Slot(void) = default;
};

// Fixed-block pool: allocates Slots in blocks of BLOCK_SIZE, recycles freed
// slots through an intrusive free-list, never moves a Slot once constructed.
class SlotPool {

private:
    static constexpr std::uint32_t k_none_32 = 0xFFFFFFFF;
    static constexpr std::size_t BLOCK_SIZE  = 4096;
    static constexpr std::size_t MAX_BLOCK   = 1024;

private:
    std::array<std::atomic<Slot*>, MAX_BLOCK> m_blocks{};
    std::atomic<std::size_t>                  m_block_count{0};

private:
    std::atomic<std::uint64_t> m_free_head{(static_cast<std::uint64_t>(0) << 32) | k_none_32};
    std::mutex                 m_grow_mutex;

public:
    SlotPool(void) = default;

public:
    static SlotPool& getInstance(void) noexcept {
        alignas(SlotPool) static std::byte buffer[sizeof(SlotPool)];
        static SlotPool *pool = ::new (static_cast<void*>(buffer)) SlotPool();
        return *pool;
    }

public:
    // Lock-Free Acquire
    Slot* acquire(void) noexcept {
        std::uint64_t current_head = m_free_head.load(std::memory_order_acquire);
        while (true) {
            std::uint32_t idx = static_cast<std::uint32_t>(current_head & 0xFFFFFFFF);
            std::uint32_t tag = static_cast<std::uint32_t>(current_head >> 32);

            if (idx == k_none_32) {
                grow();
                current_head = m_free_head.load(std::memory_order_acquire);
                continue;
            }

            Slot* slot = getSlot(idx);
            std::uint32_t next_idx = slot->next_free.load(std::memory_order_relaxed);
            std::uint64_t new_head = (static_cast<std::uint64_t>(tag + 1) << 32) | next_idx;

            if (m_free_head.compare_exchange_weak(current_head, new_head,
                                                  std::memory_order_acq_rel,
                                                  std::memory_order_acquire)) {
                slot->ptr.store(nullptr, std::memory_order_relaxed);
                slot->generation.fetch_add(1, std::memory_order_relaxed); // impair -> pair
                return slot;
            }
        }
    }

    void release(Slot* slot) noexcept {
        slot->ptr.store(nullptr, std::memory_order_release);
        slot->generation.fetch_add(1, std::memory_order_release); // pair -> impair

        std::uint32_t idx = slot->index.load(std::memory_order_relaxed);
        std::uint64_t current_head = m_free_head.load(std::memory_order_relaxed);

        while (true) {
            std::uint32_t head_idx = static_cast<std::uint32_t>(current_head & 0xFFFFFFFF);
            std::uint32_t tag      = static_cast<std::uint32_t>(current_head >> 32);

            slot->next_free.store(head_idx, std::memory_order_relaxed);
            std::uint64_t new_head = (static_cast<std::uint64_t>(tag + 1) << 32) | idx;

            if (m_free_head.compare_exchange_weak(current_head, new_head,
                                                  std::memory_order_release,
                                                  std::memory_order_relaxed)) {
                break;
            }
        }
    }

private:
    Slot* getSlot(std::size_t idx) noexcept {
        Slot* block = m_blocks[idx / BLOCK_SIZE].load(std::memory_order_acquire);
        return &block[idx % BLOCK_SIZE];
    }

    void grow(void) {
        std::lock_guard<std::mutex> lock(m_grow_mutex);
        std::uint64_t current_head = m_free_head.load(std::memory_order_relaxed);
        if (static_cast<std::uint32_t>(current_head & 0xFFFFFFFF) != k_none_32) {
            return;
        }

        std::size_t current_count = m_block_count.load(std::memory_order_relaxed);
        if (current_count >= MAX_BLOCK) {
            assert(false && ERROR_BUFFER_OVERFLOW);
            return;
        }

        std::size_t base = current_count * BLOCK_SIZE;
        Slot* raw_block = new Slot[BLOCK_SIZE];

        for (std::size_t i = 0; i < BLOCK_SIZE; ++i) {
            std::size_t idx = base + i;
            std::uint32_t next = (i + 1 == BLOCK_SIZE) ? k_none_32 : static_cast<std::uint32_t>(idx + 1);

            raw_block[i].index.store(static_cast<std::uint32_t>(idx), std::memory_order_relaxed);
            raw_block[i].generation.store(1, std::memory_order_relaxed); // free = impair
            raw_block[i].next_free.store(next, std::memory_order_relaxed);
        }

        m_blocks[current_count].store(raw_block, std::memory_order_release);
        m_block_count.store(current_count + 1, std::memory_order_release);

        std::uint32_t tag      = static_cast<std::uint32_t>(current_head >> 32);
        std::uint64_t new_head = (static_cast<std::uint64_t>(tag + 1) << 32) | static_cast<std::uint32_t>(base);

        m_free_head.store(new_head, std::memory_order_release);
    }

public:
    ~SlotPool(void) {
        std::size_t count = m_block_count.load(std::memory_order_relaxed);
        for (std::size_t i = 0; i < count; ++i) {
            delete[] m_blocks[i].load(std::memory_order_relaxed);
        }
    }
};

// ============================================================================
// UnsafeTracker<T> / Tracker<T>
// ============================================================================

class TrackerBase {};

template<typename T> class UnsafeTracker : private TrackerBase {

private:
    Slot*         m_slot{nullptr};
    std::uint32_t m_generation{0};

public:
    UnsafeTracker(void)                              = default;
    UnsafeTracker(UnsafeTracker&&)                   = default;
    UnsafeTracker(const UnsafeTracker&)              = default;
    UnsafeTracker& operator=(UnsafeTracker&&)        = default;
    UnsafeTracker& operator=(const UnsafeTracker&)   = default;

public:
    UnsafeTracker(Slot* slot, std::uint32_t generation) noexcept
        : m_slot(slot), m_generation(generation) {}

    template<typename U> requires IsRelatedTo<U, T>
    UnsafeTracker(const UnsafeTracker<U>& other) noexcept
        : m_slot(other.m_slot), m_generation(other.m_generation) {}

public:
    T& operator*(void) {
        assert(locate<T>() && ERROR_NULL_PTR_DEREF);
        return *locate<T>();
    }

    const T& operator*(void) const {
        assert(locate<T>() && ERROR_NULL_PTR_DEREF);
        return *locate<T>();
    }

    T*       operator->(void)       { return locate<T>(); }
    const T* operator->(void) const { return locate<T>(); }

    explicit operator bool(void) const { return locate<T>() != nullptr; }

    bool operator==(const UnsafeTracker<T>& other) const noexcept {
        return m_slot == other.m_slot && m_generation == other.m_generation;
    }

    bool operator!=(const UnsafeTracker<T>& other) const noexcept {
        return !(*this == other);
    }

public:
    template<typename U = T> requires IsRelatedTo<U, T>
    U* locate(void) noexcept {
        if (!m_slot) return nullptr;

        Trackable*    base = m_slot->ptr.load(std::memory_order_acquire);
        std::uint32_t gen  = m_slot->generation.load(std::memory_order_acquire);

        if (!base || (gen & 1) != 0 || gen != m_generation) return nullptr;

        if constexpr (std::same_as<U, void>) {
            return static_cast<void*>(base);
        } else if constexpr (requires (Trackable* b) { static_cast<U*>(b); }) {
            return static_cast<U*>(base);
        } else {
            return dynamic_cast<U*>(base);
        }
    }

    template<typename U = T> requires IsRelatedTo<U, T>
    const U* locate(void) const noexcept {
        if (!m_slot) return nullptr;

        const Trackable* base = m_slot->ptr.load(std::memory_order_acquire);
        std::uint32_t    gen  = m_slot->generation.load(std::memory_order_acquire);

        if (!base || (gen & 1) != 0 || gen != m_generation) return nullptr;

        if constexpr (std::same_as<U, void>) {
            return static_cast<const void*>(base);
        } else if constexpr (requires (const Trackable* b) { static_cast<const U*>(b); }) {
            return static_cast<const U*>(base);
        } else {
            return dynamic_cast<const U*>(base);
        }
    }

public:
    template<typename> friend class UnsafeTracker;
};

// A safe Tracker for basic usage
template<IsTrackable SafeType> using Tracker = UnsafeTracker<SafeType>;

// ============================================================================
// Trackable
// ============================================================================

#define skip_FiveRule(CLASS)                     \
    CLASS(void)                       = default; \
    CLASS(CLASS&&)                    = default; \
    CLASS(const CLASS&)               = default; \
    CLASS& operator=(CLASS&&)         = default; \
    CLASS& operator=(const CLASS&)    = default

class Trackable {

private:
    mutable Slot* m_slot{nullptr};

public:
    Trackable(void) = default;

public:
    Trackable(const Trackable&) : m_slot(nullptr) {}
    Trackable& operator=(const Trackable&) { return *this; }

public:
    Trackable(Trackable&& other) noexcept : m_slot(other.m_slot) {
        other.m_slot = nullptr;
        relocate();
    }

    Trackable& operator=(Trackable&& other) noexcept {
        if (this == &other) return *this;
        releaseSlot();
        m_slot = other.m_slot;
        other.m_slot = nullptr;
        relocate();
        return *this;
    }

protected:
    void relocate(void) const noexcept {
        if (!m_slot) return;
        m_slot->ptr.store(const_cast<Trackable*>(this), std::memory_order_release);
        m_slot->generation.fetch_add(0, std::memory_order_release);
    }

public:
    Tracker<Trackable> tracker(void) const noexcept {
        if (!m_slot) {
            m_slot = SlotPool::getInstance().acquire();
            m_slot->ptr.store(const_cast<Trackable*>(this), std::memory_order_release);
        }
        return Tracker<Trackable>(m_slot, m_slot->generation.load(std::memory_order_relaxed));
    }

private:
    void releaseSlot(void) const noexcept {
        if (!m_slot) return;
        SlotPool::getInstance().release(m_slot);
        m_slot = nullptr;
    }

public:
    virtual ~Trackable(void) {
        releaseSlot();
    }
};

// ============================================================================
// Box
// ============================================================================

template<typename T> class Box : public Trackable {

public:
    T value;

public:
    skip_FiveRule(Box);

public:
    template<typename... Args> requires (sizeof...(Args) != 1 || (!std::same_as<std::decay_t<Args>, Box> && ...))
    Box(Args&&... args) : value(std::forward<Args>(args)...) {}

public:
    T&       operator*(void)       noexcept { return value; }
    const T& operator*(void) const noexcept { return value; }

public:
    T*       operator->(void)       noexcept { return &value; }
    const T* operator->(void) const noexcept { return &value; }

public:
    virtual ~Box(void) = default;
};

} // namespace skip

#endif // SKIP_TRACER_HPP_

/******************************************************************************
MIT License

Copyright (c) 2026 agemo-dev

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/