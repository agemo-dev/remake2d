# Math

The `math` module is an essential tool for manipulating and combining values of **RE:MAKE 2D**'s 
base types, whether for position, dimension, or color calculations.

---

## Overview

The `math` module gathers all the **arithmetic** and **comparison operators** usable on **RE:MAKE 2D**'s 
base types (`Vec2d`, `Dim2d`, `Fact2d`, `Grid2d`, `Area`, `Color`, `Date`, `Clock`). These types have no 
native operators in their respective headers, so the header **"remake2d/math.hpp"** must be included to use them.

!!! info
    The header **"remake2d/math.hpp"** already includes **"remake2d/all/types.hpp"** and **"remake2d/clock.hpp"**, 
	so there is no need to include them separately.

---

## Constant

The header also exposes the mathematical constant `pi`:

```cpp
inline constexpr fmax pi = 3.14159265358979;
```

---

## Arithmetic operators

The `Vec2d`, `Dim2d`, `Fact2d`, `Grid2d` and `Area` types support the following set of operators: `+`, `-`, `/`, `*`, `%`, 
along with their assignment equivalents `+=`, `-=`, `/=`, `*=`, `%=`:

```cpp
Vec2d operator+(const Vec2d&, const Vec2d&) noexcept;
Vec2d operator-(const Vec2d&, const Vec2d&) noexcept;
Vec2d operator/(const Vec2d&, const Vec2d&) noexcept;
Vec2d operator*(const Vec2d&, const Vec2d&) noexcept;
Vec2d operator%(const Vec2d&, const Vec2d&) noexcept; // fmod

Vec2d& operator+=(Vec2d&, const Vec2d&) noexcept;
Vec2d& operator-=(Vec2d&, const Vec2d&) noexcept;
Vec2d& operator/=(Vec2d&, const Vec2d&) noexcept;
Vec2d& operator*=(Vec2d&, const Vec2d&) noexcept;
Vec2d& operator%=(Vec2d&, const Vec2d&) noexcept;
```

The same set of operators is available for `Fact2d`, `Dim2d`, `Grid2d` and `Area`:

```cpp
Fact2d operator+(const Fact2d&, const Fact2d&) noexcept;
Fact2d operator-(const Fact2d&, const Fact2d&) noexcept;
Fact2d operator/(const Fact2d&, const Fact2d&) noexcept;
Fact2d operator*(const Fact2d&, const Fact2d&) noexcept;
Fact2d operator%(const Fact2d&, const Fact2d&) noexcept;

Dim2d operator+(const Dim2d&, const Dim2d&) noexcept;
Dim2d operator-(const Dim2d&, const Dim2d&) noexcept;
Dim2d operator/(const Dim2d&, const Dim2d&) noexcept;
Dim2d operator*(const Dim2d&, const Dim2d&) noexcept;
Dim2d operator%(const Dim2d&, const Dim2d&) noexcept;

Grid2d operator+(const Grid2d&, const Grid2d&) noexcept;
Grid2d operator-(const Grid2d&, const Grid2d&) noexcept;
Grid2d operator/(const Grid2d&, const Grid2d&) noexcept;
Grid2d operator*(const Grid2d&, const Grid2d&) noexcept;
Grid2d operator%(const Grid2d&, const Grid2d&) noexcept;

Area operator+(const Area&, const Area&) noexcept;
Area operator-(const Area&, const Area&) noexcept;
Area operator/(const Area&, const Area&) noexcept;
Area operator*(const Area&, const Area&) noexcept;
Area operator%(const Area&, const Area&) noexcept;
```

!!! info
    For `Grid2d` and `Area`, whose members are integers, the `%` operator performs a **classic integer modulo** rather 
	than an `fmod`, unlike `Vec2d`, `Dim2d` and `Fact2d` whose members are floating-point.

### Usage

```cpp
#include <remake2d/math.hpp>

int main(void) {
    rmk::Vec2d vec1 = { 100, 200 };
    rmk::Vec2d vec2 = { 200, 300 };

    rmk::Vec2d vec3 = vec1 + vec2; // { 300, 500 }
    rmk::Vec2d vec4 = vec1 - vec2; // { -100, -100 }
    rmk::Vec2d vec5 = vec1 / vec2; // { 0.5, 0.666... }

    vec1 += vec2; // modifies vec1 directly
}
```

```cpp
rmk::Grid2d g1 = { 10, 5 };
rmk::Grid2d g2 = { 3, 2 };

rmk::Grid2d g3 = g1 + g2; // { 13, 7 }
rmk::Grid2d g4 = g1 % g2; // { 1, 1 } (integer modulo)
```

```cpp
rmk::Area z1 = { 100, 200, 300, 400 };
rmk::Area z2 = {  50, 100, 100, 200 };

rmk::Area z3 = z1 + z2; // { 150, 300, 400, 600 }
rmk::Area z4 = z1 % z2; // { 0, 0, 0, 0 }
```

---

## Operators on `Color`

The `Color` type only supports the `+`, `-` and `%` operators (as well as `+=`, `-=`, `%=`), since division and multiplication
have no meaning on color components:

```cpp
Color operator+(const Color&, const Color&) noexcept;
Color operator-(const Color&, const Color&) noexcept;
Color operator%(const Color&, const Color&) noexcept;

Color& operator+=(Color&, const Color&) noexcept;
Color& operator-=(Color&, const Color&) noexcept;
Color& operator%=(Color&, const Color&) noexcept;
```

- `+` and `-`: combine colors by respectively selecting the maximum or minimum of each RGBA component, with a clamp between 0 and 255.
- `%`: computes the average of each RGBA component.

```cpp
rmk::Color purple = rmk::color::red + rmk::color::blue; // { 255, 0, 255, 255 }
rmk::Color mix     = rmk::color::red % rmk::color::blue; // average of the components
```

!!! info
    For more details on the behavior of these operators, refer to the [Color](../types/color.md) section.

---

## `Date` and `Clock` comparisons

The `math` module also provides the full set of comparison operators for the `Date` and `Clock` types, respectively comparing 
`year - month - day` and `hour - minute - second`:

```cpp
bool operator==(const Date&, const Date&) noexcept;
bool operator!=(const Date&, const Date&) noexcept;
bool operator<(const Date&, const Date&)  noexcept;
bool operator>(const Date&, const Date&)  noexcept;
bool operator<=(const Date&, const Date&) noexcept;
bool operator>=(const Date&, const Date&) noexcept;

bool operator==(const Clock&, const Clock&) noexcept;
bool operator!=(const Clock&, const Clock&) noexcept;
bool operator<(const Clock&, const Clock&)  noexcept;
bool operator>(const Clock&, const Clock&)  noexcept;
bool operator<=(const Clock&, const Clock&) noexcept;
bool operator>=(const Clock&, const Clock&) noexcept;
```

```cpp
rmk::Date d1(15, 6, 2026);
rmk::Date d2(20, 6, 2026);

if (d1 < d2) { /* d1 comes before d2 */ }
```

---

[:octicons-arrow-left-24: Previous chapter](../types/vector.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](stream.md){ .md-button .md-button--primary }