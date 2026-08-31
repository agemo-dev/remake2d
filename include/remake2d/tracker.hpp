#ifndef REMAKE2D_TRACKER_
#define REMAKE2D_TRACKER_

#include <remake2d/error.hpp>
#include <remake2d/concept.hpp>

#include <memory>
#include <compare>
#include <concepts>

namespace rmk {

template<IsTrackable T> struct Slot {
public:
    const T* ptr{nullptr};

    static std::shared_ptr<Slot<T>> make(void) noexcept;
	bool operator==(const Slot<T>&)      const noexcept;
	bool operator!=(const Slot<T>&)      const noexcept;
};


class TrackerBase {
rmk_heritableBaseClass(TrackerBase);
};

template<IsTrackable T> class Tracker : private TrackerBase {
private:
    std::weak_ptr<Slot<T>> m_tracked;

public:
    using Balise = std::shared_ptr<Slot<T>>;

public:
    Tracker(void)                       = default;
    Tracker(Tracker&&)                  = default;
    Tracker(const Tracker&)             = default;
    Tracker& operator=(Tracker&&)       = default;
    Tracker& operator=(const Tracker&)  = default;

public:
    Tracker(const Balise&);

public:
	T&       operator*(void);
	T*       operator->(void);
	const T& operator*(void)  const;
	const T* operator->(void) const;
	explicit operator bool() const;
	bool     operator==(const Tracker<T>&) const noexcept;
	bool     operator!=(const Tracker<T>&) const noexcept;

public:
    void track(const Balise&) noexcept;

    template<typename U = T> requires IsRelatedTo<U, T>
    U* locate(void) noexcept;

    template<typename U = T> requires IsRelatedTo<U, T>
    const U* locate(void) const noexcept;
};

class TrackableBase {
rmk_heritableBaseClass(TrackableBase);
};

template<typename Derived> class Trackable : private TrackableBase {
protected:
    mutable Tracker<Derived>::Balise m_slot;

public:
    Trackable(void)                        = default;
    Trackable(const Trackable&)            = default;
    Trackable& operator=(const Trackable&) = default;

public:
    Trackable(Trackable&&);
    Trackable& operator=(Trackable&&);

public:
    void relocate(void)                  noexcept;
    Tracker<Derived> tracker(void)       noexcept;
    Tracker<Derived> tracker(void) const noexcept;

public:
    virtual ~Trackable(void);
};

} // namespace rmk

#include<remake2d/template/tracker.tpp>

#endif