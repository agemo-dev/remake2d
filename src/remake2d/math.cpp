#include <remake2d/math.hpp>

#include <cmath>

namespace rmk {

Color& operator+=(Color& lhs, const Color& rhs) noexcept {
    return lhs = {
        (byte)std::min(255, lhs.r + rhs.r),
        (byte)std::min(255, lhs.g + rhs.g),
        (byte)std::min(255, lhs.b + rhs.b),
        (byte)std::min(255, lhs.a + rhs.a)
    };
}

Color& operator-=(Color& lhs, const Color& rhs) noexcept {
    return lhs = {
        (byte)std::max(0, lhs.r - rhs.r),
        (byte)std::max(0, lhs.g - rhs.g),
        (byte)std::max(0, lhs.b - rhs.b),
        (byte)std::max(0, lhs.a - rhs.a)
    };
}

Color& operator%=(Color& lhs, const Color& rhs) noexcept {
    return lhs = {
        (byte)((lhs.r + rhs.r) / 2),
        (byte)((lhs.g + rhs.g) / 2),
        (byte)((lhs.b + rhs.b) / 2),
        (byte)((lhs.a + rhs.a) / 2)
    };
}

Color operator+(const Color& lhs, const Color& rhs) noexcept {
    return {
        (byte)std::min(255, lhs.r + rhs.r),
        (byte)std::min(255, lhs.g + rhs.g),
        (byte)std::min(255, lhs.b + rhs.b),
        (byte)std::min(255, lhs.a + rhs.a)
    };
}

Color operator-(const Color& lhs, const Color& rhs) noexcept {
    return {
        (byte)std::max(0, lhs.r - rhs.r),
        (byte)std::max(0, lhs.g - rhs.g),
        (byte)std::max(0, lhs.b - rhs.b),
        (byte)std::max(0, lhs.a - rhs.a)
    };
}

Color operator%(const Color& lhs, const Color& rhs) noexcept {
    return {
        (byte)((lhs.r + rhs.r) / 2),
        (byte)((lhs.g + rhs.g) / 2),
        (byte)((lhs.b + rhs.b) / 2),
        (byte)((lhs.a + rhs.a) / 2)
    };
}

Vec2d& operator+=(Vec2d& lhs, const Vec2d& rhs) noexcept {
    return lhs = lhs + rhs;
}

Vec2d& operator-=(Vec2d& lhs, const Vec2d& rhs) noexcept {
    return lhs = lhs - rhs;
}

Vec2d& operator/=(Vec2d& lhs, const Vec2d& rhs) noexcept {
    return lhs = lhs / rhs;
}

Vec2d& operator*=(Vec2d& lhs, const Vec2d& rhs) noexcept {
    return lhs = lhs * rhs;
}

Vec2d& operator%=(Vec2d& lhs, const Vec2d& rhs) noexcept {
    return lhs = lhs % rhs;
}

Vec2d operator+(const Vec2d& lhs, const Vec2d& rhs) noexcept {
    return { lhs.x + rhs.x, lhs.y + rhs.y };
}

Vec2d operator-(const Vec2d& lhs, const Vec2d& rhs) noexcept {
    return { lhs.x - rhs.x, lhs.y - rhs.y };
}

Vec2d operator/(const Vec2d& lhs, const Vec2d& rhs) noexcept {
    return { lhs.x / rhs.x, lhs.y / rhs.y };
}

Vec2d operator*(const Vec2d& lhs, const Vec2d& rhs) noexcept {
    return { lhs.x * rhs.x, lhs.y * rhs.y };
}

Vec2d operator%(const Vec2d& lhs, const Vec2d& rhs) noexcept {
    return { std::fmod(lhs.x, rhs.x), std::fmod(lhs.y, rhs.y) };
}

Fact2d& operator+=(Fact2d& lhs, const Fact2d& rhs) noexcept {
    return lhs = lhs + rhs;
}

Fact2d& operator-=(Fact2d& lhs, const Fact2d& rhs) noexcept {
    return lhs = lhs - rhs;
}

Fact2d& operator/=(Fact2d& lhs, const Fact2d& rhs) noexcept {
    return lhs = lhs / rhs;
}

Fact2d& operator*=(Fact2d& lhs, const Fact2d& rhs) noexcept {
    return lhs = lhs * rhs;
}

Fact2d& operator%=(Fact2d& lhs, const Fact2d& rhs) noexcept {
    return lhs = lhs % rhs;
}

Fact2d operator+(const Fact2d& lhs, const Fact2d& rhs) noexcept {
    return { lhs.x + rhs.x, lhs.y + rhs.y };
}

Fact2d operator-(const Fact2d& lhs, const Fact2d& rhs) noexcept {
    return { lhs.x - rhs.x, lhs.y - rhs.y };
}

Fact2d operator/(const Fact2d& lhs, const Fact2d& rhs) noexcept {
    return { lhs.x / rhs.x, lhs.y / rhs.y };
}

Fact2d operator*(const Fact2d& lhs, const Fact2d& rhs) noexcept {
    return { lhs.x * rhs.x, lhs.y * rhs.y };
}

Fact2d operator%(const Fact2d& lhs, const Fact2d& rhs) noexcept {
    return { std::fmod(lhs.x, rhs.x), std::fmod(lhs.y, rhs.y) };
}

Grid2d& operator+=(Grid2d& lhs, const Grid2d& rhs) noexcept {
    return lhs = lhs - rhs;
}

Grid2d& operator-=(Grid2d& lhs, const Grid2d& rhs) noexcept {
    return lhs = lhs - rhs;
}

Grid2d& operator/=(Grid2d& lhs, const Grid2d& rhs) noexcept {
    return lhs = lhs / rhs;
}

Grid2d& operator*=(Grid2d& lhs, const Grid2d& rhs) noexcept {
    return lhs = lhs * rhs;
}

Grid2d& operator%=(Grid2d& lhs, const Grid2d& rhs) noexcept {
    return lhs = lhs % rhs;
}

Grid2d operator+(const Grid2d& lhs, const Grid2d& rhs) noexcept {
    return { lhs.x + rhs.x, lhs.y + rhs.y };
}

Grid2d operator-(const Grid2d& lhs, const Grid2d& rhs) noexcept {
    return { lhs.x - rhs.x, lhs.y - rhs.y };
}

Grid2d operator/(const Grid2d& lhs, const Grid2d& rhs) noexcept {
    return { lhs.x / rhs.x, lhs.y / rhs.y };
}

Grid2d operator*(const Grid2d& lhs, const Grid2d& rhs) noexcept {
    return { lhs.x * rhs.x, lhs.y * rhs.y };
}

Grid2d operator%(const Grid2d& lhs, const Grid2d& rhs) noexcept {
    return { lhs.x % rhs.x, lhs.y % rhs.y };
}

Dim2d& operator+=(Dim2d& lhs, const Dim2d& rhs) noexcept {
    return lhs = lhs + rhs;
}

Dim2d& operator-=(Dim2d& lhs, const Dim2d& rhs) noexcept {
    return lhs = lhs - rhs;
}

Dim2d& operator/=(Dim2d& lhs, const Dim2d& rhs) noexcept {
    return lhs = lhs / rhs;
}

Dim2d& operator*=(Dim2d& lhs, const Dim2d& rhs) noexcept {
    return lhs = lhs * rhs;
}

Dim2d& operator%=(Dim2d& lhs, const Dim2d& rhs) noexcept {
    return lhs = lhs % rhs;
}

Dim2d operator+(const Dim2d& lhs, const Dim2d& rhs) noexcept {
    return { lhs.w + rhs.w, lhs.h + rhs.h };
}

Dim2d operator-(const Dim2d& lhs, const Dim2d& rhs) noexcept {
    return { lhs.w - rhs.w, lhs.h - rhs.h };
}

Dim2d operator/(const Dim2d& lhs, const Dim2d& rhs) noexcept {
    return { lhs.w / rhs.w, lhs.h / rhs.h };
}

Dim2d operator*(const Dim2d& lhs, const Dim2d& rhs) noexcept {
    return { lhs.w * rhs.w, lhs.h * rhs.h };
}

Dim2d operator%(const Dim2d& lhs, const Dim2d& rhs) noexcept {
    return { std::fmod(lhs.w, rhs.w), std::fmod(lhs.h, rhs.h) };
}

Area& operator+=(Area& lhs, const Area& rhs) noexcept {
    return lhs = lhs + rhs;
}

Area& operator-=(Area& lhs, const Area& rhs) noexcept {
    return lhs = lhs - rhs;
}

Area& operator/=(Area& lhs, const Area& rhs) noexcept {
    return lhs = lhs / rhs;
}

Area& operator*=(Area& lhs, const Area& rhs) noexcept {
    return lhs = lhs * rhs;
}

Area& operator%=(Area& lhs, const Area& rhs) noexcept {
    return lhs = lhs % rhs;
}

Area operator+(const Area& lhs, const Area& rhs) noexcept {
    return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.w + rhs.w, lhs.h + rhs.h };
}

Area operator-(const Area& lhs, const Area& rhs) noexcept {
    return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.w - rhs.w, lhs.h - rhs.h };
}

Area operator/(const Area& lhs, const Area& rhs) noexcept {
    return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.w / rhs.w, lhs.h / rhs.h };
}

Area operator*(const Area& lhs, const Area& rhs) noexcept {
    return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.w * rhs.w, lhs.h * rhs.h };
}

Area operator%(const Area& lhs, const Area& rhs) noexcept {
    return { lhs.x % rhs.x, lhs.y % rhs.y, lhs.w % rhs.w, lhs.h % rhs.h };
}

} //namespace rmk