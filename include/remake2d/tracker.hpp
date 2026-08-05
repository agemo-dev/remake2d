#ifndef REMAKE2D_TRACKER_
#define REMAKE2D_TRACKER_

#include <memory>

namespace rmk {

template<typename T> class Slot {
public:
    T* ptr{nullptr};

    static std::shared_ptr<Slot<T>> make(void) noexcept;
};

template<typename T> class Tracker {
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
	T* operator->(void);
	T* operator->(void) const;
    T* locate(void)     const noexcept;
    void track(const Balise&) noexcept;
};

class TrackerBase {
public:
    TrackerBase(void)                          = default;
    TrackerBase(TrackerBase&&)                 = default;
    TrackerBase(const TrackerBase&)            = default;
    TrackerBase& operator=(TrackerBase&&)      = default;
    TrackerBase& operator=(const TrackerBase&) = default;

public:
	virtual void relocate(void) noexcept = 0;
};

template<typename Derived> class Trackable : private TrackerBase {
protected:
    typename Tracker<Derived>::Balise m_slot;

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

public:
    virtual ~Trackable(void);
};

} // namespace rmk

#include<remake2d/template/tracker.tpp>

#endif