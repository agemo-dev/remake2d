#ifndef REMAKE2D_DRAW_
#define REMAKE2D_DRAW_

#include <remake2d/color.hpp>
#include <remake2d/numeric.hpp>
#include <remake2d/config/forward.hpp>
#include <remake2d/private/struct.hpp>

#include <vector>

namespace rmk {

struct Vertex {
    f32   x{0.0f}, y{0.0f};
    Color color{color::white};
    f32   u{0.0f}, v{0.0f};

public:
    constexpr Vertex(f32, f32, Color, f32 = 0.0f, f32 = 0.0f);

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
    IVector<SDL_FPoint, (usize)point::max> points{};
};

namespace contour {
SDL_FPoint           breaker(void)             noexcept;
bool                   isBreak(SDL_FPoint)     noexcept;
} // namespace contour


class Printable {
private:
    Color  m_color{color::white};

public:
    mutable bool    is_draw_dirty{true};
    mutable bool    is_fill_dirty{true};

public:
    void  color(Color)      noexcept;
    Color color(void) const noexcept;

public:
    rmk_heritableBaseClass(Printable);
};


class Drawable : public virtual Printable {
public:
    mutable bool  drawn{false};

public:
    mutable std::vector<DrawPack> _draw_cache_(5);

public:
    virtual void draw(const Drawable&) noexcept = 0;

public:
    const std::vector<DrawPack>& _draw_(void);

public:
    rmk_heritableBaseClass(Drawable);
};


class Fillable : public virtual Printable {
public:
    mutable bool filled{false};

public:
    mutable std::vector<VertexBatch> _fill_cache_(5);

public:
    const std::vector<VertexBatch>& _fill_(void);

public:
    virtual void fill(const Fillable&) noexcept = 0;

public:
    rmk_heritableBaseClass(Fillable);
};

} // namespace rmk

#endif