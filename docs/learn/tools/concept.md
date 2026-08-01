# Concept

Constraining a template to only accept certain types avoids a lot of obscure compilation errors; concepts are the C++20 
tool that makes this clean.

---

## Overview

**RE:MAKE 2D** defines its own concepts to restrict its templates to the expected types: shapes, textures, physics bodies, 
actors, signals, savable types, and base value types. This lives in the header **"remake2d/concept.hpp"**, relying on `std::derived_from`
and `std::same_as`/`std::is_arithmetic_v`.

---

## Inheritance concepts

Most concepts here simply check that a type derives from a given base class:

```cpp
template<typename G>
concept IsShape = std::derived_from<G, class Geometry>;

template<typename T>
concept IsTexture = std::derived_from<T, class TextureBase>;

template<typename P>
concept IsPhysic = std::derived_from<P, class PhysicBody>;

template<typename A>
concept IsActor  = std::derived_from<A, class ActorBase>;

template<typename S>
concept IsSignal = std::derived_from<S, class SignalBase>;

template<typename S>
concept IsSavable = std::derived_from<S, class ISavable>;
```

- `IsShape`   : constrains a type to inherit from `Geometry`.
- `IsTexture` : constrains a type to inherit from `TextureBase`.
- `IsPhysic`  : constrains a type to inherit from `PhysicBody`.
- `IsActor`   : constrains a type to inherit from `ActorBase`.
- `IsSignal`  : constrains a type to inherit from `SignalBase`.
- `IsSavable` : constrains a type to implement `ISavable`.

These show up throughout the engine whenever a function or class needs to stay generic while still rejecting arbitrary types:

```cpp
// collision test between two shapes
template<IsShape T1, IsShape T2> bool intersect(T1 a, T2 b) {
	return a.hasIntersected(b);
}
```

---

## `IsBasicType`

`IsBasicType` gathers every base value type of the engine, plus any arithmetic type (`int`, `float`, etc.):

```cpp
template<typename T>
concept IsBasicType = std::same_as<T, struct Vec2d>         ||
                      std::same_as<T, struct Fact2d>        ||
                      std::same_as<T, struct Grid2d>        ||
                      std::same_as<T, struct Dim2d>         ||
                      std::same_as<T, struct Area>          ||
                      std::same_as<T, struct Triangulation> ||
                      std::same_as<T, struct Color>         ||
                      std::same_as<T, struct HSL>           ||
					  std::is_arithmetic_v<T>;
```

This concept is what lets any base value stream directly into a `Text` with `<<`, without overloading the operator for each type one by one:

```cpp
template<IsBasicType T> T add(T a, T b) {
	return a + b;
}
```

!!! info
    `IsBasicType` accepts any arithmetic type. This means custom numeric aliases such as `rmk::i32` or `rmk::f32` also satisfy the concept.

---

[:octicons-arrow-left-24: Previous chapter](stream.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](utility.md){ .md-button .md-button--primary }
