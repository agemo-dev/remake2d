# Savable

Rebuilding a `Data` by hand for every object to save gets repetitive fast; `Savable` lets that conversion be described once per type, so `DataFile` can then save or load it directly.

---

## Overview

`Savable` is contained in the header **"remake2d/data.hpp"**. It's a trait any custom type can implement, requiring only two methods: one to turn the object into a `Data`, the other to read it back from one.

---

## Methods

```cpp
virtual Data sdata(void) const  = 0; // serializes the object into a Data
virtual void ldata(const Data&) = 0; // restores the object's state from a Data
```

---

## Usage

### Implementing `Savable`

A type becomes savable by inheriting from `Savable` and implementing both methods, mapping each field to and from a named entry in a `Data` map:

```cpp
class Player : public rmk::Savable {
public:
    std::string name;
    rmk::i32     score{0};
    rmk::Vec2d   pos;

public:
    rmk::Data sdata(void) const override {
        std::pair<const std::string, rmk::Data> fields[] = {
            {"name", rmk::Data(name)},
            {"score", rmk::Data(score)},
            {"position", rmk::Data(pos)}
        };
        return rmk::Data::map(fields);
    }

    void ldata(const rmk::Data& d) override {
        name  = d.get<std::string>("name");
        score = d.get<rmk::imax>("score");
        pos   = d.get<rmk::Vec2d>("position");
    }
};
```

### Saving and loading

Once `Savable` is implemented, the object passes directly to a `DataFile`, without ever building or reading a `Data` by hand at the call site:

```cpp
rmk::DataFile file("save1");

Player p;
p.name  = "Player 1";
p.score = 1500;

file.save(p);
file.load(p);
```

The same object can also be streamed with `<<`/`>>` (in header **"remake2d/stream.hpp"**), behaving exactly like save/load:

```cpp
file << p;
file >> p;
```

!!! info
    Nested savable objects work naturally, since `sdata` can embed another object's own `sdata` result as one of its fields.

---

## Property

| Type | Copiable | Movable | Bases and Traits |
|---|---|---|---|
| Savable | Yes | Yes | None |

---

[:octicons-arrow-left-24: Previous chapter](datafile.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](script.md){ .md-button .md-button--primary }