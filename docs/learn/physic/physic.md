# Advanced Physic

Dropping a single body into the default world works without any tuning, but bordering a level, adjusting how gravity feels, 
or iterating every body of a given kind calls for `PhysicManager`.

---

## Overview

`PhysicManager` is contained in the header **"remake2d/physic.hpp"**, exposed through the global singleton `physics`. It holds 
the physics simulation itself, and every `StaticBody`/`DynamicBody` created anywhere in the program is automatically registered into it.

---

## Methods

```cpp
void remove(PhysicBody&);                        // remove a body from the simulation
void world(const Area&)          noexcept;        // set world boundaries (creates boundary walls)
void gravitationalConstant(f32)  noexcept;        // set gravity
void pixelsPerMeter(f32)         noexcept;        // set pixel-to-meter ratio

Area world(void)                 const noexcept; // get world boundaries
f32  pixelsPerMeter(void)        const noexcept; // get pixel-to-meter ratio
f32  gravitationalConstant(void) const noexcept; // get gravity

const std::vector<PhysicBody*>&  bodies(void)   const noexcept; // all physics bodies
const std::vector<StaticBody*>&  statics(void)  const noexcept; // static bodies only
const std::vector<DynamicBody*>& dynamics(void) const noexcept; // dynamic bodies only

void update(void); // step the simulation (called by loop)
```

---

## Usage

### Gravity

Gravity applies to every `DynamicBody` with gravity enabled, expressed in meters per second squared:

```cpp
rmk::physics.gravitationalConstant(9.8f);
```

### Pixels per meter

The simulation thinks in meters, while every `Vec2d`/`Dim2d` passed to a shape or a body is expressed in pixels; pixelsPerMeter 
is the ratio that bridges the two, and also determines how strong gravity and forces feel:

```cpp
rmk::physics.pixelsPerMeter(32.0f); // 32 pixels = 1 meter
```

### World boundaries

Without a defined limit, a body can fall off-screen indefinitely; world bounds the simulation to a rectangular area, creating four 
static walls around the given `Area`:

```cpp
rmk::physics.world({0, 0, 2000, 1200});
```

```cpp
rmk::Area bounds = rmk::physics.world();
```

### Accessing bodies

Grabbing every enemy for a global effect, or iterating only the static bodies in a level, works through bodies, or filtered by 
kind through statics/dynamics:

```cpp
for (auto* body : rmk::physics.dynamics()) {
    if (body->tag() == "enemy") body->velocity({0, 0});
}
```

### Removing a body

Remove unregisters a body from the simulation and destroys its underlying physical representation; the `PhysicBody` object 
itself is not destroyed:

```cpp
rmk::physics.remove(rock);
```

### Update

Update steps the simulation forward by one frame, dispatches contact signals on every affected body, and syncs each body's 
cached position back from the simulation:

```cpp
// In render loop
rmk::physics.update();
```

!!! info
    Update is already called automatically by loop every frame.

---

[:octicons-arrow-left-24: Previous chapter](body.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](../maps/tilegrid.md){ .md-button .md-button--primary }