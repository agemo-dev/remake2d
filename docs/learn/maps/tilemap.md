# Tile Map

Building a level tile by tile with manual drawing calls would get unmanageable fast; `TileMap` takes a tileset and a layout, and handles the rest.

---

## Overview

`TileMap` is contained in the header **"remake2d/tilemap.hpp"**. A tile layout is described by a flat array of IDs, applied over a grid whose dimensions
and tileset clipping are described by a `TileMapData` structure:

```cpp
struct TileMapData {
    Vec2d  center;
    Dim2d  size;
    Vec2d  clip_start;
    Dim2d  clip_size;
    Grid2d cut;
    u8     margin{0};
};
```

- `center`     : the map's center position.
- `size`       : the map's total rendered size.
- `clip_start` : top-left position of the first tile in the tileset image.
- `clip_size`  : size of a single tile in the tileset image.
- `cut`        : number of columns and rows of the map.
- `margin`     : spacing between tiles in the tileset image.

---

## Methods

```cpp
Dim2d  clip(void)      const noexcept;   // get tile clip size
Dim2d  size(void)      const noexcept;   // get map size
Vec2d  center(void)    const noexcept;   // get map position
Grid2d cut(void)       const noexcept;   // get grid dimensions
u32    tileCount(void) const noexcept;   // total number of tiles
u32    tileCount(TileID) const noexcept; // count of a specific tile type

void move(Vec2d)        noexcept;   // move map
void resize(Dim2d)      noexcept;   // resize map
void counterStart(i16)  noexcept;   // set starting tile ID offset
void tag(std::string_view, TileID); // assign a tag to a tile ID

void load(TileTemplate);   // load tile layout
void build(void) noexcept; // build physics for tagged tiles (async)

void applyPhysic(TileID);
void applyPhysic(std::string_view);

PhysicBody& body(TileID);
PhysicBody& body(std::string_view);
```

---

## Usage

### Creating a map

The constructor takes the tileset path and a `TileMapData` structure describing its geometry:

```cpp
TileMap(std::string_view tileset_path, TileMapData data);
```

```cpp
rmk::TileMapData data {
	.center     = {400, 300},
	.size       = {800, 600},
	.clip_start = {0, 0},
	.clip_size  = {32, 32},
	.cut        = {10, 8}
}

rmk::TileMap map("tileset.png", data);
```

### Loading a layout

With the geometry in place, all that's left is describing which tile goes where; `load` expects a flat array whose length equals `cut.x * cut.y`:

```cpp
rmk::TileMap::TileTemplate layout = {
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    // ... 5 more rows
};

map.load(layout);
```

### Drawing a map

```cpp
// In render loop
win.draw(map, rmk::color::white);
```

```cpp
#include <remake2d/window.hpp>
#include <remake2d/loop.hpp>
#include <remake2d/tilemap.hpp>

int main(void) {
    rmk::Window win;
    rmk::TileMapData data {
		.center     = win.center(),
		.size       = {800, 600},
		.clip_start = {0, 0},
		.clip_size  = {32, 32},
		.cut        = {10, 8}
	}

    rmk::TileMap map("tileset.png", data);
    map.load(rmk::TileMap::TileTemplate(80, 0));

    rmk::loop.execute(win, [&](void) {
        win.draw(map, rmk::color::white);
    });

    rmk::loop.update();
}
```

### Tags

Hardcoding numeric IDs everywhere in the code gets unreadable fast; a tag gives a type of tile a proper name:

```cpp
map.tag("rock", 1);
map.tag("water", 2);
```

---

## Physics on tiles

A wall or a rock drawn in the tileset doesn't block anything until it has a physics body; applyPhysic turns a given tile type into a real physics
entity, creating a `StaticBody` for every tile of that type.

### Applying physics to a tile type

```cpp
map.tag("rock", 1);
map.applyPhysic("rock"); // also works with map.applyPhysic(1)

map.build(); // builds physics for every tile of that type, asynchronously
```

Build must be called after applyPhysic, and before relying on the resulting bodies; it runs the underlying work asynchronously so it doesn't block 
the frame it's called on.

### Using the resulting body

Once built, every tile matching that ID becomes a solid `StaticBody` automatically placed and moved with the map; body gives a reference to it, 
configurable exactly like a body created by hand, tags and contact callbacks included:

```cpp
rmk::PhysicBody& rock = map.body("rock");

rock.isSolid(true);
rock.onContactStart.join([](rmk::PhysicBody* self, rmk::PhysicBody* other) {
    if (other->tag() == "player") std::cout << "player touched a rock\n";
});
```

This physical body is copied between each instance of the same ID, so any modification made to it must happen before the call to build.

### Counter start

External map editors often reserve ID 0 for "empty tile"; counterStart shifts where the map's own IDs start counting from, to stay consistent
with the imported tileset:

```cpp
map.counterStart(1);
```

---

## Following a map with a camera

A map larger than the window can't be displayed entirely at scale without becoming unreadable; that's where a `Camera` comes in, attached
to the viewport the map is drawn through, exactly like any other draw call. What changes with a linked camera is that `TileMap` automatically
culls and rescales only the tiles currently visible, instead of drawing the whole grid every frame.

```cpp
rmk::Window win;
rmk::Camera cam({0, 0}, win.size(), {2000, 1200}); // world is 2000x1200

win.addViewport("world", { rmk::Area(0, win.size()), cam });

// or with linkCamera after creating the "world" viewport without a camera
// win.linkCamera("world", cam);
```

- The viewport's `Area` defines the screen region the map will render into.
- `cam`'s size should match that screen region, while its third constructor argument (limit) defines the boundaries of the world the camera
- is allowed to scroll within.

Once the camera is linked, the map draws through that viewport, simply by naming it:

```cpp
rmk::loop.execute(win, [&](void) {
    cam.follow(player.shape()); // or any Geometry/PhysicBody/Vec2d
    win.draw(map, rmk::color::white, "world");
});
```

Draw looks up the `"world"` viewport, finds its linked camera, and passes it down to the map's internal draw call, which computes which 
rows and columns fall inside the camera's current view, and only clips, moves and draws those.

!!! info
    If the same viewport is used without a linked camera, `TileMap` falls back to drawing every tile in world space.

---

[:octicons-arrow-left-24: Previous chapter](tilegrid.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](parallax.md){ .md-button .md-button--primary }