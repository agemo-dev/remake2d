# Coding in Lua

`Script` isn't limited to reading values: since the engine registers its own types and singletons directly into the shared Lua state, it's possible to write a large part,
or even the entirety, of a game's logic directly in Lua.

---

### Namespaces and types

The engine's **C++ namespaces** are respected on the Lua side as nested tables: `rmk::window::pos::centered` becomes `rmk.window.pos.centered`, `rmk::color::red` becomes
`rmk.color.red`, and so on. Registered types are directly accessible under `rmk.<TypeName>`, and are constructed the same way their C++ constructor would be called:

```lua
local win    = rmk.Window("My Game", rmk.window.pos.centered, rmk.window.size.hd)
local circle = rmk.Circle(win:center(), 100)
local area   = win:area()
```

Every function and signal exposed on the C++ side follows the same calling convention as any Lua method (`object:method(...)`), as long as it has been registered
beforehand; see the [Load type](loadtype.md) section to register your own C++ types and make them usable the same way.

---

### The main loop

`rmk.loop` drives the whole program. `execute` attaches a window and a per-frame callback; `update` blocks and runs it until the window closes:

```lua
local win = rmk.Window("My Game", rmk.window.pos.centered, rmk.window.size.hd)

rmk.loop:execute(win, function()
    win:draw(circle, rmk.color.cyan)
end)

rmk.loop:update()
```

---

### Shapes and rendering

All shapes (`Circle`, `Square`, `Rectangle`, `Triangle`, `Hexagone`, `Losange`, `Line`, `Point`, `Ellipse`) share the same base interface : `move`,
`resize`, `center`, `size` , etc, and are drawn through `Window::draw`:

```lua
local sqr = rmk.Square(win:center(), 64)
sqr:rotate(0.3)

win:draw(sqr, rmk.color.orange)
```

`Sprites`, `Text`, and `Animation` follow the same pattern .

---

### Physics

`StaticBody` and `DynamicBody` are built from an existing shape. `StaticBody` never moves on its own; `DynamicBody` is simulated by the physics world and exposes its
physical attributes as paired getter/setter methods — calling with no argument reads, calling with one argument writes:

```lua
local body = rmk.DynamicBody(sqr)

body:gravity(false)     -- disable gravity for this body only
body:isBullet(true)     -- continuous collision detection, for fast-moving bodies
body:warp(area)         -- teleport to the opposite edge when leaving `area`
body:limit(area)        -- keep the body confined to `area`

body:push(rmk.Vec2d(0, -50))          -- apply a physical force
body:velocity(rmk.Vec2d(120, 0))      -- set velocity directly
body:jump(300)                        -- upward impulse

body:move(rmk.Vec2d(400, 200))        -- reposition directly (inherited from PhysicBody, absolute)
```

`move` is shared with every other shape and texture in the engine and is always **absolute** — it moves the object *to* the given position, not *by* it.

---

### Actors and scenes

`Scene` groups layers of actors and drives their update/draw cycle; `Act` groups reusable behavior that can be linked to several actors by tag:

```lua
local scene = rmk.Scene()
local intro = rmk.Act()

intro:link("guard", {"torch", "gate"})   -- link this Act to several tagged actors
scene:add(someActor, 0)                  -- add an actor on layer 0
```

---

### Data and persistence

`Data` is a small dynamic value type (numbers, strings, vectors, colors, nested lists and maps) used for saving and loading game state through `DataFile`:

```lua
local save = rmk.Data.map({
    level = rmk.Data(3),
    pos   = rmk.Data(rmk.Vec2d(120, 40)),
})

local file = rmk.DataFile("save1")
file:save(save)

local loaded = rmk.Data()
file:load(loaded)
print(loaded:get("level"))
```

---

### Events, timers, and signals

Anything exposed as a `Signal` on the C++ side — contact events, timers, input ; is joined with `bind`/`join` rather than a raw function pointer:

```lua
body.onContactStart:bind(function(self, other)
    print("hit!")
end)

timer.onTimeout:join(function()
    print("one second passed")
end)
```

---

## What isn't available in Lua, and why

Not everything in the C++ API is exposed as-is. Most of the time this isn't an oversight, but a deliberate boundary between what Lua's runtime can safely and
meaningfully support.

**Coroutines** aren't exposed, because Lua already has its own native coroutines (`coroutine.create`, `coroutine.wrap`, `coroutine.yield`).
`Croutine<>` is the internal mechanism that lets `rmk_pause()`/`rmk_await()` work on the C++ side; a Lua script has no reason to reach for
it when the language already provides the same capability directly.

**`Signal::connect(Slot&)` / `disconnect(Slot&)`** are not bound, for a safety reason rather than a technical one. `connect` only stores a raw pointer to the `Slot` it's
given , it never takes ownership of it. In C++, that's fine: the caller is expected to keep the `Slot` alive (typically as a member of some longer-lived object) for as
long as the connection should exist, and to pass the same object back to `disconnect` later. From Lua, there's no equivalent guarantee: a closure passed as an argument
can be garbage-collected the moment the script stops referencing it, leaving the signal holding a dangling pointer.

This is why the Lua-facing API uses `join`/`bind` instead:

```cpp
void join(Slot&&);             // takes ownership of a temporary Slot
void bind(Condition, Args...);
```

`join` takes its `Slot` by rvalue reference and keeps it internally , there's no external object whose lifetime the caller (or the garbage collector) needs to manage.
That's the method registered on every Lua-facing signal (`onContactStart:bind(...)`, `Timer:join(...)`, and so on).

Some classes/methods have templates without specialization .

**Stream operators (`operator<<`) and RAII guards (`PauseGuard` and similar)** stay C++-only by nature: the former exists for `std::cout`-style debugging, which Lua
already covers with `print`/`tostring`; the latter relies on deterministic destruction at C++ scope exit, a concept Lua's garbage-collected model doesn't have a direct
equivalent for.

---

[:octicons-arrow-left-24: Previous chapter](loadtype.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](../core/error.md){ .md-button .md-button--primary }