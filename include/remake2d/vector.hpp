#ifndef REMAKE2D_VECTOR_
#define REMAKE2D_VECTOR_

#include <remake2d/numeric.hpp>
#include <remake2d/config/forward.hpp>

#include <compare>

namespace rmk {

struct Fact2d;
struct Grid2d;

struct Vec2d {
    f32 x{0.0f}, y{0.0f};

public:
    constexpr Vec2d(void)                     = default;
    constexpr Vec2d(Vec2d&&)                  = default;
    constexpr Vec2d(const Vec2d&)             = default;
    constexpr Vec2d& operator=(Vec2d&&)       = default;
    constexpr Vec2d& operator=(const Vec2d&)  = default;

public:
    operator Fact2d(void);
    operator Grid2d(void);
    constexpr Vec2d(f32 XY) 	  : x(XY), y(XY) {}
    constexpr Vec2d(f32 X, f32 Y) : x(X), y(Y)   {}

public:
    operator SDL_FPoint(void) const;
    constexpr auto operator<=>(const Vec2d&) const noexcept = default;
};


struct Dim2d {
    f32 w{0.0f}, h{0.0f};

public:
    constexpr Dim2d(void)                     = default;
    constexpr Dim2d(Dim2d&&)                  = default;
    constexpr Dim2d(const Dim2d&)             = default;
    constexpr Dim2d& operator=(Dim2d&&)       = default;
    constexpr Dim2d& operator=(const Dim2d&)  = default;
    
public:
    constexpr Dim2d(f32 WH)       : w(WH), h(WH) {}
    constexpr Dim2d(f32 W, f32 H) : w(W), h(H)   {}
    
public:
    constexpr auto operator<=>(const Dim2d&) const noexcept = default;
};

struct Fact2d {
    f32 x{0.0f}, y{0.0f};
    
public:
    constexpr Fact2d(void)                      = default;
    constexpr Fact2d(Fact2d&&)                  = default;
    constexpr Fact2d(const Fact2d&)             = default;
    constexpr Fact2d& operator=(Fact2d&&)       = default;
    constexpr Fact2d& operator=(const Fact2d&)  = default;
    
public:
    constexpr Fact2d(f32 X, f32 Y) : x(X < 0 ? 0 : X), y(Y < 0 ? 0 : Y)     {}
    constexpr Fact2d(f32 XY)       : x(XY < 0 ? 0 : XY), y(XY < 0 ? 0 : XY) {}
    
public:
    operator Vec2d(void) const { return Vec2d{ x, y }; }
    operator SDL_FPoint(void) const;
    constexpr auto operator<=>(const Fact2d&) const noexcept = default;
};

struct Grid2d {
    usize x{0}, y{0};

public:
    constexpr Grid2d(void)                      = default;
    constexpr Grid2d(Grid2d&&)                  = default;
    constexpr Grid2d(const Grid2d&)             = default;
    constexpr Grid2d& operator=(Grid2d&&)       = default;
    constexpr Grid2d& operator=(const Grid2d&)  = default;

public:
    constexpr Grid2d(usize XY) : x(XY), y(XY) {}
    constexpr Grid2d(usize X, usize Y) : x(X), y(Y) {}

public:
	operator Vec2d(void) const { return Vec2d{ (f32)x, (f32)y }; }
    operator SDL_Point(void) const;
    constexpr auto operator<=>(const Grid2d&) const noexcept = default;
};

} //namespace rmk
#endif