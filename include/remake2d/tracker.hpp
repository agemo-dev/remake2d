#ifndef REMAKE2D_TRACKER_
#define REMAKE2D_TRACKER_

#include <remake2d/error.hpp>

#include <memory>
#include <compare>

namespace rmk {

template<typename T> class Slot {
public:
    const T* ptr{nullptr};

    static std::shared_ptr<Slot<T>> make(void) noexcept;
	bool operator==(const Slot<T>&) const noexcept;
	bool operator!=(const Slot<T>&) const noexcept;
};


class TrackerBase {
public:
    TrackerBase(void)                          = default;
    TrackerBase(TrackerBase&&)                 = default;
    TrackerBase(const TrackerBase&)            = default;
    TrackerBase& operator=(TrackerBase&&)      = default;
    TrackerBase& operator=(const TrackerBase&) = default;

public:
	virtual void locate(void) noexcept = 0
};

template<typename T> class Tracker : private TrackerBase {
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
	bool     operator==(const Tracker<T>&) const noexcept;
	bool     operator!=(const Tracker<T>&) const noexcept;

public:
    T* locate(void)             noexcept;
    void track(const Balise&)   noexcept;
    const T* locate(void) const noexcept override;
};

class TrackableBase {
public:
    TrackableBase(void)                            = default;
    TrackableBase(TrackableBase&&)                 = default;
    TrackableBase(const TrackableBase&)            = default;
    TrackableBase& operator=(TrackableBase&&)      = default;
    TrackableBase& operator=(const TrackableBase&) = default;

public:
	virtual void relocate(void) noexcept = 0;
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
    void relocate(void) noexcept override;
    Tracker<Derived> tracker(void) noexcept;
    Tracker<Derived> tracker(void) const noexcept;

public:
    virtual ~Trackable(void);
};

} // namespace rmk

#include<remake2d/template/tracker.tpp>

#endif