# Utility

A game engine quickly accumulates small helpers rewritten over and over across projects: converting degrees to radians, finding a
file among several paths, computing a relative layer. **RE:MAKE 2D** gathers them once and for all.

---

## Overview

These helpers live in the header **"remake2d/utility.hpp"**, split across several namespaces: `angle`, `physic`, `layer`, `file` 
and `color`. They're mostly free functions, with no state attached.

---

## Nil

Representing "no particular value" for a generic unknown type is tricky; that's exactly the role of `Nil`, exposed as the constant 
`nil`, which implicitly converts to any type by default-constructing it, and compares to anything via `==`/`!=`:

```cpp
inline constexpr Nil nil;
```

```cpp
rmk::Vec2d v = rmk::nil;      // { 0, 0 }
if (v == rmk::nil) { /* ... */ }
```

!!! info
    `nil` is mostly used internally as a default return value, but can be used anywhere a "zeroed" value of an unknown type is needed.

---

## Angle

Shapes and physics bodies work in radians, but thinking in degrees stays more natural; these helpers convert both ways, with literals included:

```cpp
fmax degToRad(fmax degrees) noexcept;
fmax radToDeg(fmax radians) noexcept;

namespace literal {
fmax operator""_deg(fmax) noexcept; // degrees to radians literal
fmax operator""_rad(fmax) noexcept; // radians to degrees literal
}
```

```cpp
using namespace rmk::angle::literal;

f32 angle = 90.0_deg; // converts 90 degrees to radians
shape.rotate(angle);
```

---

## Physic units

**Box2D** thinks in meters, while the rest of the engine works in pixels; these helpers bridge the two, based on the ratio defined in 
`PhysicManager::pixelsPerMeter`:

```cpp
f32   pixelToMeter(f32) noexcept;
f32   meterToPixel(f32) noexcept;
Vec2d pixelToMeter(const Vec2d&) noexcept;
Vec2d meterToPixel(const Vec2d&) noexcept;
Dim2d pixelToMeter(const Dim2d&) noexcept;
Dim2d meterToPixel(const Dim2d&) noexcept;
```

```cpp
f32 meters = rmk::physic::pixelToMeter(64.0f);
f32 pixels = rmk::physic::meterToPixel(2.0f);
```

---

## Layer

Ordering a scene's background, characters, and HUD correctly gets tedious with arbitrary numbers; the `layer` namespace gives named
offsets computed from a base "wall" value:

```cpp
enum class layer : i16 {
    min   = -50, // minumum layer
    max   = 249, // maximun layer

    size  = 50, // distance between two layers

    ground = min,           // ground layer start
    world  = ground + size, // world layer start
    sky    = world  + size, // sky layer start
    ui     = sky    + size, // ui layer start
    log    = ui     + size, // log layer start

    count = log + size * 2 - 1 // total layers count
};

// Layer helpers (returns i16 layer value)
namespace level {
    i16 ground(u8 wall) noexcept; // wall + layer::ground  (background elements)
    i16 world(u8 wall)  noexcept; // wall + layer::world   (game world objects)
    i16 sky(u8 wall)    noexcept; // wall + layer::sky     (foreground effects)
    i16 ui(u8 wall)     noexcept; // wall + layer::ui      (user interface)
    i16 log(u8 wall)    noexcept; // wall + layer::log     (debug overlay)
}
```

```cpp
scene.add(background,  rmk::layer::ground(10)); // -10
scene.add(player,      rmk::layer::world(0));   // 0
scene.add(hud,         rmk::layer::ui(200));    // 199 : value is automatically clamped to 49 if is overlimit
```

!!! info
    `min` and `max` define the valid range of layer values accepted by `Scene::add`.

---

## File

Manually checking whether an asset exists at one path or another gets repetitive once a project supports multiple platforms or
fallback locations:

```cpp
namespace file {
std::string jump(std::span<std::string_view> paths) noexcept;
}
```

```cpp
std::string_view paths [] = {
    "assets/player.png",
    "fallback/player.png"
};

std::string path = rmk::file::jump(paths);
```

!!! info
    If no file is found among the given paths, `jump` returns `nil`.

---

## Color conversion

Interpolating two colors visually doesn't work well in raw RGB; going through HSL gives a far more natural result, which is
exactly what `Random::rand<Color>` and `Random::mixColor` do under the hood through these conversions:

```cpp
namespace color {
HSL RGBToHSL(const Color&);
Color HSLToRGB(const HSL&);
}
```

```cpp
rmk::HSL hsl     = rmk::color::RGBToHSL(rmk::color::red);
rmk::Color color = rmk::color::HSLToRGB(hsl);
```

---

## Version

This header also contains enumeration and a function to find out the current version of the engine being used:

```cpp
// version enum

enum class version : u8 {
	major = 0,
	minor = 1,
	patch = 0
};

// Helper function
std::string currentVersion(void) noexcept; // full current version (x.y.z)
```

---

## Constants

A few generic constants are also exposed, mostly to make certain return values more readable than a plain `0`/`1`:

```cpp
inline constexpr u8 fail    = 1;
inline constexpr u8 success = 0;
inline constexpr bool on  = true;
inline constexpr bool off = false;
inline constexpr bool isTrue  = true;
inline constexpr bool isFalse = false;
```

```cpp
u8 result = doSomething();
if (result == rmk::success) { /* ... */ }
```

---

[:octicons-arrow-left-24: Previous chapter](concept.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](random.md){ .md-button .md-button--primary }