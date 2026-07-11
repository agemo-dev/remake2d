#include <remake2d/camera.hpp>
#include <remake2d/shape.hpp>
#include <remake2d/physic.hpp>
#include <remake2d/utility.hpp>

#include <utility>

namespace rmk {

Camera::Camera(void) {
    onMove.bind([this](void) { return _hasMove(); });
    onMove.joinPriority([this](void) { _offset();  });
    onMove.joinPriority([this](void) { _replace(); });
}

Camera::Camera(const Vec2d& center, const Dim2d& size, const Dim2d& limit)  
 :  m_center(center), m_size(size), m_limit(limit), m_ghost(center), m_lasted_point(center) {
    onMove.bind([this](void) { return _hasMove(); });
    onMove.joinPriority([this](void) { _offset();  });
    onMove.joinPriority([this](void) { _replace(); });
}

void Camera::move(const Vec2d& center) noexcept {
    m_center = center;
}

void Camera::limit(const Dim2d& limit) noexcept {
    m_limit = limit;
}

void Camera::resize(const Dim2d& size) noexcept {
    m_size = size;
}

void Camera::smoothing(f32 s) noexcept {
    m_smoothing = s < 0.0f ? 0.0f : s > 1.0f ? 1.0f : s;
}

f32 Camera::smoothing(void) const noexcept {
    return m_smoothing;
}

void Camera::zoom(f32 z) noexcept {
    m_zoom = z < 0.1f ? 0.1f : z;
}

f32 Camera::zoom(void) const noexcept {
    return m_zoom;
}

Dim2d Camera::size(void) const noexcept {
    return m_size;
}

Vec2d Camera::center(void) const noexcept {
    return m_center;
}

Dim2d Camera::limit(void) const noexcept {
    return m_limit;
}

Vec2d Camera::offset(void) const noexcept {
    return m_offset;
}

Vec2d Camera::followedPoint(void) const noexcept {
    return m_followed_point ? *m_followed_point : nil;
}

void Camera::follow(Geometry& shape) noexcept {
    m_followed_point = &shape.m_center;
    m_ghost 		 = shape.m_center;
    m_lasted_point   = shape.m_center;
}

void Camera::follow(PhysicBody& body) noexcept {
    m_followed_point = &body.m_shape_cache.center;
    m_ghost          = body.m_shape_cache.center;
    m_lasted_point   = body.m_shape_cache.center;
}

void Camera::follow(Vec2d& point) noexcept {
    m_followed_point = &point;
    m_ghost          = point;
    m_lasted_point   = point;
}

bool Camera::_hasMove(void) const noexcept {
    if(!m_followed_point) return false;
    Vec2d followed = *m_followed_point;
    Vec2d lasted   = m_lasted_point;
	
    return followed.x != lasted.x || followed.y != lasted.y;
}

void Camera::_offset(void) noexcept {
    if(!m_followed_point) return;
    const Vec2d followed = *m_followed_point;

    if (m_smoothing > 0.0f) {
        Vec2d ghost = m_ghost;
        f32   t     = m_smoothing * delta.tick();
        ghost.x     = ghost.x + (followed.x - ghost.x) * t;
        ghost.y     = ghost.y + (followed.y - ghost.y) * t;
        m_ghost = ghost;
    } else {
        m_ghost = followed;
    }

    m_ghost.x = std::clamp(m_ghost.x, 0.0f, (f32)m_limit.w - m_size.w);
    m_ghost.y = std::clamp(m_ghost.y, 0.0f, (f32)m_limit.h - m_size.h);

    Vec2d& last = m_lasted_point;
    m_offset = {m_ghost.x - last.x, m_ghost.y - last.y};
}

void Camera::_replace(void) noexcept {
    if(m_followed_point) m_lasted_point = *m_followed_point;
}

}//namespace rmk