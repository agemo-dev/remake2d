# Camera

Drawing a world larger than the screen means deciding what part of it to show; `Camera` tracks a position, applies smoothing and world
limits, and reports how far the view has shifted since last frame.

---

## Overview

`Camera` is contained in the header **"remake2d/camera.hpp"**. It doesn't draw anything on its own. It holds a position, a size, and
optional limits, and computes an offset that a **viewport** uses to shift what gets rendered through it.

```cpp
Camera(void);
Camera(const Vec2d& center, const Dim2d& size, const Dim2d& limit = nil);
```

- `center` : the camera's starting position.
- `size`   : the camera's size, matching the screen region it will render into.
- `limit`  : the boundaries of the world the camera is allowed to scroll within.

---

## Methods

```cpp
void  move(const Vec2d&)   noexcept;   // set the camera's center directly
void  resize(const Dim2d&) noexcept;   // change the camera's size
void  limit(const Dim2d&)  noexcept;   // change the world boundaries
void  zoom(f32)            noexcept;   // set the zoom factor
void  smoothing(f32)       noexcept;   // set how smoothly the camera catches up to what it follows

Vec2d center(void)        const noexcept; // current center position
Dim2d size(void)          const noexcept; // current size
Dim2d limit(void)         const noexcept; // current world boundaries
Vec2d offset(void)        const noexcept; // shift since last frame
f32   zoom(void)          const noexcept; // current zoom factor
f32   smoothing(void)     const noexcept; // current smoothing factor
Vec2d followedPoint(void) const noexcept; // position the camera is centered on, or nil

void follow(const Followable&) noexcept; // start tracking an object
void unfollow(void)            noexcept; // stop tracking, hold position
```

---

## Usage

### Moving a camera directly

Without `follow`, a camera stays wherever it's placed until moved by hand:

```cpp
rmk::Camera cam(0, {800, 600}, {2000, 1200});

cam.move({500, 300});
```

### Zoom and smoothing

`zoom` scales what the camera sees; a value above 1 zooms in, below 1 zooms out. `smoothing` controls how quickly the camera catches up to
its target instead of snapping to it instantly: `0` (the default) means no smoothing, higher values ease in more gradually.

```cpp
cam.zoom(1.5f);
cam.smoothing(0.1f);
```

### Following

`follow` only accepts a type that derives from `Followable`:

```cpp
class Followable : public Trackable<Followable> {
public:
	virtual Vec2d center(void) const noexcept = 0;
};
```

A `Followable` is any object that can report its own center, and that can be tracked safely even if it moves in memory or gets destroyed
later (that's what `Trackable` provides.

`Geometry` (and therefore every shape built on top of it, like `Rectangle` or `Circle`) already implements `Followable`. It's a convenient
default when a camera just needs to follow a position without any custom behavior attached to it:

```cpp
rmk::Rectangle playerShape(100, 32);

cam.follow(playerShape);
```

Nothing else in the engine implements `Followable` by default, including `PhysicBody`. That's intentional: rather than picking one built-in
type to special-case, `Camera` leaves it to the person to define what "the thing being followed" means for their own game, the same way
`ActorBase` is meant to be inherited from rather than used directly. A player character, for instance, can implement `Followable` itself and
decide exactly what its center should be:

```cpp
class Player : public rmk::DynamicActor, public rmk::Followable {
public:
    rmk::Vec2d center(void) const noexcept override { return body.center(); }
};

Player player;
cam.follow(player);
```

Calling `follow` again with a different object simply replaces what the camera was tracking. To stop tracking anything and hold the camera
in place:

```cpp
cam.unfollow();
```

`followedPoint` reports the position the camera is currently centered on. Once the followed object has been destroyed, it returns `nil`
instead of the last known position:

```cpp
Vec2d whereTheCameraIsLooking = cam.followedPoint();
```

### World limits

The `limit` passed to the constructor (or set later) keeps the camera from scrolling past the edges of the world, even if the followed
object goes further:

```cpp
cam.limit({2000, 1200}); // camera never shows anything past this boundary
```

used `nil` to delete the limit:

```cpp
cam.limit(rmk::nil);
```
---

[:octicons-arrow-left-24: Previous chapter](viewport.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](../texture/texture.md){ .md-button .md-button--primary }
