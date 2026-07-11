# Viewport

Viewports are extensions of windows that allow restricting rendering to a specific area of the screen.

---

## Overview

Viewports are represented by the `Window::Viewport` structure. They allow delimiting the rendering zone to a defined portion of a window.

By default, every object is rendered within the **global viewport** of the window, whose origin is the top-left corner:

```txt
(0, 0)  _ _ _ _ _ _ _ _ _ _ _ _ _ (x)
       |
       |
       |
       |
       |
       |
       |
      (y)
```

Viewports allow restricting this area to a defined portion:

```txt
(0, 0)  _ _ _ _ _ _ _ _ _ _ _ _ _ (x)
       |
       |
       |     (0, 0) _ _ _ _ (x')
       |           |
       |           |
       |          (y')
       |
      (y)
```

Their constructor is as follows:

```cpp
Viewport(void);
Viewport(const Area& zone);
Viewport(const Area& zone, Camera& camera);
```

- `zone` — rendering area of the viewport
- `camera` — camera attached to the viewport

In this section, only the constructor taking an `Area` parameter will be covered.
Camera usage will be addressed in the [tilemap](../maps/tilemap.md) section.

---

## Usage

Viewports are created and managed through the following `Window` methods:

```cpp
void addViewport(std::string_view tag, Viewport vp) noexcept; // register a named viewport
void removeViewport(std::string_view tag)           noexcept; // delete a named viewport
void useViewport(std::string_view tag)              noexcept; // activate a viewport for subsequent draws
void resetViewport(void)                            noexcept; // restore the default full-window viewport
```

- `tag` — unique identifier of the viewport

For example:

```cpp
rmk::Window win;
rmk::Rectangle rect({50, 50}, {20, 20});
rmk::Window::Viewport vp({250, 100, 100, 100});

win.addViewport("view", vp);

win.fill(rect, rmk::color::cyan, "view");
```

The `useViewport` method sets a default active viewport, used when no identifier
is passed to the window's rendering methods. `resetViewport` deactivates it
and restores the global viewport.

---

[:octicons-arrow-left-24: Previous chapter](shape.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](../texture/texture.md){ .md-button .md-button--primary }