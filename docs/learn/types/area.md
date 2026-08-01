# Area

**Area** is a type used to **delimit** a specific zone, notably for defining **camera bounds**, **physics world limits**, and **tile regions**.

---

## Overview

**Area** is represented by the type `Area`, included in the header **"remake2d/vector.hpp"**.
It is composed of four integers:

- `i32 x` : coordinate on the X axis
- `i32 y` : coordinate on the Y axis
- `i32 w` : area's width
- `i32 h` : area's height

!!! info
    Unlike `Vec2d` and `Dim2d`, `Area` takes integer values rather than floats because it uses
    whole pixels for coordinates and size, not **relative** values.
    The `Area` type is also **comparable**.
    
---

##Method

The `Area` type own just one method:

```cpp
Vec2d center(void) noexcept; // get Area center
```

---

## Usage

Its constructor is as follows:

```cpp
constexpr Area(void);
constexpr Area(i32 x, i32 y, i32 w, i32 h);
```

```cpp
#include <remake2d/vector.hpp>

int main(void) {
    rmk::Area zone = { 400, 300, 100, 100 }; // Area at coordinates (400, 300) with size (100, 100)
}
```

---

## Arithmetic

**Area** also supports the following operators: `+`, `-`, `/`, `*`, `%`, `+=`, `-=`, `/=`, `*=`, and `%=`:

```cpp
#include <remake2d/math.hpp>

int main(void) {
    rmk::Area z1 = { 100, 200, 300, 400 };
    rmk::Area z2 = {  50, 100, 100, 200 };
    
    rmk::Area z3 = z1 + z2; // { 150, 300, 400, 600 }
    rmk::Area z4 = z1 - z2; // {  50, 100, 200, 200 }
    rmk::Area z5 = z1 * z2; // { 5000, 20000, 30000, 80000 }
    rmk::Area z6 = z1 % z2; // { 0, 0, 0, 0 }
}
```

---

[:octicons-arrow-left-24: Previous chapter](vector.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](color.md){ .md-button .md-button--primary }