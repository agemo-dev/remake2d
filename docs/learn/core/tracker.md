# Tracker

Following an object across frames usually means keeping a raw pointer to it , until that object moves in memory, or gets
destroyed, and the pointer silently turns into a landmine. `Tracker` gives you a safe way to reference an object without owning it.

---

## Overview

`Tracker`, `Slot` and `Trackable` are contained in the header **"remake2d/tracker.hpp"**. Together they let any class opt into
being followed safely: `Trackable<Derived>` is a base to inherit from, `Tracker<Derived>` is the handle you keep around, and
`Slot<Derived>` is the small piece of shared state that connects the two. `Camera::follow` is the main place this shows up in
the engine, but nothing ties `Tracker` to cameras specifically , any class can use it.

`Tracker<T>` is in fact an alias for `UnsafeTracker<T>` constrained by `IsTrackable<T>`; some parts of the public API (like
`Actor::parent()`) hand out `UnsafeTracker` directly rather than `Tracker`, on the idea that at that point it's little more
than a plain pointer, and it's up to you to decide whether to re-wrap it as a `Tracker` for that compile-time guarantee back.

```cpp
template<typename T> class Slot;            // internal indirection, rarely touched directly
template<typename T> class Tracker;         // the handle you keep
template<typename Derived> class Trackable; // the base you inherit from
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
UnsafeTracker<Derived> tracker(void) noexcept;   // hand out a reference to this instance
void relocate(void) noexcept;                    // rebind after a move
```

`tracker()` is declared to return `UnsafeTracker<Derived>` rather than `Tracker<Derived>`: inside `Trackable<Derived>`'s own
generic implementation, there's no way to guarantee `Derived` already satisfies `IsTrackable<Derived>` at that point, since
`Derived` is still being defined when it inherits from `Trackable<Derived>`. Writing `Tracker<Derived>` at the call site
works fine once `Derived` is complete, since `Tracker<T>` is just `UnsafeTracker<T>` with that check attached.

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
T&       operator*(void);
T*       operator->(void);
const T& operator*(void)  const;
const T* operator->(void) const;
explicit operator bool()  const;
bool     operator==(const UnsafeTracker<T>&) const noexcept;
bool     operator!=(const UnsafeTracker<T>&) const noexcept;

template<typename U = T> requires IsRelatedTo<U, T>
U* locate(void) noexcept; // current address as U, or nullptr if the object is gone (mutable)

template<typename U = T> requires IsRelatedTo<U, T>
const U* locate(void) const noexcept; // current address as U, or nullptr if the object is gone (constant)
```

`locate()` takes an optional template parameter `U` (defaulting to `T`) and converts the returned pointer to it: a
`static_cast` when `U` and `T` are related by inheritance, a `dynamic_cast` otherwise — handy for pulling out a base or
derived pointer without going through `operator->` first.

### Usage

#### Checking before use

`locate()` and `operator->` never crash on a destroyed object . They return `nullptr` instead, so the check is explicit:

```cpp
if (t) {
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

## Property

| Type | Copiable | Movable | Bases and Traits |
|---|---|---|---|
| UnsafeTracker | Yes | Yes | None |
| Tracker | Yes | Yes | None |

---

[:octicons-arrow-left-24: Previous chapter](event.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](croutine.md){ .md-button .md-button--primary }