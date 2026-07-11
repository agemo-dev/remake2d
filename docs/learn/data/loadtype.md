# Load type

Reading Lua variables from C++ only goes so far if the reverse isn't possible: registerType is what lets Lua manipulate custom C++ classes, 
exactly the way the engine already does for its own internal types.

---

## Overview

As explained in **Lua hot-reload**, it's possible to write an entire game in Lua with RE:MAKE 2D; unfortunately OOP doesn't exist there, so 
RE:MAKE 2D offers an interface to load custom C++ types and make them directly usable from Lua. This goes through `SolState`, exposed through 
the singleton `script`, contained in the header **"remake2d/script.hpp"**.

---

## Methods

```cpp
template<typename T> void loadVar(std::string_view id, T& data) noexcept;

std::string loadedTypes(void)              const noexcept;
bool        isLoadedType(std::string_view) const noexcept;
```

---

## Usage

### Registering a type

registerType takes the type to expose, its constructor signatures, and an initializer function that binds member functions and fields to 
Lua-accessible names:

```cpp
template<typename T, typename... Ctors>
void registerType(std::string_view name, std::function<void(SolState::Type&)> init = nullptr);
```

```cpp
class Player {
public:
    Player(void) = default;
    void move(rmk::f32 x, rmk::f32 y) { pos.x += x; pos.y += y; }
    rmk::Vec2d pos;
    rmk::i32   hp{100};
};
```

```cpp
rmk::script.registerType<Player, Player()>("Player", [](rmk::SolState::Type& ut) {
    ut["move"] = &Player::move;
    ut["hp"]   = &Player::hp;
    ut["pos"]  = &Player::pos;
});
```

- `Player()` lists the constructors exposed to Lua.
- The initializer function binds member functions and fields to Lua-accessible names.

Once registered, the type becomes directly usable from any Lua script sharing the same state:

```lua
local p = Player()
p.hp = 90
p:move(10, 0)
```

### Exposing a C++ variable

No need to register a whole type just to share a simple variable with Lua; loadVar exposes it directly under a given name:

```cpp
rmk::i32 score = 0;
rmk::script.loadVar("score", score);
```

```lua
print(score)
```

### Checking registered types

Combining several scripts and modules risks registering the same type twice; loadedTypes and isLoadedType let you inspect what's already been 
registered before doing it again:

```cpp
if (!rmk::script.isLoadedType("Player")) {
    rmk::script.registerType<Player, Player()>("Player");
}
```

---

[:octicons-arrow-left-24: Previous chapter](script.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](../core/error.md){ .md-button .md-button--primary }