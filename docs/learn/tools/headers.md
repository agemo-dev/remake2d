# Interface headers

Including multiple headers separately can quickly become cumbersome, which is
why it is important to be able to avoid it.

---

## Overview

To avoid repetitive includes, **RE:MAKE 2D** offers **interface headers**
so you can include a whole category of headers at once.
These categories are the following:

- everything
- assets
- bases
- cores
- entities
- graphics
- maps
- times
- tools
- traits
- types

---

## Usage

To include one of these categories, simply do: `#include <remake2d/all/<CategoryName>.hpp>`
example:

```cpp
#include <remake2d/all/everything.hpp>
```

This way, every header contained within it will be included as well.

---

## Content

### Everything

The `everything` interface simply includes every public header of the engine.

```cpp
#include <remake2d/numeric.hpp>
#include <remake2d/concept.hpp>
#include <remake2d/utility.hpp>
#include <remake2d/croutine.hpp>
#include <remake2d/tracker.hpp>
#include <remake2d/math.hpp>
#include <remake2d/lock.hpp>
#include <remake2d/error.hpp>
#include <remake2d/color.hpp>
#include <remake2d/vector.hpp>
#include <remake2d/shape.hpp>
#include <remake2d/signal.hpp>
#include <remake2d/time.hpp>
#include <remake2d/clock.hpp>
#include <remake2d/system.hpp>
#include <remake2d/event.hpp>
#include <remake2d/sound.hpp>
#include <remake2d/texture.hpp>
#include <remake2d/actor.hpp>
#include <remake2d/scene.hpp>
#include <remake2d/window.hpp>
#include <remake2d/camera.hpp>
#include <remake2d/physic.hpp>
#include <remake2d/tilemap.hpp>
#include <remake2d/parallax.hpp>
#include <remake2d/tilegrid.hpp>
#include <remake2d/data.hpp>
#include <remake2d/stream.hpp>
#include <remake2d/random.hpp>
#include <remake2d/loop.hpp>

#ifdef REMAKE2D_ADDON_SCRIPT
#include <remake2d/script.hpp>
#endif
```

Note that it also includes the add-on headers if they are linked to the
project ;) .

### Assets

The `assets` interface includes every module responsible for managing
external files:

```cpp
#include <remake2d/data.hpp>
#include <remake2d/sound.hpp>
#include <remake2d/texture.hpp>

#ifdef REMAKE2D_ADDON_SCRIPT
#include <remake2d/script.hpp>
#endif
```

### Bases

The `bases` interface includes every header expected to be included by any
project using this engine; think of it as a base kit for every file
containing the `main` function:

```cpp
#include <remake2d/loop.hpp>
#include <remake2d/scene.hpp>
#include <remake2d/event.hpp>
#include <remake2d/shape.hpp>
#include <remake2d/window.hpp>
#include <remake2d/utility.hpp>
#include <remake2d/all/types.hpp>
```

### Cores

The `core` interface lets you include every module that is either heavily
used or essential to the engine's internal operation; it is, in a sense,
the heart of the engine:

```cpp
#include <remake2d/math.hpp>
#include <remake2d/lock.hpp>
#include <remake2d/error.hpp>
#include <remake2d/system.hpp>
#include <remake2d/signal.hpp>
#include <remake2d/croutine.hpp>
```

### Entities

The `entities` interface includes every module responsible for entity
management:

```cpp
#include <remake2d/actor.hpp>
#include <remake2d/shape.hpp>
#include <remake2d/physic.hpp>
```

### Graphics

This one includes every module related to graphics and basic rendering:

```cpp
#include <remake2d/color.hpp>
#include <remake2d/shape.hpp>
#include <remake2d/window.hpp>
#include <remake2d/texture.hpp>
```

### Maps

The `maps` interface includes every module that can be used to display a
background or generate a map:

```cpp
#include <remake2d/tilemap.hpp>
#include <remake2d/tilegrid.hpp>
#include <remake2d/parallax.hpp>
```

### Times

Includes every time-management module:

```cpp
#include <remake2d/time.hpp>
#include <remake2d/clock.hpp>
```

### Tools

Includes every header of the general-purpose modules that act as a sort of
**toolbox**:

```cpp
#include <remake2d/stream.hpp>
#include <remake2d/random.hpp>
#include <remake2d/concept.hpp>
#include <remake2d/utility.hpp>
```

### Traits

Includes every trait module of the engine, such as: Drawable, Followable, or
Updatable. For this interface, the headers involved won't be listed here,
since this interface **cheats** by including the engine's private headers
directly to avoid pulling in unnecessary content; though the more curious
among you, who have read the source, will already know what's inside...

### Types

And finally, the baby of the family, which simply includes the **base
type** modules:

```cpp
#include <remake2d/area.hpp>
#include <remake2d/color.hpp>
#include <remake2d/vector.hpp>
#include <remake2d/numeric.hpp>
```

---

[:octicons-arrow-left-24: Previous chapter](random.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](../data/data.md){ .md-button .md-button--primary }