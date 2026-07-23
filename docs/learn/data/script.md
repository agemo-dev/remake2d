# Lua hot reload

Hot reloading is an essential module for making rapid program debugging easier.

---

## Overview

**RE:MAKE 2D** natively supports **Lua script execution**. This allows you to change values at runtime, or even code an entire game in Lua ... although the features are somewhat 
limited on this interface. For this we'll use the `Script` type, contained in the header **"remake2d/script.hpp"**.

---

## Methods

The methods of the `Script` class are as follows:

```cpp
void update(void);                             // run (or re-run) the Lua file
template<typename T> T get(std::string_view);  // read a global Lua variable
Signal<> onFileChanged;                        // emitted when the file changes on disk
```

---

## Usage

### Loading a script

The constructor simply takes the path to a Lua file on disk:

```cpp
Script(std::string_view file);
```

Once constructed, the file isn't executed yet; `update` is what runs it, and it can be called again at any time to re-run the script:

```lua
-- rock.lua
size     = 45
friction = 0.8
```

```cpp
rmk::Script rock("rock.lua");
rock.update(); // runs the file, populating its environment
```

### Reading Lua variables

Once the script has run, any global variable declared in the Lua file becomes readable from C++ through `get<T>`:

```cpp
rmk::f32 size     = rock.get<rmk::f32>("size");
rmk::f32 friction = rock.get<rmk::f32>("friction");
```

### Hot reloading

`onFileChanged` is a `Signal<>` bound internally to compare the file's last write time on disk against the last time it was checked. Joining it lets you react whenever the
file is saved externally, without restarting your program:

```cpp
rock.onFileChanged.join([&]() {
    rock.update();
    rockShape.resize({rock.get<rmk::f32>("size")});
});
```

---

## Coding in Lua

`Script` isn't limited to reading values: since the engine registers its own types and singletons directly into the shared Lua state, it's possible to write a large part, 
or even the entirety, of a game's logic directly in Lua.

The engine's **C++ namespaces** are respected on the Lua side as nested tables: `rmk::window::pos::centered` becomes `rmk.window.pos.centered`, `rmk::color::red` becomes 
`rmk.color.red`, and so on. Types like `Vec2d`, `Window`, `PhysicBody` or `Signal` are also directly accessible under `rmk.<TypeName>`:

```lua
-- game.lua
local win = rmk.Window("My Game", rmk.window.pos.centered, rmk.window.size.hd)
local circle = rmk.Circle(win:center(), 100)

rmk.loop:execute(win, function()
    win:draw(circle, rmk.color.cyan)
end)

rmk.loop:update()
```

!!! warning
	Used `variable:method()` or `method(variable)` for methods, and `variable.member` for namespaces (tables) and members .
	sous peine de causer **segment fault** .

That script can then be run from a minimal C++ entry point, which simply loads and launches the file:

```cpp
rmk::Script game("game.lua");
game.update();
```

!!! info
    Every function and signal exposed on the C++ side follows the same Lua calling convention (`object:method(...)`), as long as it has been registered beforehand;
	see the [Load type](loadtype.md) section to register your own C++ types and make them usable the same way.

---

[:octicons-arrow-left-24: Previous chapter](isavable.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](loadtype.md){ .md-button .md-button--primary }