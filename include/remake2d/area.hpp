#ifndef REMAKE2D_AREA_
#define REMAKE2D_AREA_

#include <remake2d/vector.hpp>
#include <remake2d/private/draw.hpp>

#include <array>

namespace rmk {

struct Triangulation {
    Vec2d a, b, c;

public:
    constexpr Triangulation(void)                             = default;
    constexpr Triangulation(Triangulation&&)                  = default;
    constexpr Triangulation(const Triangulation&)             = default;
    constexpr Triangulation& operator=(Triangulation&&)       = default;
    constexpr Triangulation& operator=(const Triangulation&)  = default;
    constexpr Triangulation(const Vec2d& A, const Vec2d& B, const Vec2d& C) : a(A), b(B), c(C) {}

public:
    constexpr auto operator<=>(const Triangulation&) const noexcept = default;
};

struct Area : public Drawable, public Fillable {
    i32 x{0}, y{0}, w{0}, h{0};

public:
    Area(void)                    = default;
    Area(Area&&)                  = default;
    Area(const Area&)             = default;
    Area& operator=(Area&&)       = default;
    Area& operator=(const Area&)  = default;

    Area(i32 X, i32 Y, i32 W, i32 H)     : x(X), y(Y), w(W), h(H)         {}
    Area(const Vec2d& p, const Dim2d& s) : x(p.x), y(p.y), w(s.w), h(s.h) {}

public:
    Vec2d pos(void)    const noexcept;
    Dim2d size(void)   const noexcept;
    Vec2d center(void) const noexcept;

public:
    operator SDL_Rect(void) const;

    // compare dimension only
    bool operator<(const Area&)  const noexcept;
    bool operator>(const Area&)  const noexcept;
    bool operator==(const Area&) const noexcept;
    bool operator<=(const Area&) const noexcept;
    bool operator>=(const Area&) const noexcept;

public:
    std::array<Triangulation, 2> toTriangulation(void) const noexcept;

public:
    void draw(const Drawable&) const noexcept override;
    void fill(const Fillable&) const noexcept override;
};

} // namespace rmk
#endif