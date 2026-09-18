#ifndef REMAKE2D_TRACKER_
#define REMAKE2D_TRACKER_

#include <remake2d/error.hpp>
#include <remake2d/concept.hpp>
#include <remake2d/private/struct.hpp>

#include <memory>
#include <compare>
#include <concepts>

namespace rmk {

template<typename T> struct Slot {
public:
    const T* ptr{nullptr};

    static std::shared_ptr<Slot<T>> make(void) noexcept;
	bool operator==(const Slot<T>&)      const noexcept;
	bool operator!=(const Slot<T>&)      const noexcept;
};

template<typename T> using Balise = std::shared_ptr<Slot<T>>;

class TrackerBaseID { rmk_heritableBaseClass(TrackerBaseID); };

template<typename T> class UnsafeTracker : public TrackerBaseID {
private:
    std::weak_ptr<Slot<T>> m_tracked;

public:
    UnsafeTracker(void)                             = default;
    UnsafeTracker(UnsafeTracker&&)                  = default;
    UnsafeTracker(const UnsafeTracker&)             = default;
    UnsafeTracker& operator=(UnsafeTracker&&)       = default;
    UnsafeTracker& operator=(const UnsafeTracker&)  = default;

private:
    UnsafeTracker(const Balise<T>&);

public:
	T&       operator*(void);
	T*       operator->(void);
	const T& operator*(void)  const;
	const T* operator->(void) const;
	explicit operator bool()  const;
	bool     operator==(const UnsafeTracker<T>&) const noexcept;
	bool     operator!=(const UnsafeTracker<T>&) const noexcept;

public:
    template<typename U = T> requires IsRelatedTo<U, T>
    U* locate(void) noexcept;

    template<typename U = T> requires IsRelatedTo<U, T>
    const U* locate(void) const noexcept;

private:
    template<typename Derived> friend class Trackable;
};

template<IsTrackable SafeType> using Tracker = UnsafeTracker<SafeType>;

class TrackableBaseID { rmk_heritableBaseClass(TrackableBaseID); };

template<typename Derived> class Trackable : public TrackableBaseID {

protected:
    mutable Balise<Derived> m_slot;

public:
    Trackable(void)                        = default;
    Trackable(const Trackable&)            = default;
    Trackable& operator=(const Trackable&) = default;

public:
    Trackable(Trackable&&);
    Trackable& operator=(Trackable&&);

public:
    void relocate(void)                         noexcept;
    UnsafeTracker<Derived> tracker(void)        noexcept;
    UnsafeTracker<Derived> tracker(void)  const noexcept;

public:
    virtual ~Trackable(void);
};

} // namespace rmk

#include<remake2d/template/tracker.tpp>

#endif