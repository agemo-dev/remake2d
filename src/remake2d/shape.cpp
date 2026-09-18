#include <remake2d/shape.hpp>
#include <remake2d/utility.hpp>

#include <SDL2/SDL.h>

namespace rmk {

Geometry::Geometry(const Vec2d& center, const Dim2d& size)
	: m_center(center), m_size(size) {
		m_size.w = m_size.w == 0 ? 1 : m_size.w;
		m_size.h = m_size.h == 0 ? 1 : m_size.h;
	}

void Geometry::fill(const Fillable& main) const noexcept {
    if (!is_fill_dirty) return;

    if (&main != static_cast<const Fillable*>(this)) {
        main.is_fill_dirty = false;
        main.filled        = true;
        _color(main.color());
    }

    VertexBatch batch;
    batch.texture = nullptr;

    auto raw = _verticesImpl();
    batch.vertices.reserve(raw.size());
    for (auto v : raw) {
        v.color = color();
        batch.vertices.push_back(v);
    }

    main._fill_cache_.push_back(batch);
    is_fill_dirty   = false;
    filled          = true;

}

void Geometry::draw(const Drawable& main) const noexcept {
    if (!is_draw_dirty) return;

    if (&main != static_cast<const Drawable*>(this)) {
        main.is_draw_dirty = false;
        main.drawn        = true;
        _color(main.color());
    }

    main._draw_cache_.push_back(DrawPack{ color(), _contourImpl() });
    is_draw_dirty    = false;
    drawn            = true;
}

template<> Circle Geometry::as(void) const noexcept {
    return Circle(m_center, m_size.w);
}

template<> Square Geometry::as(void) const noexcept {
    return Square(m_center, m_size.w);
}

Point::Point(const Vec2d& pos) : Shape<1>(pos, 1) {}

void Point::transform(const Vec2d& delta, f32 angle, const Fact2d& scaling) noexcept {
    m_center = { m_center.x + delta.x, m_center.y + delta.y };
    m_points[0] = m_center;
    m_is_changed = true;
    (void) angle; (void) scaling;
}

Circle::Circle(const Vec2d& center, f32 diameter) : Shape<36>(center, diameter) {}

void Circle::transform(const Vec2d& delta, f32 angle, const Fact2d& scaling) noexcept {

    f32 cosA = std::cos(angle);
    f32 sinA = std::sin(angle);

    for(auto& p : m_points) {
        // translation
        f32 x = p.x - m_center.x;
        f32 y = p.y - m_center.y;
        // scaling
        x *= scaling.x;
        y *= scaling.x;
        // rotation
        Vec2d rotation = { x * cosA - y * sinA, x * sinA + y * cosA };
        //translation
        p = { m_center.x + rotation.x + delta.x, m_center.y + rotation.y + delta.y };
    }
    m_center = { m_center.x + delta.x, m_center.y + delta.y };
    m_is_changed = true;
	_triangulate();
}


Triangle::Triangle(const Vec2d& center, const Dim2d& size) : Shape<3>(center, size) {
    rotate(- pi / 2);
}

Rectangle::Rectangle(const Vec2d& center, const Dim2d& size) : Shape<4>(center, size) {
    _build();
}

void Rectangle::_build(void) noexcept {
    Dim2d delta = { m_size.w / 2.0f, m_size.h / 2.0f };
    m_points[0] = {m_center.x - delta.w, m_center.y - delta.h};
    m_points[1] = {m_center.x + delta.w, m_center.y - delta.h};
    m_points[2] = {m_center.x + delta.w, m_center.y + delta.h};
    m_points[3] = {m_center.x - delta.w, m_center.y + delta.h};
    m_points[4] = m_points[0];

    m_is_changed = true;
    _triangulate();
}


Square::Square(const Vec2d& center, f32 size) : Rectangle(center, size) {}


void Square::transform(const Vec2d& delta, f32 angle, const Fact2d& scaling) noexcept {

    f32 cosA = std::cos(angle);
    f32 sinA = std::sin(angle);

    for(auto& p : m_points) {
        // translation
        f32 x = p.x - m_center.x;
        f32 y = p.y - m_center.y;
        // scaling
        x *= scaling.x;
        y *= scaling.x;
        // rotation
        Vec2d rotation = { x * cosA - y * sinA, x * sinA + y * cosA };
        //translation
        p = { m_center.x + rotation.x + delta.x, m_center.y + rotation.y + delta.y };
    }
    m_center = { m_center.x + delta.x, m_center.y + delta.y };
    m_is_changed = true;
	_triangulate();
}

} // namespace rmk