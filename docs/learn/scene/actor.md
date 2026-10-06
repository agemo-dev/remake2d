# Actor

A player, an enemy, a projectile: each one needs its own logic, run every frame, and often a link to the others
(a weapon following its owner, for example). That's what `Actor` is for.

---

## Overview

`Actor` and `PhysicActor` are contained in the header **"remake2d/actor.hpp"**. `Actor` is an abstract class: you inherit from it, write its
`update` method, then add it to a `Scene` that calls it every frame. An actor can also have children, updated
automatically along with it. `PhysicActor` is a variant that directly embeds a [physics body](../physic/body.md).

---

## Actor

### Methods

```cpp
virtual void update(void) = 0;                   // logic run every frame (to implement)

void active(bool)       noexcept;                // enable or disable the actor
bool active(void) const noexcept;                // check if it is active

void addChild(Actor&)    noexcept;               // attach a child
void removeChild(Actor&) noexcept;               // detach a child

UnsafeTracker<Actor>&                    parent(void)   noexcept;   // parent (empty if none)
std::vector<UnsafeTracker<Actor>>&       children(void) noexcept;   // list of children
```

`parent` and `children` also have a `const` version.

!!! info
    `parent()` and `children()` return `UnsafeTracker` rather than `Tracker`. Inside its own class, `Actor` is still an incomplete type,
    so the `IsTrackable` constraint cannot be evaluated (see [Tracker](../core/tracker.md)). You can convert the result to
    `Tracker<Actor>` if you want that guarantee back.

### Usage

#### Creating an actor

Inherit from `Actor` and implement `update`:

```cpp
class Player : public rmk::Actor {
public:
    void update(void) override {
        // called once per frame
    }
};
```

The actor is then added to a scene, which takes care of calling it (see [Scene & Act](scene.md)):

```cpp
rmk::Scene scene;
Player     player;

scene.add(player, rmk::layer::world(0));
```

#### Enabling and disabling

A disabled actor is no longer updated, without being removed from the scene:

```cpp
player.active(false);
if (!player.active()) { /* skipped by the scene */ }
```

#### Parent and children hierarchy

A weapon following its owner doesn't need to be added to the scene: attach it to its parent with `addChild`.

```cpp
Player player;
Weapon sword;

player.addChild(sword);

sword.parent();    // tracker to player
player.children(); // contains sword
```

If the child already had a parent, it is detached from it before being attached to the new one. `removeChild` does the opposite, and `parent()` becomes empty again:

```cpp
player.removeChild(sword);

if (!sword.parent()) { /* no more parent */ }
```

!!! warning
    There is no cycle check: attaching an actor to one of its own descendants causes infinite recursion during the update.

#### Update order

When an actor is updated by the scene, its `update` is called first, then those of its children, depth first:

```cpp
// for each active actor in the scene:
//   actor.update()
//     child1.update()
//       grandchild.update()
//     child2.update()
```

!!! info
    Only the `active` state of actors added **directly to the scene** is checked. Disabling a parent therefore disables its whole subtree,
    but a child's own `active` flag is not read while its parent is being updated.

#### Destruction and moving

An actor is a Trackable, so these cases stay safe:

- **Destroying a parent**: its children lose their parent (`parent()` becomes empty).
- **Destroying a child**: its tracker stays in the parent's list, but it is empty and skipped during the update. Remember to test it if you iterate over `children()`.
- **Moving an actor** (for example inside a `std::vector` that reallocates): the parents, children and scenes referencing it keep pointing to it.

!!! info
    Copying an actor does not copy its parent and children .

---

## PhysicActor

`PhysicActor<P>` is an `Actor` that owns a physics body, reachable through the public member `body`. The parameter `P` must be a
`PhysicBody` (the `IsPhysic` constraint). Two aliases are provided:

```cpp
using StaticActor  = PhysicActor<StaticBody>;
using DynamicActor = PhysicActor<DynamicBody>;
```

### Methods

```cpp
PhysicActor(const Geometry&);          // builds the body from a shape

P body;                                // the physics body

virtual void update(void) override {}; // empty by default
```

Unlike `Actor`, `update` is not pure here: a `PhysicActor` can be used as is, without inheriting from it.

### Usage

#### Direct use

A wall or a platform has no logic of its own, so a `StaticActor` is enough:

```cpp
rmk::Rectangle floorShape({400, 550}, {800, 50});
rmk::StaticActor floor(floorShape);

scene.add(floor, rmk::layer::world(0));
```

The body is handled through `body` (see [Physic entity](../physic/body.md)):

```cpp
rmk::Rectangle heroShape({400, 100}, {32, 48});
rmk::DynamicActor hero(heroShape);

hero.body.move({10, 0});
win.draw(hero.body);
```

#### Adding logic

For custom behavior, inherit from the alias and override `update`:

```cpp
class Enemy : public rmk::DynamicActor {
public:
    Enemy(const rmk::Geometry& shape) : rmk::DynamicActor(shape) {}

    void update(void) override {
        body.move({-1, 0});
    }
};
```

---

## Property

| Type | Copiable | Movable | Bases and Traits |
|---|---|---|---|
| Actor | Yes | Yes | Trackable |
| PhysicActor | Yes | Yes | Actor |

---

[:octicons-arrow-left-24: Previous chapter](../maps/parallax.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](scene.md){ .md-button .md-button--primary }