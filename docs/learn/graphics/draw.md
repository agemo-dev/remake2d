# Printable

Graphical rendering is essential for any application, so we need a way to display our objects on screen.

---

## Overview

In this section, we'll finally learn how to draw our own custom types on screen!
To do that, we'll use a dedicated engine trait: **Printable**, contained in the header **"remake2d/window.hpp"**.

!!! info
    You've already run into it without knowing: the `Geometry` class (and therefore every shape)
    inherits from `Printable`, which is why you can pass them straight to `win.draw` and `win.fill`.

---

## Methods

The `Printable` class has **two virtual methods** and **five public members**:

```cpp
mutable bool filled{false}; // check if filled
mutable bool drawn{false};  // check if drawn

mutable bool overridden{false}; // check if inherit from parent attributs

mutable bool is_fill_dirty{true};
mutable bool is_draw_dirty{true};

void  color(Color) noexcept;       // set print color
void  layer(i16)   noexcept;       // set print layer
Color color(void)  const noexcept; // get print color
i16   layer(void)  const noexcept; // get print layer

void from(const Printable& main) const noexcept;
virtual void draw(const Printable& main) const noexcept {};
virtual void fill(const Printable& main) const noexcept {};
```

---

## Usage

### Inheritance

To make a type displayable on screen, all you have to do is make your class inherit from `Printable`:

```cpp
class Player : public rmk::Printable {
    rmk::DynamicBody m_body;
};
```

### Implementation

To draw your object, just implement one of the two virtual methods (or both):

- `draw`: draws the **outline**.
- `fill`: fills the shape with a background **color**.

The implementation must always follow the same pattern, otherwise you risk bugs or a malfunctioning drawing system:

```cpp
void draw(const Printable& main) const noexcept {
    from(main);
    m_body.draw(main);
}

void fill(const Printable& main) const noexcept {
    from(main);
    m_body.fill(main);
}
```

You can of course display as many elements as you like in your methods:

```cpp
void draw(const Printable& main) const noexcept {
    from(main);
    m_body1.draw(main);
    m_body2.draw(main);
    m_body3.draw(main);
}
```

The `from` method is essential: it's what propagates the data from `main` to its components.

!!! warning
    You **must** pass the parameter of the `draw`/`fill` methods on to every element
    you draw, otherwise the system will malfunction.

### Color

By default, the object is displayed in white. To change that, we use the `color` method:

```cpp
Player p;
p.color(rmk::color::red);
```

### Layer

You can change a printable's layer with the `layer` method, whose value starts at 0:

```cpp
p.layer(4);
```

Objects are displayed in ascending layer order: an object on layer 1 will be drawn behind an object on layer 2, and so on.

That said, **RE:MAKE 2D** enforces limits on the number of layers you can use. They are stored in the `layer` enumeration,
found in the **"remake2d/utility.hpp"** and **"remake2d/window.hpp"** headers:

```cpp
enum class layer : i16 {
    min   = -50, // minimum layer
    max   = 249, // maximum layer

    size  = 50, // distance between two layers group

    ground = min,
    world  = ground + size,
    sky    = world  + size,
    ui     = sky    + size,
    log    = ui     + size,

    count = log + size * 2 - 1 // total layers count
};
```

As you can see, there are also **layer groups**, spaced `layer::size` apart:

```cpp
p.layer((i16) layer::world);
```

There are also helpers that make it easier to split each group into subgroups:

```cpp
namespace level {
i16 ground(u8 layer) noexcept;
i16 world(u8 layer)  noexcept;
i16 sky(u8 layer)    noexcept;
i16 ui(u8 layer)     noexcept;
i16 log(u8 layer)    noexcept;
} // namespace level
```

```cpp
p.layer(rmk::level::world(2));
```

!!! info
    The `layer` parameter of these helpers is clamped to `layer::size` if it is greater.

### Public members

The `Printable` class also exposes a few public members:

- `filled`     : set to `true` by the engine the first time the object is displayed through `fill`.
- `drawn`      : set to `true` by the engine the first time the object is displayed through `draw`.
- `overridden` : set manually to `true` if you want set a custom color or layer for a children .

There are also `is_draw_dirty` and `is_fill_dirty`, which tell the engine that the object has been modified
and that its drawing cache needs to be refreshed.
They're meant to be set to `true` manually by the user, but the engine already detects
automatically when one of the object's components has changed =).

---

## Property

| Type | Copiable | Movable | Bases and Traits |
|---|---|---|---|
| Printable | Yes | Yes | None |

---

[:octicons-arrow-left-24: Previous chapter](viewport.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](camera.md){ .md-button .md-button--primary }