# Window

**Windows** are essential objects for game and application development, serving as the primary support for graphical rendering.

---

## Overview

Windows are represented by the `Window` type, available via the **"remake2d/window.hpp"** header. Their constructors are as follows:

```cpp
Window(void);
Window(std::string_view title, Vec2d pos, Dim2d size);
```

- `title` : window title
- `pos`   : window position from the top-left corner of the screen
- `size`  : window dimensions

!!! info
    All parameters are optional and will be initialized with default values.
    Windows are automatically bound to the `onWindowClosed`, `onWindowResized`
    and `onWindowMoved` events, in order to keep their internal data up to date.

---

## Methods

The public methods of `Window` are as follows:

```cpp
u32      ID(void) const noexcept;           // get the unique window identifier
Vec2d    pos(void) const noexcept;          // get window position (upper-left corner)
Dim2d    size(void) const noexcept;         // get window dimensions
Vec2d    center(void) const noexcept;       // get window center coordinates
Area     area(void) const noexcept;         // get window area
void     move(Vec2d) noexcept;              // move window to a new position
void     resize(Dim2d) noexcept;            // resize window
void     maxSize(Dim2d) noexcept;           // set maximum window size
void     rename(std::string_view) noexcept; // change window title
void     icon(std::string_view);            // set window icon from an image file
void     resizable(bool) noexcept;          // allow or forbid user resizing
void     fullScreen(bool) noexcept;         // toggle fullscreen mode
void     border(bool) noexcept;             // show or hide window decorations
void     close(void) noexcept;              // close and destroy the window
bool     isOpen(void) const noexcept;       // check if window is still open
bool     isFocus(void) const noexcept;      // check if window has input focus
std::string title(void) noexcept;           // get current window title
void     blendMode(window::blendmode) noexcept; // set window blend mode

void     present(void);                      // present current frame
void     screenshot(std::string_view path);  // take a screenshot of current frame
void     clear(Color = rmk::color::black) noexcept; // clear window with a solid color

void     draw(const Drawable&, i16 layer = 0) noexcept; // draw a drawable object
void     fill(const Fillable&, i16 layer = 0) noexcept; // draw filled a fillable object

void     connectViewport(Viewport&)    noexcept;  // register viewport
void     disconnectViewport(Viewport&) noexcept;  // unregister viewport
```

!!! info
    Some methods such as `draw` or `blendMode` are covered in more detail in other
    sections of the documentation.

---

## Usage

```cpp
#include <remake2d/window.hpp>

int main(void) {
    rmk::Window win("window", 0, {1200, 600});

    rmk::Vec2d  center = win.center();
    std::string title  = win.title();

    win.resizable(true);
    win.resize({800, 400});
    win.move({50, 50});
}
```

---

## Constants

The **"remake2d/window.hpp"** header also exposes predefined constants for
creating windows with standard dimensions and positions:

```cpp
namespace window {

namespace pos {
inline constexpr Vec2d centered  = { -1 };
inline constexpr Vec2d undefined = { -2 };
}

namespace size {
inline constexpr Dim2d svga   = { 800,  600  };
inline constexpr Dim2d vga    = { 640,  480  };
inline constexpr Dim2d hd     = { 1280, 720  };
inline constexpr Dim2d fullhd = { 1920, 1080 };
inline constexpr Dim2d qhd    = { 2560, 1440 };
inline constexpr Dim2d uhd    = { 3840, 2160 };
}

}
```

!!! info
    By default, windows are constructed with the title `"RE:MAKE 2D"`,
    the position `window::pos::undefined` and the size `window::size::hd`,
    if these values are not explicitly provided.

---

## Property

| Type | Copiable | Movable | Traits |
|---|---|---|---|
| Window | No | Yes | Trackable |

---

[:octicons-arrow-left-24: Previous chapter](../types/color.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](loop.md){ .md-button .md-button--primary }