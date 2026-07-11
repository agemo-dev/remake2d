# Data

Saving a game's state means serializing numbers, positions, strings, and sometimes nested structures, 
without needing a different format for each. `Data` unifies all of it into one generic container.

---

## Overview

`Data` is contained in the header **"remake2d/data.hpp"**. It's a variant type able to hold any of the 
engine's base types, a nested list of `Data`, or a nested map of named `Data`, making it the common format 
used to save and load information through `DataFile`.

---

## Methods

```cpp
const Data& operator[](std::string_view key) const;
const Data& operator[](usize idx) const;
template<typename T> T get(std::string_view key) const;
template<typename T> operator T(void) const;
static Data map(std::span<std::pair<const std::string, Data>>);
static Data list(std::span<Data>);
```

---

## Usage

### Building a `Data`

`Data` wraps any single value directly through its constructors, accepting `byte`, `rune`, `imax`, `fmax`, 
`bool`, `Vec2d`, `Fact2d`, `Grid2d`, `Dim2d`, `Color`, `Area`, or a string:

```cpp
Data(byte); Data(rune); Data(imax); Data(fmax); Data(bool);
Data(Vec2d); Data(Fact2d); Data(Grid2d); Data(Dim2d); Data(Color); Data(Area);
Data(std::string_view);
```

```cpp
rmk::Data score(1500);
rmk::Data name("Player 1");
rmk::Data pos(rmk::Vec2d{100, 200});
```

An inventory or a list of scores is built with `Data::list`:

```cpp
std::vector<rmk::Data> inventory = { rmk::Data("sword"), rmk::Data("shield") };
rmk::Data inv = rmk::Data::list(inventory);
```

And to group several fields under names, `Data::map` takes `std::pair<const std::string, Data>` entries:

```cpp
std::pair<const std::string, rmk::Data> fields[] = {
    {"name", rmk::Data("Player 1")},
    {"score", rmk::Data(1500)},
    {"position", rmk::Data(rmk::Vec2d{100, 200})}
};

rmk::Data player = rmk::Data::map(fields);
```

Since every field is itself a `Data`, maps and lists nest freely, allowing a full save to be represented
as one single structure:

```cpp
std::pair<const std::string, rmk::Data> save[] = {
    {"player", player},
    {"inventory", inv}
};

rmk::Data root = rmk::Data::map(save);
```

### Reading a `Data`

Fields are read with `operator[]` for a map, or `get<T>` for a typed value directly:

```cpp
rmk::i32   score = player["score"];
rmk::Vec2d pos   = player.get<rmk::Vec2d>("position");
```

`operator T` allows implicit conversion when the target type is unambiguous, letting `operator[]` chain 
naturally into a typed read without going through get:

```cpp
std::string playerName = player["name"];
```

List elements are read the same way, by index:

```cpp
rmk::Data first = inv[0];
```

!!! info
    `Data`'s numeric constructors accept `byte`, `rune`, `imax` and `fmax`; smaller integer or floating-point 
	types are implicitly widened into one of these.

---

[:octicons-arrow-left-24: Previous chapter](../tools/scene.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](datafile.md){ .md-button .md-button--primary }

---