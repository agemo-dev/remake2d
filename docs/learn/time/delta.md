# Delta Time

Movement, animation speed, timers — almost everything that changes over time in a game needs to know how long the last frame 
actually took, since frame duration is never perfectly constant.

---

## Overview

`DeltaTime` exposes a singleton `delta`, contained in the header **"remake2d/time.hpp"**. It tracks how long the previous frame 
took, and caps the frame rate to a configurable maximum.

---

## Methods

```cpp
fmax tick(void)   const noexcept; // delta time of the current frame, in seconds
fmax FPS(void)    const noexcept; // current frames per second
fmax maxFPS(void) const noexcept; // current frame rate cap
void maxFPS(fmax) noexcept;       // set frame rate cap (clamped 30-1440)
void update(void);                // advance delta and dispatch signals
```

---

## Usage

### Reading delta time

Multiplying a speed by tick is what keeps movement consistent regardless of how fast or slow a frame runs:

```cpp
player.move({speed * (f32)rmk::delta.tick(), 0});
```

FPS is simply the inverse of tick, handy for an on-screen counter:

```cpp
rmk::f32 fps = (rmk::f32)rmk::delta.FPS();
```

### Capping the frame rate

By default the frame rate is capped at 120. `maxFPS` changes that limit, clamped between 30 and 1440:

```cpp
rmk::delta.maxFPS(60.0);
```

### Update

`update` measures the elapsed time since the last call, updates tick and FPS, and dispatches every bound `Signal` in the
engine, including timers and events:

```cpp
// In render loop
rmk::delta.update();
```

!!! info
    Update is already called automatically by `loop`, so it never needs to be called manually unless a custom loop
	is being driven by hand.

---

[:octicons-arrow-left-24: Previous chapter](../maps/parallax.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](timer.md){ .md-button .md-button--primary }
