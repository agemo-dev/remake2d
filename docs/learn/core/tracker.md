# Tracker

Following an object across frames usually means keeping a raw pointer to it, until that object moves in memory, or gets
destroyed, and the pointer silently turns into a landmine. `Tracker` gives you a safe way to reference an object without owning it.

---

## Overview

`Tracker<T>`, `Box<T>` and `Trackable` are contained in the header **"remake2d/tracker.hpp"**. Together they let any class opt into being followed safely:
`Trackable` is a trait to inherit from, `Tracker<T>` is the handle you keep around, and `Box<T>` is a wrapper to track otherwise untrackable types.


```cpp
class Trackable;                                                    // the base you inherit from
template<typename T> class Box;                                     // wrapper, inherits from Trackable
template<typename T> class UnsafeTracker;                           // the handle you keep
template<IsTrackable SafeType> using Tracker = UnsafeTracker<SafeType>; // the handle you keep
```

`Tracker<T>` checks `IsTrackable<T>`, which needs `T` to be a complete type with a public `Trackable` base. Inside the
body of a class, that class is still incomplete, so a class cannot hold a `Tracker` to itself as a member:

```cpp
class Foo : public rmk::Trackable {
    rmk::Tracker<Foo>       a; // error: Foo is incomplete here
    rmk::UnsafeTracker<Foo> b; // OK, no constraint
};
```
This is why parts of the API such as `Actor::parent()` return an `UnsafeTracker`. Inside member function bodies, the class is
complete and `Tracker<Foo>` works normally.

---

## The problem it solves

A plain pointer or reference to an object breaks in two common situations:

- **the object moves**: e.g. it lives inside a `std::vector` that reallocates when it grows;
- **the object is destroyed**: nothing marks the pointer as invalid, so using it afterwards is undefined behavior.

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
Tracker<Trackable> tracker(void) const noexcept;   // hand out a reference to this instance
```

`tracker()` returns a `Tracker<Trackable>`, which converts implicitly to a `Tracker<Derived>` thanks to the converting
constructor of `UnsafeTracker` (see below). Writing `Tracker<Player>` at the call site works fine once `Player` is complete.

### Usage

#### Making a class followable

Inherit from `Trackable`:

```cpp
class Player : public rmk::Trackable {

public:
    Vec2d center(void) const noexcept { return m_center; }

private:
    Vec2d m_center;
};
```

That is enough for construction, copying, moving and destruction; nothing else to write. Calling `tracker()` at any point after
construction hands out a `Tracker<Player>` that stays valid for as long as the `Player` does:

```cpp
Player p;
rmk::Tracker<Player> t = p.tracker();
```

A copy is a distinct object, not a stand-in for the original; copying a `Player` never redirects trackers already pointing
at the source:

```cpp
Player original;
rmk::Tracker<Player> t = original.tracker();

Player copy = original;
t.locate(); // still &original, not &copy
```

#### Moving

Moving is handled automatically: the move constructor and the move assignment operator of `Trackable` relink the slot to the
new address, so a derived class never has to do it itself. A defaulted move needs nothing more:

```cpp
class Monster : public rmk::Trackable {
public:
    Monster(Monster&&)            noexcept = default; // OK: Trackable's move constructor is called implicitly
    Monster& operator=(Monster&&) noexcept = default;
    // ...
};
```

The only rule applies when you write a move by hand: **you must move the base class explicitly**. If you forget it,
`Trackable` is default-constructed instead, and trackers keep following the moved-from object.

```cpp
class Player : public rmk::Trackable {
public:
    Player(Player&& other) noexcept
        : rmk::Trackable(std::move(other)), m_center(other.m_center) {}

    Player& operator=(Player&& other) noexcept {
        rmk::Trackable::operator=(std::move(other));
        m_center = other.m_center;
        return *this;
    }
    // ...
};
```

Everyday containers reallocate all the time, and this is precisely the case `Tracker` is built to survive, as long as the
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
UnsafeTracker(void);                                              // tracks nothing

template<typename U> requires IsRelatedTo<U, T>
UnsafeTracker(const UnsafeTracker<U>&) noexcept;                  // conversion between related types

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

`locate()` takes an optional template parameter `U` (defaulting to `T`) and converts the returned pointer to it. It uses a
`static_cast` whenever one is valid for `U`, and falls back to a `dynamic_cast` only when it is not (virtual inheritance, or a
cross cast between branches). With `U = void` it returns a `void*`. It is handy for pulling out a base or derived pointer
without going through `operator->` first.

### Usage

#### Checking before use

`locate()` and `operator->` never crash on a destroyed object. They return `nullptr` instead, so the check is explicit:

```cpp
if (t) {
    t->takeDamage(10);
}
```

`operator*` is different: it has no null result to return. It triggers an `assert` in debug builds and is undefined
behavior in release builds if the object is gone, so only use it once you know the tracker is valid.

#### A default-constructed Tracker tracks nothing

```cpp
rmk::Tracker<Player> t; // not bound to anything yet
t.locate(); // nullptr

Player p;
t = p.tracker();
t.locate(); // &p
```

---

## Box

### Usage

`Box<T>` is a **wrapper** used to wrap various types. The engine only lets you track types that inherit from
`Trackable`, so `Box<T>` acts as an adapter that makes a non trackable type (`int`, `std::string`, etc.) trackable:

```cpp
rmk::Box<int> num;
std::cout << num.value << std::endl;

// or by dereferencing
std::cout << *num << std::endl;
```

### Methods

This type has two overloads, const and mutable, of the `*` (dereference) and `->` operators:

```cpp
T&       operator*(void)       noexcept { return value; }
const T& operator*(void) const noexcept { return value; }

T*       operator->(void)       noexcept { return &value; }
const T* operator->(void) const noexcept { return &value; }
```

It only holds one public member:

```cpp
T value;
```

`Box<T>` inherits from `Trackable`, so it can be tracked like any other trackable class.

---

## Credits

This module is entirely taken from the [skiptracer library](https://github.com/agemo-dev/skiptracer).
You can also explore it for more details.

---

## Property

| Type | Copiable | Movable | Bases and Traits |
|---|---|---|---|
| Trackable | Yes | Yes | None |
| Box | Yes | Yes | Trackable |
| UnsafeTracker | Yes | Yes | None |
| Tracker | Yes | Yes | None |

---

[:octicons-arrow-left-24: Previous chapter](event.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](croutine.md){ .md-button .md-button--primary }