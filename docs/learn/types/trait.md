# Traits

---

## Overview

In this section we will learn what a **RE:MAKE 2D** trait is.
If you are reading the documentation in order (which is strongly
recommended), you have probably already noticed this kind of section at
the end of the next pages:

| Type | Copyable | Movable | Traits |
|---|---|---|---|
| Window | No | Yes | Trackable |

We already understand what the `copyable` and `movable` properties are;
they simply indicate whether the mentioned type can be copied or moved.
In the example above, the `Window` type is not copyable, but it is movable
and has the **Trackable** trait.
On this page, we will focus on the `trait` property. It indicates whether a
type inherits from one of the engine's **traits** or not.

> But what exactly is a **trait**?

Well, a **trait** is the name given to a class that serves as an interface
for the engine, allowing for an extensible user-facing API.
This name is borrowed from **Rust traits**. For reference, see the
[official Rust documentation on traits](https://doc.rust-lang.org/book/ch10-02-traits.html).

---

## Usage

A user class only needs to inherit **publicly** from these types to be used
within the engine.
Each trait has a condition to fulfill, often a pure virtual method to
implement, but this will be explained in more depth on dedicated pages.

```cpp
class MyClass : public Trait {
public:
    void methodToImplement(void) const noexcept override;
};
```

---

### Different traits

**RE:MAKE 2D** offers several traits, namely:

- [Drawable](../graphics/draw.md)
- [Fillable](../graphics/draw.md)
- [Followable](..)
- [Updatable](..)
- [Trackable](..)
- [Savable](..)
- [Actor](..)

Most of the time, their name will end with **-able**, but there can be
exceptions (such as `Actor`).

---


[:octicons-arrow-left-24: Previous Chapter](color.md){ .md-button }
[Next Chapter :octicons-arrow-right-24:](../graphics/window.md){ .md-button .md-button--primary }