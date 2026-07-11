#ifndef REMAKE2D_SHAPE_TPP_
#define REMAKE2D_SHAPE_TPP_

namespace rmk{

template<IsShape S> S Geometry::as(void) const noexcept {
    return S(m_center, m_size);
}


template<size_t POINT_COUNT> requires (POINT_COUNT > (u8)point::min && POINT_COUNT <= (u8)point::max)
Shape<POINT_COUNT>::Shape(const Vec2d& center,const Dim2d& size) : Geometry(center, size)  {
    _build();
}

template<size_t POINT_COUNT> requires (POINT_COUNT > (u8)point::min && POINT_COUNT <= (u8)point::max)
void Shape<POINT_COUNT>::_triangulate(void) noexcept {
    if (!m_is_changed || m_n < 3) return;
    for (size_t i = 0; i < m_n - 2; i++) {
        m_tris[i] = {m_points[0], m_points[i+1], m_points[i+2]};
    }
    size_t vi = 0;
    size_t n = m_n - 2;
    for (size_t i = 0; i < n; i++) {
        m_vertex_cache[vi++] = {m_tris[i].a, {255, 255, 255, 255}, {0, 0}};
        m_vertex_cache[vi++] = {m_tris[i].b, {255, 255, 255, 255}, {0, 0}};
        m_vertex_cache[vi++] = {m_tris[i].c, {255, 255, 255, 255}, {0, 0}};
    }
    for (size_t i = 0; i < m_n; i++) {
        m_contour_cache[i] = m_points[i];
    }
    m_contour_cache[m_n] = m_contour_cache[0];
    
    m_is_changed = false;
}


template<size_t POINT_COUNT> requires (POINT_COUNT > (u8)point::min && POINT_COUNT <= (u8)point::max)
std::vector<SDL_FPoint> Shape<POINT_COUNT>::_toContour(void) const noexcept {
    return std::vector<SDL_FPoint>(m_contour_cache, m_contour_cache + m_n + 1);
}

template<size_t POINT_COUNT> requires (POINT_COUNT > (u8)point::min && POINT_COUNT <= (u8)point::max)
std::vector<SDL_Vertex> Shape<POINT_COUNT>::_toVertices(void) const noexcept {
    auto n = (m_n >= 3) ? (m_n - 2) * 3 : 0;
    return std::vector<SDL_Vertex>(m_vertex_cache, m_vertex_cache + n);
}

template<size_t POINT_COUNT> requires (POINT_COUNT > (u8)point::min && POINT_COUNT <= (u8)point::max)
const Triangulation* Shape<POINT_COUNT>::_triangulations(void) const noexcept {
    return m_tris;
}

template<size_t POINT_COUNT> requires (POINT_COUNT > (u8)point::min && POINT_COUNT <= (u8)point::max)
Vec2d Shape<POINT_COUNT>::center(void) const noexcept {
    return m_center;
} 

template<size_t POINT_COUNT> requires (POINT_COUNT > (u8)point::min && POINT_COUNT <= (u8)point::max)
Dim2d Shape<POINT_COUNT>::size(void) const noexcept {
    return m_size;
}

template<size_t POINT_COUNT> requires (POINT_COUNT > (u8)point::min && POINT_COUNT <= (u8)point::max)
u8 Shape<POINT_COUNT>::points(void) const noexcept {
    return m_n;
}

template<size_t POINT_COUNT> requires (POINT_COUNT > (u8)point::min && POINT_COUNT <= (u8)point::max)
const Vec2d* Shape<POINT_COUNT>::pointsPos(void) const noexcept {
    return const_cast<const Vec2d*>(m_points);
}

template<size_t POINT_COUNT> requires (POINT_COUNT > (u8)point::min && POINT_COUNT <= (u8)point::max)
void Shape<POINT_COUNT>::move(const Vec2d& center) noexcept {
    Vec2d delta = {center.x - m_center.x, center.y - m_center.y};
    transform(delta, 0, {1});
    m_center = center;
}

template<size_t POINT_COUNT> requires (POINT_COUNT > (u8)point::min && POINT_COUNT <= (u8)point::max)
void Shape<POINT_COUNT>::rotate(f32 angle) noexcept {
    transform({0}, angle, {1});
}

template<size_t POINT_COUNT> requires (POINT_COUNT > (u8)point::min && POINT_COUNT <= (u8)point::max)
void Shape<POINT_COUNT>::scale(const Fact2d& scaling) noexcept {
    transform({0}, 0, scaling);
}

template<size_t POINT_COUNT> requires (POINT_COUNT > (u8)point::min && POINT_COUNT <= (u8)point::max)
void Shape<POINT_COUNT>::resize(const Dim2d& size) noexcept {
	if (m_size.w == 0.0f || m_size.h == 0.0f) return;
    Fact2d delta = { size.w  / m_size.w, size.h / m_size.h };
    transform({0}, 0, delta);
    m_size = size;
}

template<size_t POINT_COUNT> requires (POINT_COUNT > (u8)point::min && POINT_COUNT <= (u8)point::max)
void Shape<POINT_COUNT>::transform(const Vec2d& delta, f32 angle, const Fact2d& scaling) noexcept {
    
    f32 cosA = std::cos(angle);
    f32 sinA = std::sin(angle);

    for(auto& p : m_points) {
        // translation
        f32 x = p.x - m_center.x;
        f32 y = p.y - m_center.y;
        // scaling
        x *= scaling.x;
        y *= scaling.y;
        // rotation
        Vec2d rotation = { x * cosA - y * sinA, x * sinA + y * cosA };
        //translation
        p = { m_center.x + rotation.x + delta.x, m_center.y + rotation.y + delta.y };
    }
    m_center = { m_center.x + delta.x, m_center.y + delta.y };
    m_is_changed = true;
	_triangulate();
}

template<size_t POINT_COUNT> requires (POINT_COUNT > (u8)point::min && POINT_COUNT <= (u8)point::max)
bool Shape<POINT_COUNT>::hasIntersected(const Geometry& other) const noexcept {
    auto getBounds = [](const Vec2d pts[], u8 n) {
        f32 minX = INFINITY, minY = INFINITY;
        f32 maxX = -INFINITY, maxY = -INFINITY;
        for(int i = 0; i < n; i++) {
            minX = std::min(minX, pts[i].x);
            maxX = std::max(maxX, pts[i].x);
            minY = std::min(minY, pts[i].y);
            maxY = std::max(maxY, pts[i].y);
        }
        return std::tuple<f32,f32,f32,f32>(minX, maxX, minY, maxY);
    };

    auto [minAx, maxAx, minAy, maxAy] = getBounds(m_points, m_n);
    auto [minBx, maxBx, minBy, maxBy] = getBounds(other.pointsPos(), other.points());

    if(maxAx < minBx || maxBx < minAx || maxAy < minBy || maxBy < minAy) return false;

    auto checkAxes = [](const Vec2d A[], const u8& n1, const Vec2d B[], const u8& n2) {
        for(size_t i = 0; i < n1 - 1; i++) {
            Vec2d edge = {A[i+1].x - A[i].x, A[i+1].y - A[i].y};
            Vec2d axis = {-edge.y, edge.x};

            f32 minA = INFINITY, maxA = -INFINITY;
            for(int j = 0; j < n1; j++) {
                f32 proj = A[j].x * axis.x + A[j].y * axis.y;
                minA = std::min(minA, proj);
                maxA = std::max(maxA, proj);
            }

            f32 minB = INFINITY, maxB = -INFINITY;
            for(int j = 0; j < n2; j++) {
                f32 proj = B[j].x * axis.x + B[j].y * axis.y;
                minB = std::min(minB, proj);
                maxB = std::max(maxB, proj);
            }
            if(maxA < minB || maxB < minA) return false;
        }
        return true;
    };

    return checkAxes(m_points, m_n, other.pointsPos(), other.points()) && checkAxes(other.pointsPos(), other.points(), m_points, m_n);
}

template<size_t POINT_COUNT> requires (POINT_COUNT > (u8)point::min && POINT_COUNT <= (u8)point::max)
void Shape<POINT_COUNT>::_build(void) noexcept {
    f32 cosA, sinA;
    for(u16 i = 0; i < m_n; i++) {
        f32 angle = 2.0f * M_PI * i / m_n;
        cosA = std::cos(angle);
        sinA = std::sin(angle);
        m_points[i] = {
            m_center.x + m_size.w / 2.0f * cosA,
            m_center.y + m_size.h / 2.0f * sinA
        };
    }
    m_points[m_n] = m_points[0];
    m_is_changed = true;
    _triangulate();
}


}//namespace rmk
#endif