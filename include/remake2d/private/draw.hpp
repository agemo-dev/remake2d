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
private:
    mutable Color  m_color{color::white};

public:
    mutable bool    is_draw_dirty{true};
    mutable bool    is_fill_dirty{true};

public:
    void  color(Color)      noexcept;
    Color color(void) const noexcept;

protected:
    void _color(Color) const noexcept;

public:
    rmk_heritableBaseClass(Printable);
};


class Drawable : public virtual Printable {
public:
    mutable bool  drawn{false};

public:
    mutable std::vector<DrawPack> _draw_cache_;

public:
    virtual void draw(const Drawable&) const noexcept = 0;

public:
    const std::vector<DrawPack>& _draw_(void) const;

public:
    rmk_heritableBaseClass(Drawable);
};


class Fillable : public virtual Printable {
public:
    mutable bool filled{false};

public:
    mutable std::vector<VertexBatch> _fill_cache_;

public:
    const std::vector<VertexBatch>& _fill_(void) const;

public:
    virtual void fill(const Fillable&) const noexcept = 0;

public:
    rmk_heritableBaseClass(Fillable);
};

} // namespace rmk

#endif