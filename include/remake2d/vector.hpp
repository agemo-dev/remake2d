#ifndef REMAKE2D_VECTOR_
#define REMAKE2D_VECTOR_

#include <remake2d/numeric.hpp>
#include <remake2d/private/struct.hpp>
#include <remake2d/config/forward.hpp>

#include <compare>

namespace rmk {

struct Fact2d;
struct Grid2d;

struct Vec2d {

public:
    f32 x{0.0f}, y{0.0f};

public:
    rmk_baseClass(Vec2d);

public:
    operator Fact2d(void) const noexcept;
    operator Grid2d(void) const noexcept;

public:
    constexpr Vec2d(f32 XY)       : x(XY), y(XY) {}
    constexpr Vec2d(f32 X, f32 Y) : x(X), y(Y)   {}

public:
    operator SDL_FPoint(void) const noexcept;
    constexpr auto operator<=>(const Vec2d&) const noexcept = default;
};


struct Dim2d {

public:
    f32 w{0.0f}, h{0.0f};

public:
    rmk_baseClass(Dim2d);

public:
    constexpr Dim2d(f32 WH)       : w(WH), h(WH) {}
    constexpr Dim2d(f32 W, f32 H) : w(W), h(H)   {}

public:
    constexpr auto operator<=>(const Dim2d&) const noexcept = default;
};

struct Fact2d {

public:
    f32 x{0.0f}, y{0.0f};

public:
    rmk_baseClass(Fact2d);

public:
    constexpr Fact2d(f32 X, f32 Y) : x(X < 0 ? 0 : X), y(Y < 0 ? 0 : Y)     {}
    constexpr Fact2d(f32 XY)       : x(XY < 0 ? 0 : XY), y(XY < 0 ? 0 : XY) {}

public:
    operator SDL_FPoint(void) const noexcept;
    operator Vec2d(void) const noexcept { return Vec2d{ x, y }; }
    constexpr auto operator<=>(const Fact2d&) const noexcept = default;
};

struct Grid2d {

public:
    usize x{0}, y{0};

public:
    rmk_baseClass(Grid2d);

public:
    constexpr Grid2d(usize XY) : x(XY), y(XY) {}
    constexpr Grid2d(usize X, usize Y) : x(X), y(Y) {}

public:
    operator Vec2d(void) const noexcept { return Vec2d{ (f32)x, (f32)y }; }
    operator SDL_Point(void) const noexcept;
    constexpr auto operator<=>(const Grid2d&) const noexcept = default;
};

} //namespace rmk
#endif