# Physic Entity

A shape drawn on screen doesn't react to anything by default; attaching a physics body to it is what lets it collide, fall, bounce, or get pushed.

---

## Overview

Physics entities are represented by `PhysicBody`, contained in the header **"remake2d/physic.hpp"**. `PhysicBody` is an abstract base class carrying 
the shared logic, while `StaticBody` and `DynamicBody` implement the two concrete kinds of bodies the engine offers. Every body is built from an existing 
`Geometry`, whose points are used to compute the underlying physical shape.

---

## Methods

The methods shared by every `PhysicBody` are as follows:

```cpp
void               tag(std::string_view) noexcept;         // set identifying tag
const std::string& tag(void) const noexcept;               // get tag
u64                ID(void) const noexcept;                // get unique body ID
void               isSolid(bool) noexcept;                 // set solid or sensor
bool               isSolid(void) const noexcept;            // check if solid
void               link(Sprite&) noexcept;                  // attach a sprite texture
void               move(const Vec2d&)     noexcept;
void               rotate(f32)            noexcept;
void               scale(const Fact2d&)   noexcept;
void               resize(const Dim2d&)   noexcept;
Vec2d              center(void)    const noexcept;
Dim2d              size(void)      const noexcept;

void               linkAnimation(std::pair<std::string, Animation>); // attach an animation
Animation&         animation(std::string_view);            // get animation by name

Signal<PhysicBody*, PhysicBody*> onContact;                 // emitted on contact persist
Signal<PhysicBody*, PhysicBody*> onContactStart;            // emitted on contact begin
Signal<PhysicBody*, PhysicBody*> onContactEnd;               // emitted on contact end
```

`DynamicBody` additionally exposes:

```cpp
void  mass(f32)              noexcept;
void  bounce(f32)            noexcept;
void  bounceThreshold(f32)   noexcept;
void  infiniteBounce(bool)   noexcept;
void  friction(f32)          noexcept;
void  gravity(bool)          noexcept;
void  isBullet(bool)         noexcept;
void  warp(const Area&)      noexcept;
void  limit(const Area&)     noexcept;

void  jump(f32)              noexcept;
void  move(const Vec2d&)     noexcept;
void  velocity(const Vec2d&) noexcept;
Vec2d velocity(void)   const noexcept;

Signal<DynamicBody*> onMove;
Signal<DynamicBody*> onMoveUp;
Signal<DynamicBody*> onMoveDown;
Signal<DynamicBody*> onMoveLeft;
Signal<DynamicBody*> onMoveRight;
```

---

## Usage

### Static vs dynamic

A wall doesn't need to react to anything; that's exactly what `StaticBody` does, never moving and ignoring forces and gravity. `DynamicBody`, 
on the other hand, is affected by gravity, forces, and collisions — the player, a projectile, a falling crate.

Both are built from a `Geometry`:

```cpp
rmk::Rectangle floorShape({400, 550}, {800, 50});
rmk::StaticBody floor(floorShape);

rmk::Rectangle playerShape({400, 100}, {32, 48});
rmk::DynamicBody player(playerShape);
```

### Tagging a body

Knowing what was just touched inside a contact callback without keeping a separate reference comes down to a tag:

```cpp
player.tag("player");
floor.tag("floor");
```

### Solid vs sensor

By default, a body physically blocks others. Making it non-solid turns it into a sensor: it still reports contacts through `onContact*`, 
but lets everything pass through it, useful for a trigger or detection zone:

```cpp
rmk::Rectangle zoneShape({600, 300}, {100, 100});
rmk::StaticBody trigger(zoneShape);

trigger.isSolid(false);
trigger.tag("checkpoint");
```

### Linking a texture

Link attaches a `Sprite` to a body, so its position and rotation automatically follow the physics simulation, with no need to sync them by hand:

```cpp
rmk::Sprite playerSprite("player.png", {{400, 100}, {32, 48}});
player.link(playerSprite);
```

### Contacts

`onContact` fires every frame two bodies are still overlapping, while `onContactStart`/`onContactEnd` fire once, on the frame contact begins or ends:

```cpp
player.onContactStart.join([](rmk::PhysicBody* self, rmk::PhysicBody* other) {
    if (other->tag() == "checkpoint") std::cout << "checkpoint reached\n";
});
```

### Mass, friction and bounce

These only apply to `DynamicBody`, since a `StaticBody` never moves:

```cpp
player.mass(10.0f);       // kg
player.friction(0.3f);    // 0.0 - 1.0
player.bounce(0.5f);      // restitution, 0.0 - 1.0
```

For a ball that should always bounce at full energy, infiniteBounce ignores the usual impact-speed threshold:

```cpp
ball.infiniteBounce(true);
```

### Gravity

A floating platform or a top-down game simply doesn't need gravity; it can be turned off per body:

```cpp
player.gravity(false);
```

### Warp and limit

Warp makes a body reappear on the opposite side of a zone once it exits it, for a wraparound screen effect. Limit does the opposite: it keeps 
the body from leaving a zone, stopping it right at the edges:

```cpp
player.warp({0, 0, 800, 600});   // wraps around screen edges
enemy.limit({0, 0, 2000, 1200}); // stays confined to the level bounds
```

### Jump and velocity

Jump applies an instant upward impulse, while velocity reads or sets the body's current speed directly:

```cpp
player.jump(400.0f);

if (rmk::event.onPressSpace.isActive()) player.jump(400.0f);

rmk::Vec2d v = player.velocity();
player.velocity({v.x, 0}); // cancel vertical speed
```

### Movement signals

Triggering a walk animation based on direction doesn't require checking velocity manually every frame; `onMove` fires 
as soon as the body is moving, and `onMoveUp`/`onMoveDown`/`onMoveLeft`/`onMoveRight` fire based on the exact direction:

```cpp
player.onMoveLeft.join([&](rmk::DynamicBody*) {
    playerAnim.play(-1, 8); // walk-left animation
});
```

### Attaching an animation

Animations don't go through the sprite attached via link; they attach directly to the body, and are retrieved by name, which 
makes it easy to manage several on the same character:

```cpp
rmk::Animation walk("player_walk.png", {{0, 0}, {32, 48}}, 6, {32, 48});
player.linkAnimation({"walk", walk});

player.animation("walk").play(-1, 10);
```

---

[:octicons-arrow-left-24: Previous chapter](../sound/sound.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](physic.md){ .md-button .md-button--primary }

---