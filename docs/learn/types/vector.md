# Vector

---

## Overview

**Vectors**, **dimensions**, **factors** and **grids** are four closely related types, each holding two numeric members, 
but each carrying a distinct meaning depending on what they represent: a position, a size, a scaling factor, or a grid coordinate.

---

## Vector

`Vec2d` represents a **position** in space. It holds two `f32` members:

- `f32 x` : coordinate on the X axis
- `f32 y` : coordinate on the Y axis

```cpp
constexpr Vec2d(void);
constexpr Vec2d(f32 XY);
constexpr Vec2d(f32 X, f32 Y);
```

`Vec2d` converts implicitly to both `Fact2d` and `Grid2d` :

```cpp
operator Fact2d(void); // negative components clamped to 0
operator Grid2d(void); // cast to usize
```

---

## Dimension

`Dim2d` represents a **size**, holding two `f32` members:

- `f32 w` : width
- `f32 h` : height

```cpp
constexpr Dim2d(void);
constexpr Dim2d(f32 WH);
constexpr Dim2d(f32 W, f32 H);
```

---

## Factor

`Fact2d` represents a **scaling factor**, holding two `f32` members, both clamped to a minimum of `0` at construction:

- `f32 x` : factor on the X axis
- `f32 y` : factor on the Y axis

```cpp
constexpr Fact2d(void);
constexpr Fact2d(f32 XY);
constexpr Fact2d(f32 X, f32 Y);
```

It converts implicitly to `Vec2d` :

```cpp
operator Vec2d(void);
```

---

## Grid

`Grid2d` represents a **grid coordinate** or a **number of divisions**, holding two `usize` members:

- `usize x` : column
- `usize y` : row

```cpp
constexpr Grid2d(void);
constexpr Grid2d(usize XY);
constexpr Grid2d(usize X, usize Y);
```

It converts implicitly to `Vec2d` :

```cpp
operator Vec2d(void);
```

---

## Usage

All four types live in the header **"remake2d/vector.hpp"**:

```cpp
#include <remake2d/vector.hpp>

int main(void) {
    
    rmk::Vec2d vec   = { 50.0f, 30.0f }; // position at (50, 30)
    rmk::Dim2d dim   = { 400, 300 };     // size 400x300
    rmk::Fact2d fact = { 1.5f, 1.5f };   // scaling factor (1.5, 1.5)
    rmk::Grid2d grid = { 3, 3 };         // 3x3 grid
    
    rmk::f32 x = vec.x; // retrieve the X value
    rmk::f32 h = dim.h; // retrieve the height value
    
}
```

!!! info
    None of these four types expose any method beyond their constructors and implicit conversions. All are **comparable**, 
	**copyable**, **movable**, and support `constexpr`.

---

## Arithmetic

`Vec2d`, `Dim2d`, `Fact2d` and `Grid2d` all support the following operators: `+`, `-`, `/`, `*`, `%`, `+=`, `-=`, `/=`, `*=`, and `%=`.
Since these operators aren't native to these types, the header **"remake2d/math.hpp"** must be included to use them:

```cpp
#include <remake2d/math.hpp>

int main(void) {
    rmk::Vec2d vec1 = { 100, 200 };
    rmk::Vec2d vec2 = { 200, 300 };
    
    rmk::Vec2d vec3 = vec1 + vec2; // { 300, 500 }
    rmk::Vec2d vec4 = vec1 - vec2; // { -100, -100 }
    rmk::Vec2d vec5 = vec1 / vec2; // { 0.5, 0.666... }
    
}
```

The same applies to `Dim2d`, `Fact2d` and `Grid2d`. See [Math](../tools/math.md) for the full set of operators, including the ones available for `Area` and `Color`.

---

[:octicons-arrow-left-24: Previous chapter](numeric.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](area.md){ .md-button .md-button--primary }