#ifndef REMAKE2D_MATH_
#define REMAKE2D_MATH_

#include <remake2d/all/types.hpp>
#include <remake2d/clock.hpp>


namespace rmk {
    
inline constexpr fmax pi = 3.14159265358979;


Color& operator+=(Color&, const Color&) noexcept;
Color& operator-=(Color&, const Color&) noexcept;
Color& operator%=(Color&, const Color&) noexcept;

Color operator+(const Color&, const Color&) noexcept;
Color operator-(const Color&, const Color&) noexcept;
Color operator%(const Color&, const Color&) noexcept;


Vec2d& operator+=(Vec2d&, const Vec2d&) noexcept;
Vec2d& operator-=(Vec2d&, const Vec2d&) noexcept;
Vec2d& operator/=(Vec2d&, const Vec2d&) noexcept;
Vec2d& operator*=(Vec2d&, const Vec2d&) noexcept;
Vec2d& operator%=(Vec2d&, const Vec2d&) noexcept;

Vec2d operator+(const Vec2d&, const Vec2d&) noexcept;
Vec2d operator-(const Vec2d&, const Vec2d&) noexcept;
Vec2d operator/(const Vec2d&, const Vec2d&) noexcept;
Vec2d operator*(const Vec2d&, const Vec2d&) noexcept;
Vec2d operator%(const Vec2d&, const Vec2d&) noexcept;

Fact2d& operator+=(Fact2d&, const Fact2d&) noexcept;
Fact2d& operator-=(Fact2d&, const Fact2d&) noexcept;
Fact2d& operator/=(Fact2d&, const Fact2d&) noexcept;
Fact2d& operator*=(Fact2d&, const Fact2d&) noexcept;
Fact2d& operator%=(Fact2d&, const Fact2d&) noexcept;

Fact2d operator+(const Fact2d&, const Fact2d&) noexcept;
Fact2d operator-(const Fact2d&, const Fact2d&) noexcept;
Fact2d operator/(const Fact2d&, const Fact2d&) noexcept;
Fact2d operator*(const Fact2d&, const Fact2d&) noexcept;
Fact2d operator%(const Fact2d&, const Fact2d&) noexcept;

Grid2d& operator+=(Grid2d&, const Grid2d&) noexcept;
Grid2d& operator-=(Grid2d&, const Grid2d&) noexcept;
Grid2d& operator/=(Grid2d&, const Grid2d&) noexcept;
Grid2d& operator*=(Grid2d&, const Grid2d&) noexcept;
Grid2d& operator%=(Grid2d&, const Grid2d&) noexcept;

Grid2d operator+(const Grid2d&, const Grid2d&) noexcept;
Grid2d operator-(const Grid2d&, const Grid2d&) noexcept;
Grid2d operator/(const Grid2d&, const Grid2d&) noexcept;
Grid2d operator*(const Grid2d&, const Grid2d&) noexcept;
Grid2d operator%(const Grid2d&, const Grid2d&) noexcept;

Dim2d& operator+=(Dim2d&, const Dim2d&) noexcept;
Dim2d& operator-=(Dim2d&, const Dim2d&) noexcept;
Dim2d& operator/=(Dim2d&, const Dim2d&) noexcept;
Dim2d& operator*=(Dim2d&, const Dim2d&) noexcept;
Dim2d& operator%=(Dim2d&, const Dim2d&) noexcept;

Dim2d operator+(const Dim2d&, const Dim2d&) noexcept;
Dim2d operator-(const Dim2d&, const Dim2d&) noexcept;
Dim2d operator/(const Dim2d&, const Dim2d&) noexcept;
Dim2d operator*(const Dim2d&, const Dim2d&) noexcept;
Dim2d operator%(const Dim2d&, const Dim2d&) noexcept;

Area& operator+=(Area&, const Area&) noexcept;
Area& operator-=(Area&, const Area&) noexcept;
Area& operator/=(Area&, const Area&) noexcept;
Area& operator*=(Area&, const Area&) noexcept;
Area& operator%=(Area&, const Area&) noexcept;

Area operator+(const Area&, const Area&) noexcept;
Area operator-(const Area&, const Area&) noexcept;
Area operator/(const Area&, const Area&) noexcept;
Area operator*(const Area&, const Area&) noexcept;
Area operator%(const Area&, const Area&) noexcept;


bool operator==(const Date&, const Date&) noexcept;
bool operator!=(const Date&, const Date&) noexcept;
bool operator<(const Date&, const Date&) noexcept;
bool operator>(const Date&, const Date&)  noexcept;
bool operator<=(const Date&, const Date&) noexcept;
bool operator>=(const Date&, const Date&) noexcept;


bool operator==(const Clock&, const Clock&) noexcept;
bool operator!=(const Clock&, const Clock&) noexcept;
bool operator<(const Clock&, const Clock&) noexcept;
bool operator>(const Clock&, const Clock&)  noexcept;
bool operator<=(const Clock&, const Clock&) noexcept;
bool operator>=(const Clock&, const Clock&) noexcept;

} //namespace rmk
#endif