# Tracker

Following an object across frames usually means keeping a raw pointer to it , until that object moves in memory, or gets
destroyed, and the pointer silently turns into a landmine. `Tracker` gives you a safe way to reference an object without owning it.

---

## Overview

`Tracker`, `Slot` and `Trackable` are contained in the header **"remake2d/tracker.hpp"**. Together they let any class opt into
being followed safely: `Trackable<Derived>` is a base to inherit from, `Tracker<Derived>` is the handle you keep around, and
`Slot<Derived>` is the small piece of shared state that connects the two. `Camera::follow` is the main place this shows up in
the engine, but nothing ties `Tracker` to cameras specifically , any class can use it.

```cpp
template<typename T> class Slot;      // internal indirection, rarely touched directly
template<typename T> class Tracker;   // the handle you keep
template<typename Derived> class Trackable;   // the base you inherit from
```

---

## The problem it solves

A plain pointer or reference to an object breaks in two common situations:

- **the object moves** : e.g. it lives inside a `std::vector` that reallocates when it grows;
- **the object is destroyed** : nothing marks the pointer as invalid, so using it afterwards is undefined behavior.

`Tracker<T>` handles both: `locate()` always returns the object's current address, or `nullptr` if it no longer exists.

```cpp
rmk::Tracker<Player> t = player.tracker();

if (Player* p = t.locate()) {
    p->takeDamage(10); // still alive, safe to use
}
```

---

## Trackable

### Methods

```cpp
Tracker<Derived> tracker(void) noexcept;   // hand out a safe reference to this instance
void relocate(void) noexcept;              // rebind after a move
```

### Usage

#### Making a class followable

Inherit from `Trackable<Derived>`, using the class itself as the template argument (the [CRTP](https://en.cppreference.com/w/cpp/language/crtp) pattern):

```cpp
class Player : public rmk::Trackable<Player> {
public:
    Vec2d center(void) const noexcept { return m_center; }
private:
    Vec2d m_center;
};
```

That's enough for construction, copying, and destruction ; nothing else to write. Calling `tracker()` at any point after
construction hands out a `Tracker<Player>` that stays valid for as long as the `Player` does:

```cpp
Player p;
rmk::Tracker<Player> t = p.tracker();
```

A copy is a distinct object, not a stand-in for the original ; copying a `Player` never redirects trackers already pointing
at the source:

```cpp
Player original;
rmk::Tracker<Player> t = original.tracker();

Player copy = original;
t.locate(); // still &original, not &copy
```

#### The one rule: moving

Moving is the one case `Trackable` can't handle silently ; the language itself gets in the way. A moved-from object's base
class finishes constructing *before* the derived class exists, so there's no safe moment for `Trackable` to relink the slot
to the new address on its own. Every move constructor and move-assignment operator of a `Trackable`-derived class **must**
call `relocate()` as its last step:

```cpp
class Player : public rmk::Trackable<Player> {
public:
    Player(Player&& other) noexcept
        : rmk::Trackable<Player>(std::move(other)), m_center(other.m_center) {
        relocate();
    }

    Player& operator=(Player&& other) noexcept {
        rmk::Trackable<Player>::operator=(std::move(other));
        m_center = other.m_center;
        relocate();
        return *this;
    }
    // ...
};
```

!!! warning
    Forgetting `relocate()` in a move doesn't fail loudly. Any `Tracker` obtained *before* the move keeps pointing at the old 
    address until something calls `tracker()` again , which, for an object living inside a reallocating `std::vector`, is 
    exactly the address that just became invalid.

Everyday containers reallocate all the time, and this is precisely the case `Tracker` is built to survive ; as long as the 
element type moves correctly:

```cpp
std::vector<Player> players;
players.reserve(1);
players.emplace_back();

rmk::Tracker<Player> t = players[0].tracker();

for (int i = 0; i < 100; ++i) players.emplace_back(); // reallocates, moves every Player

t.locate(); // still valid: points at the relocated element
```

---

## Tracker

### Methods

```cpp
T* locate(void) const noexcept;     // current address, or nullptr if the object is gone
T* operator->(void);                // shorthand for locate(); also nullptr if gone
void track(const Balise&) noexcept; // rebind to a different Slot
```

### Usage

#### Checking before use

`locate()` and `operator->` never crash on a destroyed object . They return `nullptr` instead, so the check is explicit:

```cpp
if (t.locate()) {
    t->takeDamage(10);
}
```

#### A default-constructed Tracker tracks nothing

```cpp
rmk::Tracker<Player> t; // not bound to anything yet
t.locate(); // nullptr

Player p;
t = p.tracker();
t.locate(); // &p
```

---

[:octicons-arrow-left-24: Previous chapter](event.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](croutine.md){ .md-button .md-button--primary }