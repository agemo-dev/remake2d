# Data file

A well-built `Data` is useless as long as it stays in memory; `DataFile` is what actually writes it to disk, 
as a JSON file tied to a simple name, and reads it back on the next launch.

---

## Overview

`DataFile` is contained in the header **"remake2d/data.hpp"**. Each `DataFile` maps a given name to a single 
JSON file stored under `SaveManager`'s root directory, exposed through the singleton `data`. It accepts both 
a raw `Data` and any type implementing `ISavable` directly in its `save`/`load` overloads.

---

## Methods

```cpp
void load(Data&);
void save(const Data&);
void load(ISavable&);
void save(const ISavable&);

void        remove(void)          noexcept;
bool        exist(void)     const noexcept;
std::string path(void)      const noexcept;
std::string name(void)      const noexcept;
```

---

## Usage

### Creating a file

No need to know the full path of a save file ahead of time; the constructor only needs a name, and the actual 
path resolves automatically against `SaveManager`'s root directory:

```cpp
DataFile(std::string_view name);
```

```cpp
rmk::DataFile file("save1"); // resolves to <root>/save1.json
```

### Saving and loading

`save`/`load` accept either a raw `Data` or any type implementing `ISavable` directly, with no distinction at the call site:

```cpp
Player p;
p.name  = "Player 1";
p.score = 1500;

file.save(p);
file.load(p);
```

Save writes the resulting JSON to disk, overwriting any previous content, while load reads the file back and overwrites 
the object passed in.

### Checking and removing a file

Before attempting a load on a save that might not exist yet, exist checks whether the underlying file is present on disk:

```cpp
if (file.exist()) file.load(p);
else file.save(p); // first run, create it
```

Remove deletes the file from disk:

```cpp
file.remove();
```

### Path and name

For logging, or for displaying available saves to the player, path and name return the file's resolved location and its original name:

```cpp
std::string p = file.path(); // e.g. "data/remake2d/json/save1.json"
std::string n = file.name(); // "save1"
```

### Save location

Every `DataFile` resolves its path against `SaveManager`'s root directory, exposed through the singleton `data`, 
which defaults to `"data/remake2d/json/"`, relative to the executable:

```cpp
rmk::data.root("saves/");

std::string root = rmk::data.root();
```

Changing the root only affects `DataFile` instances created afterwards.

---

[:octicons-arrow-left-24: Previous chapter](data.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](isavable.md){ .md-button .md-button--primary }