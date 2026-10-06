#include <remake2d/shape.hpp>
#include <remake2d/utility.hpp>

#include <SDL2/SDL.h>
#include <algorithm>

namespace rmk {

Geometry::Geometry(const Vec2d& center, const Dim2d& size)
	: m_center(center), m_size(size) {
		m_size.w = m_size.w == 0 ? 1 : m_size.w;
		m_size.h = m_size.h == 0 ? 1 : m_size.h;
	}

void Geometry::fill(const Printable& main) const noexcept {
    bool toRoot = (&main == static_cast<const Printable*>(this));

    if (!is_fill_dirty && !toRoot && _fill_id_ >= 0 && _main_fill_id_ == main._mainId()) {
        main._fill_deep_ += _last_vertex_count_;
        return;
    }
    if (!is_fill_dirty && toRoot) {
        main._fill_deep_ += _last_vertex_count_;
        return;
    }

    auto& cache     = _fill_cache_;
    auto& mainCache = main._fill_cache_;

    u32  deep        = main._fill_deep_;
    bool isFirstTime = _fill_id_ < 0 || _main_fill_id_ != main._mainId();

    if (!toRoot) {
        main.filled = true;
        _inheritData(main.color(), main.layer());

        if (_main_fill_id_ != main._mainId()) {
            _main_fill_id_      = main._mainId();
            _fill_id_           = -1;
            _last_vertex_count_ = 0;
        }

        if (!isFirstTime && deep < (u32)_fill_id_) {
            mainCache.erase(mainCache.begin() + deep, mainCache.begin() + _fill_id_);
        }
    }

    if (is_fill_dirty) {
        cache.clear();

        VertexBatch batch;
        batch.texture = nullptr;

        auto raw = _verticesImpl();
        batch.vertices.reserve(raw.size());
        for (auto v : raw) {
            v.color = color();
            batch.vertices.push_back(v);
        }

        cache.push_back(std::move(batch));
        is_fill_dirty = false;
    }

    _current_vertex_count_ = (u32)cache.size();

    if (!toRoot) {
        if (isFirstTime) {
            mainCache.insert(mainCache.begin() + deep, cache.begin(), cache.end());
        } else {
            u32 overlap = std::min(_last_vertex_count_, _current_vertex_count_);
            std::copy(
                cache.begin(), cache.begin() + overlap,
                mainCache.begin() + deep
            );

            if (_last_vertex_count_ < _current_vertex_count_) {
                mainCache.insert(
                    mainCache.begin() + deep + _last_vertex_count_,
                    cache.begin() + _last_vertex_count_, cache.end()
                );
            } else if (_last_vertex_count_ > _current_vertex_count_) {
                mainCache.erase(
                    mainCache.begin() + deep + _current_vertex_count_,
                    mainCache.begin() + deep + _last_vertex_count_
                );
            }
        }

        _fill_id_ = (i32)deep;
    }

    _last_vertex_count_ = _current_vertex_count_;
    main._fill_deep_    = (toRoot ? 0 : deep) + _current_vertex_count_;

    filled = true;
}

void Geometry::draw(const Printable& main) const noexcept {
    bool toRoot = (&main == static_cast<const Printable*>(this));

    if (!is_draw_dirty && !toRoot && _draw_id_ >= 0 && _main_id_ == main._mainId()) {
        main._draw_deep_ += _last_point_count_;
        return;
    }
    if (!is_draw_dirty && toRoot) {
        main._draw_deep_ += _last_point_count_;
        return;
    }

    auto& cache     = _draw_cache_;
    auto& mainCache = main._draw_cache_;

    u32  deep        = main._draw_deep_;
    bool isFirstTime = _draw_id_ < 0 || _main_id_ != main._mainId();

    if (!toRoot) {
        main.drawn = true;
        _inheritData(main.color(), main.layer());

        if (_main_id_ != main._mainId()) {
            _main_id_          = main._mainId();
            _draw_id_          = -1;
            _last_point_count_ = 0;
        }

        if (!isFirstTime && deep < (u32)_draw_id_) {
            mainCache.erase(mainCache.begin() + deep, mainCache.begin() + _draw_id_);
        }
    }

    if (is_draw_dirty) {
        cache.clear();
        cache.push_back(DrawPack{ color(), _contourImpl() });
        is_draw_dirty = false;
    }

    _current_point_count_ = (u32)cache.size();

    if (!toRoot) {
        if (isFirstTime) {
            mainCache.insert(mainCache.begin() + deep, cache.begin(), cache.end());
        } else {
            u32 overlap = std::min(_last_point_count_, _current_point_count_);
            std::copy(
                cache.begin(), cache.begin() + overlap,
                mainCache.begin() + deep
            );

            if (_last_point_count_ < _current_point_count_) {
                mainCache.insert(
                    mainCache.begin() + deep + _last_point_count_,
                    cache.begin() + _last_point_count_, cache.end()
                );
            } else if (_last_point_count_ > _current_point_count_) {
                mainCache.erase(
                    mainCache.begin() + deep + _current_point_count_,
                    mainCache.begin() + deep + _last_point_count_
                );
            }
        }

        _draw_id_ = (i32)deep;
    }

    _last_point_count_ = _current_point_count_;
    main._draw_deep_   = (toRoot ? 0 : deep) + _current_point_count_;

    drawn = true;
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