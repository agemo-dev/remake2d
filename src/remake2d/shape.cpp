#include <remake2d/shape.hpp>
#include <remake2d/utility.hpp>

#include <SDL2/SDL.h>

namespace rmk {

Geometry::Geometry(const Vec2d& center, const Dim2d& size)
	: m_center(center), m_size(size) {
		m_size.w = m_size.w == 0 ? 1 : m_size.w;
		m_size.h = m_size.h == 0 ? 1 : m_size.h;
	}

void Geometry::fill(const Fillable& main) noexcept {
    if (!is_fill_dirty) return;

    if (&main != this) {
        main.is_fill_dirty = false;
        main.filled        = true;
        color(main.color());
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

void Geometry::draw(const Drawable& main) noexcept {
    if (!is_draw_dirty) return;

    if (&main != this) {
        main.is_draw_dirty = false;
        main.drawn        = true;
        color(main.color());
    }

    auto raw = _contourImpl();
    std::vector<SDL_FPoint> contour;
    contour.reserve(raw.size());
    for (const auto& p : raw) contour.push_back((SDL_FPoint)p);
    main._draw_cache_.push_back({ color(), std::move(contour) });
    is_draw_dirty    = false;
    drawn    = true;
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