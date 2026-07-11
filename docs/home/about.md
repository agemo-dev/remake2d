# About **"RE:MAKE 2D"**

---

## What is **RE:MAKE 2D** ?

**RE:MAKE 2D** is a **full code, object-oriented game engine** designed
for 2D game development in C++. Built around a clear and accessible API,
it targets both junior and senior developers alike, and is suitable for
learning purposes as well as commercial projects.

Originally, this engine was nothing more than a personal library aimed
at simplifying SDL2's verbose syntax. Over time, it grew to incorporate
features inspired by other game engines, such as signals and tilemaps,
while developing its own paradigms, including a declarative event
system, Lua hot-reloading, and a chronometer system.

The first version of **RE:MAKE 2D** was developed between **01/01/2026**
and **01/06/2026**, the latter date also marking its public release. The
entire development was carried out by a single person; as such, the
engine may present certain functional gaps or bugs. Any feedback or
remarks are warmly welcome at this [email address](mailto:my.agemo.dev@gmail.com).

!!! info
    **RE:MAKE 2D** does not yet have a dedicated community. The creation
    of such a community is planned for future versions, in order to
    allow external contributions to the project.

---

## Why this name ?

The name may sound presumptuous, but it is the result of a natural
evolution of the project. As mentioned above, this engine originated
as a personal project aimed at reducing SDL2's syntactic overhead. At
the time, still relatively inexperienced in software architecture, my
codebase grew increasingly disorganized, unreadable, and difficult to
maintain as the project expanded.

I then decided to start over from scratch, with a more rigorous
object-oriented architecture. The engine was first named **RP NODE**,
in reference to Godot's node system. The gradual addition of features
absent from SDL2, starting with signals, led to a second renaming:
**OKAPI 2D**, reflecting the hybridization of concepts drawn from
various engines.

The name **RE:MAKE 2D** eventually settled for a simple reason: very
few C++ engines adopt this style of API, and the approach proposed
here stood apart enough to warrant its own identity. The prefix *RE:*
points directly to this story of rebuilding from scratch, while *MAKE*
echoes the engine's core purpose: building a game, without detours.
In short, the goal was never to **reinvent 2D development** in C++, but
rather to **propose a different way of approaching it**, by grouping
numerous features within a cohesive API, so that developers do not
have to manually integrate and learn external libraries whose setup
can prove complex and error-prone.

---

## Why use **RE:MAKE 2D** ?

**RE:MAKE 2D** is by no means the "best 2D engine in the world", and
its API style will not necessarily suit every developer profile. It
was designed with a specific goal in mind: to simplify 2D game
development and improve project organization, by relying on
specialized, battle-tested, and actively maintained libraries — SDL2
for rendering, Box2D for physics, among others — in order to reduce
the risks associated with integrating third-party dependencies.

It provides an **intuitive API** paired with **solid performance**,
making it well-suited for rapid prototyping. Thanks to its many
built-in modules, it reduces the number of external dependencies to
manage, and avoids the compatibility issues that often arise when
integrating them into a project.

Ultimately, one fully satisfied user matters more than a large but
dissatisfied community. It is with this mindset that the engine will
continue to evolve, guided by the feedback and remarks of those who
use it.

---

## Where and how to learn **RE:MAKE 2D** ?

The documentation is primarily available in the
[Learn](../learn/types/numeric.md) section of the site, organized
into progressive categories ranging from creating a window to
advanced usage of the engine's core systems.

!!! info
    A [cheatsheet](cheatsheet.md) is also available in the **Home**
    section of the site, offering a quick and concise overview of the
    engine's essential features.

---

## How to contribute ?

Pull requests are reviewed and integrated after verification and
adaptation to the existing API. It is also possible to report a bug
or suggest a new feature by [email](mailto:my.agemo.dev@gmail.com).
Any person who has contributed to the project will be credited in
the **Contributors** section.

!!! info
    **RE:MAKE 2D** does not yet have a dedicated **GitHub Wiki** for
    contributions, but this feature is planned for a future version.