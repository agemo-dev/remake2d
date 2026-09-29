#ifndef REMAKE2D_DRAW_
#define REMAKE2D_DRAW_

#include <remake2d/color.hpp>
#include <remake2d/vector.hpp>
#include <remake2d/numeric.hpp>
#include <remake2d/private/point.hpp>
#include <remake2d/config/forward.hpp>
#include <remake2d/private/struct.hpp>
#include <remake2d/private/ivector.hpp>

#include <vector>

namespace rmk {

struct Vertex {
    f32   x{0.0f}, y{0.0f};
    Color color{color::white};
    f32   u{0.0f}, v{0.0f};

public:
    inline constexpr Vertex(f32 X, f32 Y, Color C, f32 U = 0, f32 V = 0) : x(X), y(Y), color(C), u(U), v(V) {}

public:
    operator SDL_Vertex(void) const;

public:
    rmk_baseClass(Vertex);
};

struct VertexBatch {
public:
    SDL_Texture*  texture{nullptr};
    IVector<Vertex, (((usize)point::max - 2) * 3)>  vertices{};
};

struct DrawPack {
public:
    Color  color{color::white};
    IVector<Vec2d, (usize)point::max> points{};
};

namespace contour {
Vec2d        breaker(void)    noexcept;
bool         isBreak(Vec2d)   noexcept;
} // namespace contour

class Printable {

// general part
private:
    mutable i16    m_layer{0};
    mutable Color  m_color{color::white};

public:
    void  color(Color)      noexcept;
    Color color(void) const noexcept;

public:
    void  layer(i16)        noexcept;
    i16   layer(void) const noexcept;

public:
    from(const Printable&);

protected:
    void _inheritData(Color, i16) const noexcept;

public:
    // Identity of this object as a composite "main", used by a child's
    // reparent check (see Area::draw/fill): the object's own address,
    // stable for as long as it's alive at this location, and distinct
    // from any other object's
    u32 _mainId(void) const noexcept { return (u32)(usize)this; }

// Draw part
public:
    mutable i32   _draw_id_{-1};
    mutable u32   _main_id_{0};
    mutable u32   _draw_deep_{0};
    mutable u32   _last_point_count_{0};
    mutable u32   _current_point_count_{0};

public:
    mutable bool  drawn{false};
    mutable bool  is_draw_dirty{true};

public:
    mutable std::vector<DrawPack> _draw_cache_;

public:
    virtual void draw(const Printable&) const noexcept {};

public:
    const std::vector<DrawPack>& _draw_(void) const;

// Fill part
public:
    mutable i32   _fill_id_{-1};
    mutable u32   _fill_deep_{0};
    mutable u32   _main_fill_id_{0};
    mutable u32   _last_vertex_count_{0};
    mutable u32   _current_vertex_count_{0};

public:
    mutable bool filled{false};
    mutable bool is_fill_dirty{true};

public:
    mutable std::vector<VertexBatch> _fill_cache_;

public:
    const std::vector<VertexBatch>& _fill_(void) const;

public:
    virtual void fill(const Printable&) const noexcept {};

public:
    rmk_heritableBaseClass(Printable);
};

} // namespace rmk

#endif