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
    Creating a window also initializes the **system** automatically, which is why
    a window must always be the first object created.

---

## Methods

The public methods of `Window` are as follows:

```cpp
Window(std::string_view title, Vec2d pos, Dim2d size); // create a new window
u32      ID(void) const noexcept;           // get the unique window identifier
Vec2d    pos(void) const noexcept;          // get window position (upper-left corner)
Dim2d    size(void) const noexcept;         // get window dimensions
Vec2d    center(void) const noexcept;       // get window center coordinates
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
void     blendMode(window::blendmode) noexcept; // set SDL blend mode

void     clear(Color = rmk::color::black, std::string_view viewport = "") noexcept; // clear window with a solid color

void     draw(const TextureBase&, Color = rmk::color::white, std::string_view viewport = "") noexcept; // draw a texture
void     draw(const TileMap&,     Color = rmk::color::white, std::string_view viewport = "") noexcept; // draw a tile map
void     draw(const Parallax&,    Color = rmk::color::white, std::string_view viewport = "") noexcept; // draw a parallax background
void     draw(const Area&,        Color = rmk::color::white, std::string_view viewport = "") noexcept; // draw rectangle outline
void     draw(const TileGrid&,    Color = rmk::color::white, std::string_view viewport = "") noexcept; // draw tile grid outline
void     draw(const Geometry&,    Color = rmk::color::white, std::string_view viewport = "") noexcept; // draw shape outline
void     draw(const PhysicBody&,  Color = rmk::color::white, std::string_view viewport = "") noexcept; // draw physics body outline

void     fill(const Area&,        Color = rmk::color::white, std::string_view viewport = "") noexcept; // draw filled rectangle
void     fill(const Geometry&,    Color = rmk::color::white, std::string_view viewport = "") noexcept; // draw filled shape
void     fill(const PhysicBody&,  Color = rmk::color::white, std::string_view viewport = "") noexcept; // draw filled physics body

void     addViewport(std::string_view, Viewport) noexcept;  // register a named viewport
void     linkCamera(std::string_view, Camera&) noexcept;    // attach a camera to a viewport
void     unlinkCamera(std::string_view) noexcept;           // detach camera from a viewport
void     removeViewport(std::string_view) noexcept;         // delete a named viewport
void     useViewport(std::string_view) noexcept;            // activate a viewport for subsequent drawing
void     resetViewport(void) noexcept;                      // restore the default full-window viewport
```

!!! info
    Some methods such as `draw`, `blendMode` or `linkCamera` are covered
    in more detail in other sections of the documentation.

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
inline constexpr Vec2d centered  = { SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED  };
inline constexpr Vec2d undefined = { SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED };
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

[:octicons-arrow-left-24: Previous chapter](../types/color.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](loop.md){ .md-button .md-button--primary }