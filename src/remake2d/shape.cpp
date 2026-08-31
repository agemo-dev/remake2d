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
    if (!__is_fill_dirty__) return;
    VertexBatch batch;
    batch.texture = nullptr;

    auto raw = _verticesImpl();
    batch.vertices.reserve(raw.size());
    for (auto v : raw) {
        v.color = m_color;
        batch.vertices.push_back(v);
    }

    main.__fill_cache__ = { batch };
    __is_fill_dirty__ = false;
}

void Geometry::draw(const Drawable& main) const noexcept {
    if (!__is_dirty__) return;
    auto raw = _contourImpl();
    std::vector<SDL_FPoint> contour;
    contour.reserve(raw.size());
    for (const auto& p : raw) contour.push_back((SDL_FPoint)p);
    main.__draw_cache__ = { DrawPack{ m_color, std::move(contour) } };
    __is_dirty__ = false;
}

template<> Circle Geometry::as(void) const noexcept {
    return Circle(m_center, m_size.w);
}

template<> Square Geometry::as(void) const noexcept {
    return Square(m_center, m_size.w);
}

Point::Point(const Vec2d& pos) : Shape<1>(pos, 1) {}

void Point::scale(const Fact2d& fact) noexcept {
	return (void) fact;
}

void Point::resize(const Dim2d& size) noexcept {
    return (void) size;
}

Circle::Circle(const Vec2d& center, f32 diameter) : Shape<36>(center, diameter) {}

void Circle::scale(const Fact2d& fact) noexcept {
    Shape<36>::scale(fact.x);
}

void Circle::resize(const Dim2d& size) noexcept {
    Shape<36>::resize(size.w);
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

void Square::scale(const Fact2d& fact) noexcept {
    Shape<4>::scale(fact.x);
}

void Square::resize(const Dim2d& size) noexcept {
    Shape<4>::resize(size.w);
}

} // namespace rmk