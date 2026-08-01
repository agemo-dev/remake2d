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

registerType takes the type to expose, its constructor signatures, an initializer function that binds member functions to Lua-accessible 
names, and an optional list of data fields exposed as real Lua fields:

```cpp
template<typename T, typename... Ctors, typename B = const std::tuple<>, typename... Fields>
void registerType(std::string_view, std::function<void(SolState::Type&)> init = nullptr, B base = rmk::type::base<>, Fields... fields);
```

and utilitaries:

```cpp
template<typename... Bases> inline constexpr auto base;  // maybe a tuple<> or tuple<...>
template <typename... Args> inline auto overload(Args&&... args);
```

```cpp
class Player : public MyBaseClass {
public:
    rmk::Vec2d pos;
    rmk::i32   hp{100};

public:
    Player(void) = default;

public:
    void move(rmk::i32 dx, rmk::i32 dy) { pos.x += dx; pos.y += dy; }
};
```

```cpp
rmk::script.registerType<Player, Player()>("Player", [](rmk::SolState::Type& ut) {
    // simple public method
    ut["move"] = &Player::move;
}, rmk::type::base<MyBaseClass>, // base class (optional)
    "hp",  &Player::hp,          // data fields, passed after base
    "pos", &Player::pos
);
```

- `Player()` lists the constructors exposed to Lua.
- The initializer function binds member **functions** to Lua-accessible names — anything assigned inside it (`ut["x"] = ...`) is always
  callable as a method from Lua (`obj:x()`), even for a plain data member.
- Data **fields** (`hp`, `pos` above) must instead be passed as trailing `"name", &Player::member` pairs, after the base class argument.
  This is what gives them real field access from Lua (`obj.hp`, `obj.hp = value`) instead of turning them into functions.

used `rmk::type::overload` for register overloaded method or with default parameter ; and `rmk::type::base<B>` to add a base class ,
to enable polymorphism to lua interface . Base class must be also register . If there's no base class but fields are needed, pass
`rmk::type::base<>` explicitly to keep the fields in the right argument position.

### Overloading a getter/setter pair with lambdas

A very common case is exposing a single Lua name that reads a value with no argument and writes it with one — this is how `velocity`,
`tag` and others work on the engine's own bodies. Always write it as plain lambdas taking `self` explicitly as the first parameter, one
lambda per arity, passed straight to `rmk::type::overload`:

```cpp
ut["velocity"] = rmk::type::overload(
    [](Player& self)                { return self.velocity(); },
    [](Player& self, rmk::Vec2d v)  { self.velocity(v); }
);
```

```lua
print(p:velocity())          -- getter
p:velocity(rmk.Vec2d(1, 0))  -- setter
```

Keeping `self` explicit and typed (instead of `auto&` or template deduction tricks) is what makes this reliable — this exact shape is
used throughout the engine's own bindings and avoids the overload/deduction ambiguities that other approaches can run into with some compilers.

Once registered, the type becomes directly usable from any Lua script sharing the same state:

```lua
local p = Player()
p.hp = 90
p:move(10, 0)
print("HP  : ", p.hp)
print("POS : ", p.pos)
```

!!! info
	Operator overloads are automatically recorded without manual configuration provided they are declared before the type is recorded in same file or scope .

### Nested namespaces

`registerType`/`_registerEngineType` accept `::`-separated names to nest a type inside a sub-table instead of registering it
at the root. Every segment except the last becomes (or reuses) a plain Lua table; only the last segment holds the type.

```cpp
script._registerEngineType<Bar>("Foo::Bar", [](rmk::SolState::Type& ut) {
    ut["greet"] = &Bar::greet;
});
```

```lua
local b = Foo.Bar()
b:greet()
```

If `Foo` is already registered as its own type or already holds other nested types, it's reused rather than overwritten , nesting
several types under the same namespace-like table is safe.

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

Combining several scripts and modules risks registering the same type twice; `loadedTypes` and `isLoadedType` let you inspect what's already been
registered before doing it again:

```cpp
if (!rmk::script.isLoadedType("Player")) {
    rmk::script.registerType<Player, Player()>("Player");
}
```

---

[:octicons-arrow-left-24: Previous chapter](script.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](lua.md){ .md-button .md-button--primary }
