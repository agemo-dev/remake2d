# Parallax

A flat background reads as flat right away; scrolling several layers at different speeds is enough to fake depth,
without modeling anything in 3D.

---

## Overview

`Parallax` is contained in the header **"remake2d/parallax.hpp"**. It's built from a list of `Sprite`s and a list 
of speed quotients, one per layer, expressed as a percentage speed reduction relative to the previous layer.

---

## Methods

```cpp
void move(const Vec2d&)     noexcept; // move background
void resize(const Dim2d&)   noexcept; // resize background
void velocity(const Vec2d&) noexcept; // set scroll speed

Dim2d size(void)     const noexcept; // get size
Vec2d center(void)   const noexcept; // get center
Vec2d velocity(void) const noexcept; // get scroll speed
```

---

## Usage

### Creating a parallax

The constructor takes a center, a size, a span of sprites, and a span of speed quotients:

```cpp
Parallax(const Vec2d&, const Dim2d&, const SpriteList&, const QuotientList&);
// SpriteList   = std::span<Sprite>
// QuotientList = std::span<u8>
```

- `sprites`   : the layers to tile, ordered from furthest to closest.
- `quotients` : per-layer speed reduction percentages (0-100).

```cpp
rmk::Sprite sky("sky.png", {{400, 300}, {800, 600}});
rmk::Sprite hills("hills.png", {{400, 300}, {800, 600}});
rmk::Sprite ground("ground.png", {{400, 300}, {800, 600}});

rmk::Sprite sprites[] = { sky, hills, ground };
rmk::u8     quotients[] = { 80, 40, 0 };

rmk::Parallax bg({400, 300}, {800, 600}, sprites, quotients);
```

!!! info
    If more quotients than sprites are provided, they're resampled evenly across the available layers.

### Drawing a parallax

```cpp
// In render loop
win.draw(bg, rmk::color::white);
```

```cpp
#include <remake2d/window.hpp>
#include <remake2d/loop.hpp>
#include <remake2d/parallax.hpp>

int main(void) {
    rmk::Window win;
    rmk::Sprite sky("sky.png", {{400, 300}, {800, 600}});
    rmk::Sprite sprites[] = { sky };
    rmk::u8     quotients[] = { 50 };
    rmk::Parallax bg({400, 300}, {800, 600}, sprites, quotients);

    bg.velocity({-100, 0}); // scroll left

    rmk::loop.execute(win, [&](void) {
        win.draw(bg, rmk::color::white);
    });

    rmk::loop.update();
}
```

### Scrolling

Velocity sets a constant scroll speed applied every frame, scaled by each layer's speed factor:

```cpp
bg.velocity({-150, 0});
```

Each layer tiles itself automatically once it scrolls off-screen, giving an endless scrolling effect 
without ever having to reposition anything manually.

---

[:octicons-arrow-left-24: Previous chapter](tilemap.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](../time/delta.md){ .md-button .md-button--primary }