# Texture

Drawing a shape's outline only goes so far; sooner or later a game needs actual images on screen, whether it's a sprite, a background, or a UI element.

---

## Overview

Textures are represented by `Texture<S>`, contained in the header **"remake2d/texture.hpp"**. It loads an image from disk and maps it onto a `Geometry`,
most commonly a `Rectangle` through the alias `Sprite`. `TextureBase` is the abstract interface shared by every texture type, including `Text` and `Animation`.

---

## Methods

```cpp
void move(const Vec2d&)     noexcept; // translate the texture
void rotate(f32)            noexcept; // rotate the texture (radians)
void scale(const Fact2d&)   noexcept; // scale the texture
void resize(const Dim2d&)   noexcept; // resize the texture
void transform(const Vec2d&, f32, const Fact2d&) noexcept; // move + rotate + scale

void clip(const Vec2d&, const Dim2d&) noexcept; // restrict drawing to a sub-region of the image
void unclip(void)                     noexcept; // remove that restriction

Vec2d center(void)   const noexcept; // get center position
Dim2d size(void)     const noexcept; // get current size
Dim2d realSize(void) const noexcept; // get original image size

bool hasIntersected(const Geometry&)    const noexcept; // collision test
bool hasIntersected(const TextureBase&) const noexcept; // collision test
```

---

## Usage

### Loading a sprite

The most common texture is a `Sprite`, a `Texture<Rectangle>` alias, taking a path and a `Rectangle` describing its position and size:

```cpp
Sprite(std::string_view path, const Rectangle&);
```

```cpp
rmk::Sprite player("player.png", {{400, 300}, {64, 64}});
```

!!! info
    **RE:MAKE 2D** supports PNG, JPG, WEBP, TGA and BMP image formats .

### Drawing a texture

```cpp
// In render loop
win.draw(player, rmk::color::white);
```

```cpp
#include <remake2d/window.hpp>
#include <remake2d/loop.hpp>
#include <remake2d/texture.hpp>

int main(void) {
    rmk::Window win;
    rmk::Sprite player("player.png", {win.center(), {64, 64}});

    rmk::loop.execute(win, [&](void) {
        win.draw(player, rmk::color::white);
    });

    rmk::loop.update();
}
```

### Tinting and transparency

A texture has no color of its own; tint and opacity are entirely driven by the `Color` passed to draw, since the underlying image is generated 
white so color modulation applies correctly:

```cpp
win.draw(player, rmk::color::red);         // tinted red
win.draw(player, {255, 255, 255, 128});      // half transparent
```

### Clipping a sprite sheet

Clip restricts drawing to a sub-rectangle of the source image, useful for picking a single frame out of a larger sheet without loading separate files:

```cpp
player.clip({64, 0}, {64, 64}); // second frame of a 64x64 sheet
```

`unclip` removes that restriction, going back to drawing the full image.

### Moving and transforming

Move, rotate, scale and resize all behave the same way as on a `Geometry`, and keep the texture's underlying shape and vertices in sync automatically:

```cpp
player.move({450, 300});
player.rotate(0.2f);
player.resize({96, 96});
```

### Collision

hasIntersected checks overlap against a `Geometry` or another `TextureBase`, reusing the same separating-axis test as shapes:

```cpp
if (player.hasIntersected(wallSprite)) { /* blocked */ }
```

---

[:octicons-arrow-left-24: Previous chapter](../graphics/viewport.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](text.md){ .md-button .md-button--primary }
