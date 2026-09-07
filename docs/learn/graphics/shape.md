# Shapes

---

## Overview

**Shapes** are classes that all inherit from the `Geometry` interface.
**Shapes** serve as the base for any **copyable** or **drawable** element on the
screen, so learning them is essential.

---

## Usage

To start, we'll use the `Shape` class, contained in the header **"remake2d/shape.hpp"**,
whose class is **templated** on a number of points, and whose constructor takes **two parameters**:

```cpp
template<size_t POINT_COUNT> requires (POINT_COUNT > (u8)point::min && POINT_COUNT <= (u8)point::max)
Shape(const Vec2d& center, const Dim2d& size);
```

- `POINT_COUNT` : number of sides of the shape.
- `center`      : the shape's center.
- `size`        : the shape's size/diameters.

Unlike `Window`, shapes are positioned directly from their center.
The size represents the shape's **vertical** and **horizontal** **diameter**.
Indeed, shapes can be irregular on X and Y.
`POINT_COUNT` itself determines the **number of sides of the shape**.
Let's declare a shape named triangle with derived type `Triangle`:

```cpp
rmk::Triangle triangle({200, 200}, {50, 50});
```

Numbers limit point are reprenseted by following enumeration:

```cpp
enum class point : u8 {
	min = 0,
	max = 50
};
```

---

## Methods

The `Geometry` class has several **pure virtual methods** that `Shape` inherits from.
Among these, we have:

```cpp
virtual u8 points(void)   			 const noexcept = 0; // number of points
virtual Dim2d size(void)   			 const noexcept = 0; // shape size
virtual Vec2d center(void) 			 const noexcept = 0; // center position
virtual const Vec2d* pointsPos(void) const noexcept = 0; // raw points array

virtual void move(const Vec2d& center) noexcept = 0;     // translate shape
virtual void rotate(f32 angle) noexcept = 0;             // rotate shape (radians)
virtual void scale(const Fact2d& scaling) noexcept = 0;  // scale shape
virtual void resize(const Dim2d& size) noexcept = 0;     // resize shape
virtual void transform(const Vec2d& center, f32 angle, const Fact2d& scaling) noexcept = 0; // translate + rotate + scale

template<IsShape S> S  as(void) const noexcept;                    // cast to another shape type
virtual bool           hasIntersected(const Geometry& other) const noexcept = 0; // collision test
```

---

## Drawing a shape

Now we can display our triangle on screen thanks to the `draw` method of the
`Window` class and the `color` method of `Geometry` :

```cpp
void  Geometry::color(Color color) noexcept; // set color
Color Geometry::color(void)  const noexcept; // get current color

void Window::draw(const Drawable& object, i16 layer = 0) noexcept;
```

- color     : draw color
- object    : drawable object
- layer     : drawing layer

Your question is probably this:
> The `object` parameter is of type `Drawable`, but my figure is of type `Triangle`!
> Furthermore, Geometry does not have any `color` methods!

This is perfectly normal, and the answer is simply that the `geometry` class inherits
from the `Drawable` trait class, which makes it drawable on the screen.
Note that the `color` method actually belongs to the `Drawable` class, but this will
be covered in another lesson.

Let's place ourselves in the game loop and write:

```cpp
triangle.color(rmk::color::red); // set draw color to red

//In render loop
win.draw(triangle);
```

```cpp
#include <remake2d/window.hpp>
#include <remake2d/shape.hpp>
#include <remake2d/loop.hpp>

int main(void) {
    rmk::Window win;
    rmk::Triangle triangle({200, 200}, {50, 50});
    triangle.color(rmk::color::red);

    rmk::loop.execute(win, [&](void) {
        win.draw(triangle);
    });

    rmk::loop.update();
}
```

![drew triangle](assets/graphics1.png)

It is also possible to **fill** the shape with the `fill` method, similar to `draw`:

```cpp
void fill(const Fillable& shape, i16 layer = 0) noexcept;
```

Once again, the `Geometry` class also derives from the `Fillable` trait class!

```cpp
win.fill(triangle);
```

![filled triangle](assets/graphics2.png)

### Transparency

It is possible to make shapes somewhat transparent. By reducing the *alpha* value of `Color`,
we also reduce **the opacity** of the shape drawn on screen.

example:

without transparency:

```cpp
triangle.color(rmk::color::red);
win.clear(rmk::color::green);
win.fill(triangle);
```

![opaque triangle](assets/graphics3.png)

with transparency:
```cpp
triangle.color({255, 0, 0, 128}); // semi-transparent red triangle
win.clear(rmk::color::green);
win.fill(triangle);
```

![semi-transparent triangle](assets/graphics4.png)

You can change the transparency mode via the `blendMode` method of the `Window` class, which can take
three distinct values:

```cpp
namespace window {

enum class blendmode : u8 {
	none,
	normal,
	add,
	mod,
	mul
};

}
```

- `none`   : disable transparency on the window
- `normal` : default transparency mode
- `add`    : additive mode with the background color
- `mod`    : modulate mode with the background color
- `mul`    : multiply mode with the background color

example:

```cpp
triangle.color({255, 0, 0, 128});
win.blendMode(rmk::window::blendmode::add);

win.clear(rmk::color::green);
win.fill(triangle);
```

![additive semi-transparent triangle](assets/graphics5.png)

---

## Shape types

**RE:MAKE 2D** offers several type aliases and specialized types for shapes:

```cpp
// Alias
using Line      = Shape<2>;   // line segment
using Losange   = Shape<4>;   // diamond shape
using Hexagone  = Shape<6>;   // hexagon
using Ellipse   = Shape<36>;  // ellipse approximation

// Derived types
class Point     : public Shape<1>;  // single point
class Triangle  : public Shape<3>;  // triangle
class Rectangle : public Shape<4>;  // axis-aligned rectangle (optimized build)
class Square    : public Rectangle; // uniform rectangle
class Circle    : public Ellipse;   // circle (w == h enforced)
```

!!! info
    For the `Square` and `Circle` types, only the `resize` method is overridden to take just the width into account.
    The constructors of the `Square` and `Circle` types take an `f32` rather than a `Dim2d` in their constructor.

---

## Property

| Type | Copiable | Movable | Trait |
|---|---|---|---|
| Geometry | Yes | Yes | Followable, Drawable and Fillable |
| Shape | Yes | Yes | None |
| Shape derived | Yes | Yes | None |

---

[:octicons-arrow-left-24: Previous chapter](loop.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](viewport.md){ .md-button .md-button--primary }