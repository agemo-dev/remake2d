#ifndef REMAKE2D_CAMERA_
#define REMAKE2D_CAMERA_

#include <remake2d/time.hpp>
#include <remake2d/vector.hpp>
#include <remake2d/signal.hpp>
#include <remake2d/tracker.hpp>
#include <remake2d/concept.hpp>
#include <remake2d/utility.hpp>
#include <remake2d/config/forward.hpp>

namespace rmk {

class Followable : public Trackable {

public:
    virtual Vec2d center(void) const noexcept = 0;

public:
    rmk_heritableBaseClass(Followable);
};

class Camera : public Trackable {
private:
    Tracker<Followable>        m_tracker;
    Vec2d                      m_center;
    Dim2d                      m_size;
    Dim2d                      m_limit;
    Vec2d                      m_offset;
    Vec2d                      m_ghost;
    Vec2d                      m_last_point;
    f32                        m_zoom{1.0f};
    f32                        m_smoothing{0.0f};

public:
    Signal<> onMove;

public:
    Camera(void);
    Camera(const Vec2d&, const Dim2d&, const Dim2d& = nil);

public:
    Camera(const Camera&)                 = default;
    Camera(Camera&&) noexcept             = default;
    Camera& operator=(const Camera&)      = default;
    Camera& operator=(Camera&&) noexcept  = default;

public:
    void  zoom(f32)            noexcept;
    void  move(const Vec2d&)   noexcept;
    void  limit(const Dim2d&)  noexcept;
    void  resize(const Dim2d&) noexcept;
    void  smoothing(f32)       noexcept;

public:
    f32  zoom(void)           const noexcept;
    Dim2d size(void)          const noexcept;
    Vec2d center(void)        const noexcept;
    Dim2d limit(void)         const noexcept;
    Vec2d offset(void)        const noexcept;
    Vec2d viewCenter(void)    const noexcept;
    Vec2d followedPoint(void) const noexcept;
    f32   smoothing(void)     const noexcept;

public:
    void unfollow(void)            noexcept;
    void follow(const Followable&) noexcept;

private:
    void _offset(void)        noexcept;
    void _replace(void)       noexcept;
    bool _hasMove(void) const noexcept;
};

}//namespace rmk

#endif