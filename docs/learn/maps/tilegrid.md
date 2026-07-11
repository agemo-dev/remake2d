# Tile Grid

Snapping objects to a grid, or just visualizing a regular layout on screen for debugging, doesn't need the full weight of a real 
`TileMap` with its textures and physics.

---

## Overview

`TileGrid` is contained in the header **"remake2d/tilegrid.hpp"**. It only computes and stores the `Area` of each cell in a grid,
given a center, a size, and a number of divisions, without carrying any texture or physics data.

---

## Methods

```cpp
void move(const Vec2d&)   noexcept; // move grid center
void cut(const Grid2d&)   noexcept; // change grid divisions
void resize(const Dim2d&) noexcept; // resize grid

usize  count(void)              const noexcept; // total number of cells
Dim2d  size(void)               const noexcept; // grid size
Grid2d cut(void)                const noexcept; // current grid divisions
Vec2d  center(void)             const noexcept; // grid center
Area   cell(const Grid2d&)      const noexcept; // get a specific cell area by (col, row)
std::vector<Area> cells(void)   const noexcept; // get all cell areas
```

---

## Usage

### Creating a grid

The constructor takes a center, a size, and a `Grid2d` defining the number of columns and rows:

```cpp
Grid2d(const Vec2d&, const Dim2d&, const Grid2d&);
```

```cpp
rmk::TileGrid grid({400, 300}, {800, 600}, {8, 6});
```

### Drawing a grid

Like any `Geometry`, `TileGrid` draws directly through `Window::draw`, handy for visualizing an alignment during development:

```cpp
// In render loop
win.draw(grid, rmk::color::gray);
```

```cpp
#include <remake2d/window.hpp>
#include <remake2d/loop.hpp>
#include <remake2d/tilegrid.hpp>

int main(void) {
    rmk::Window win;
    rmk::TileGrid grid({400, 300}, {800, 600}, {8, 6});

    rmk::loop.execute(win, [&](void) {
        win.draw(grid, rmk::color::gray);
    });

    rmk::loop.update();
}
```

### Retrieving cells

Finding which cell the mouse is in, or iterating the whole grid, works by `(col, row)` coordinate, or by grabbing everything at once:

```cpp
rmk::Area cell = grid.cell({2, 1}); // 3rd column, 2nd row
std::vector<rmk::Area> all = grid.cells();
```

!!! info
    Count returns `cut.x * cut.y`, the total number of cells in the grid.

### Modifying a grid

Move, cut and resize all rebuild the internal cell cache automatically, no manual recalculation needed:

```cpp
grid.move({500, 300});
grid.cut({10, 8});
grid.resize({1000, 700});
```

---

[:octicons-arrow-left-24: Previous chapter](../physic/physic.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](tilemap.md){ .md-button .md-button--primary }

---