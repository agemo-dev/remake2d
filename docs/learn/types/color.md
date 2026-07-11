# Color

---

## Overview

**RE:MAKE 2D** colors primarily handle the RGBA format, but also expose the HSL (Hue - Saturation - Luminosity) representation 
for cases where interpolating or generating colors visually makes more sense than working component by component.

The `Color` structure has **4 members**, all of type `byte`:

- `byte r` : the amount of red
- `byte g` : the amount of green
- `byte b` : the amount of blue
- `byte a` : the opacity

Its constructor is as follows:

```cpp
constexpr Color(void);
constexpr Color(byte R, byte G, byte B, byte A = 255);
```

!!! info
    The `r`, `g`, `b`, and `a` values range from 0 to 255.
    For `r`, `g`, and `b`, the closer the value is to 0, the darker the color,
    and the closer the value is to 255, the lighter the color.
    For the `a` (*alpha*) value, the closer the value is to 0, the more transparent
    the color, and the closer the value is to 255, the more opaque it is.

---

## Usage

To use colors, include the header **"remake2d/color.hpp"**.

```cpp
#include <remake2d/color.hpp>

int main(void) {
    
    rmk::Color black(0, 0, 0, 255);      // pure black
    rmk::Color red(255, 0, 0, 255);      // pure red
    rmk::Color purple(255, 0, 255, 255); // purple
    
    rmk::byte r = red.r; // retrieve the amount of red

}
```

---

## Base Colors

**RE:MAKE 2D** provides **25 base colors**. These colors are
contained within the `rmk::color` namespace:

```cpp
namespace color {

inline constexpr Color red       = { 255, 0,   0,   255 };
inline constexpr Color green     = {   0, 230, 50,  255 };
inline constexpr Color blue      = {   0, 0,   255, 255 };
inline constexpr Color skyblue   = {   0, 191, 255, 255 };
inline constexpr Color purple    = { 138, 43,  226, 255 };
inline constexpr Color orange    = { 249, 115,  22, 255 };
inline constexpr Color yellow    = { 255, 215, 0,   255 };
inline constexpr Color gray      = { 128, 128, 128, 255 };
inline constexpr Color gold      = { 255, 203,   0, 255 };
inline constexpr Color darkblue  = {   0, 80,  180, 255 };
inline constexpr Color darkgreen = {   0, 120,  50, 255 };
inline constexpr Color lime      = {  34, 182, 212, 255 };
inline constexpr Color silver    = { 192, 192, 192, 255 };
inline constexpr Color maroon    = { 128, 0,   0,   255 };
inline constexpr Color pink      = { 255, 0,   128, 255 };
inline constexpr Color fuchsia   = { 255, 0,   255, 255 };
inline constexpr Color aqua      = {   0, 255, 255, 255 };
inline constexpr Color raywhite  = { 245, 245, 245, 255 };
inline constexpr Color cyan      = {   6, 182, 212, 255 };
inline constexpr Color emerald   = {   5, 150, 105, 255 };
inline constexpr Color teal      = {  13, 148, 136, 255 };
inline constexpr Color amber     = { 217, 119,   6, 255 };
inline constexpr Color indigo    = {  79,  70, 229, 255 };
inline constexpr Color black     = {   0, 0,   0,   255 };
inline constexpr Color white     = { 255, 255, 255, 255 };

} // namespace color
```

---

## Arithmetic

It is possible to perform color combinations using the operators:
`+`, `-`, `%`, `+=`, `-=`, `%=`.

### Addition & Subtraction

Adding two colors together using the `+` operator picks the largest RGBA values
from both. For example:

```cpp
// red  = {255, 0, 0, 255} and blue = {0, 0, 255, 0}
// gives purple = {255, 0, 255, 255}
rmk::Color purple = rmk::color::red + rmk::color::blue;
```

And the `-` operator does the opposite.

!!! info
    Since these operations are merely selecting the smallest/largest RGBA value,
    the blend is irreversible unless the colors involved were pure (only one
    value equal to 255, the rest equal to 0 except for *alpha*) as in our
    example.

### Modulo

The `%` operator computes the average of each RGBA value, resulting in a more
precise blend that is strictly irreversible in a trivial way. Example:

```cpp
// red  = {255, 0, 0, 255} and blue = {0, 0, 255, 0}
// gives purple = {128, 0, 128, 255}
rmk::Color purple = rmk::color::red % rmk::color::blue;
```

---

## HSL

Selecting colors by RGB components works fine for fixed palettes, but becomes awkward for anything that needs a smooth gradient 
or a randomly generated hue; `HSL` represents a color as **Hue**, **Saturation** and **Luminosity** instead, which is far more 
natural for that kind of work.

The `HSL` structure has **3 members**, all of type `f32`:

- `f32 h` : hue, in degrees (0-360)
- `f32 s` : saturation (0.0-1.0)
- `f32 l` : luminosity (0.0-1.0)

Its constructor is as follows:

```cpp
constexpr HSL(void);
constexpr HSL(f32 H, f32 S, f32 L);
```

### Converting between RGB and HSL

Conversion between the two representations goes through the `color` namespace, contained in the header **"remake2d/utility.hpp"**:

```cpp
namespace color {
HSL RGBToHSL(const Color&);
Color HSLToRGB(const HSL&);
}
```

```cpp
rmk::HSL hsl     = rmk::color::RGBToHSL(rmk::color::red);
rmk::Color result = rmk::color::HSLToRGB(hsl);
```

---

[:octicons-arrow-left-24: Previous Chapter](area.md){ .md-button }
[Next Chapter :octicons-arrow-right-24:](../graphics/window.md){ .md-button .md-button--primary }