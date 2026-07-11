#ifndef REMAKE2D_VECTOR_
#define REMAKE2D_VECTOR_

#include <remake2d/numeric.hpp>

#if __has_include(<SDL2/SDL.h>)
    #include <SDL2/SDL.h>
#elif __has_include(<SDL.h>)
    #include <SDL.h>
#else
    #error "SDL not found."
#endif

#include <compare>

namespace rmk {

struct Fact2d;
struct Grid2d;

struct Vec2d {
    f32 x, y;
    
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
    operator SDL_FPoint(void) { return SDL_FPoint{x, y}; }
    constexpr auto operator<=>(const Vec2d&) const noexcept = default;
};


struct Dim2d {
    f32 w, h;
    
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
    f32 x, y;
    
public:
    constexpr Fact2d(void)                      = default;
    constexpr Fact2d(Fact2d&&)                  = default;
    constexpr Fact2d(const Fact2d&)             = default;
    constexpr Fact2d& operator=(Fact2d&&)       = default;
    constexpr Fact2d& operator=(const Fact2d&)  = default;
    
public:
    constexpr Fact2d(f32 XY)       : x(XY < 0 ? 0 : XY), y(XY < 0 ? 0 : XY) {}
    constexpr Fact2d(f32 X, f32 Y) : x(X < 0 ? 0 : X), y(Y < 0 ? 0 : Y)     {}
    
public:
    operator Vec2d(void) { return Vec2d{ x, y }; }
    operator SDL_FPoint(void) { return SDL_FPoint{ x, y }; }
    constexpr auto operator<=>(const Fact2d&) const noexcept = default;
};

struct Grid2d {
    usize x, y;

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
	operator Vec2d(void)     { return Vec2d{ (f32)x, (f32)y }; }
    operator SDL_Point(void) { return SDL_Point{ (int)x, (int)y }; }
    constexpr auto operator<=>(const Grid2d&) const noexcept = default;
};

struct Area {
    i32 x, y, w, h;

public:
    constexpr Area(void)                    = default;
    constexpr Area(Area&&)                  = default;
    constexpr Area(const Area&)             = default;
    constexpr Area& operator=(Area&&)       = default;
    constexpr Area& operator=(const Area&)  = default;
    constexpr Area(i32 X, i32 Y, i32 W, i32 H)     : x(X), y(Y), w(W), h(H)         {}
	constexpr Area(const Vec2d& p, const Dim2d& s) : x(p.x), y(p.y), w(s.w), h(s.h) {}
    
public:
    operator SDL_Rect() const { return {x, y, w, h}; }
    constexpr auto operator<=>(const Area&) const noexcept = default;
    Vec2d center() const noexcept { return { (f32)(x + w / 2), (f32)(y + h / 2) }; }
    
};


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

inline Vec2d::operator Fact2d(void) { return Fact2d{ (x < 0 ? 0 : x), (y < 0 ? 0 : y) }; }
inline Vec2d::operator Grid2d(void) { return Grid2d{ (usize)x , (usize)y }; }

} //namespace rmk
#endif