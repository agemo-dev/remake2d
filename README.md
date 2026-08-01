# RE:MAKE 2D

> *A 2D game engine that puts the developer first.*

**RE:MAKE 2D** is a full code, object-oriented 2D game engine for
C++20. It wraps SDL2, Box2D, and Lua into a single cohesive API, so
that building a game doesn't mean stitching together several
libraries by hand and hoping their setups don't conflict.

The engine covers rendering, physics, sound, scripting, and save
data out of the box, all designed to feel consistent with each other
rather than bolted on as afterthoughts. It targets developers of any
experience level, and is suited both to learning and to shipping
real projects.

---

## 🚀 A few highlights

**RE:MAKE 2D** ships with a declarative signal system driving events
and physics callbacks, a 300+ event manager covering keyboard,
mouse, touch and gamepad input, physics powered by Box2D, and
native Lua scripting with hot-reload support. A handful of examples
below give a feel for the API; the full reference lives in the
[documentation](https://agemo-dev.github.io/remake2d/).

### Signals drive most of the engine

```cpp
rmk::Signal<f32*> onHealthChanged;

onHealthChanged.bind([](void) { last_hp != hp; }, &hp); // auto-emits based on a condition
onHealthChanged.join([](f32 *hp) { std::cout << "HP : " << *hp << "\n"; });

onHealthChanged.emit(&hp); // or emit manually
```

### Physics, powered by Box2D

```cpp
rmk::Rectangle rect(500, {100, 150});
rmk::DynamicBody player(rect);

player.tag("player");
player.mass(10.0f);
player.friction(0.3f);

player.onContact.join([&](rmk::PhysicBody* self, rmk::PhysicBody* other) {
    if (other->tag() == "enemy") hp -= 10.0f;
});
```

### Lua scripting with hot-reload

```lua
-- rock.lua
size     = 45
friction = 0.8
```

```cpp
rmk::Script rock("rock.lua");
rock.onFileChanged.join([&]() {
    rock.update();
    rockShape.resize({rock.get<rmk::f32>("size")});
});
```

There's more to discover beyond these — tilemaps, save data,
coroutines, cameras — all covered in the
[documentation](https://agemo-dev.github.io/remake2d/).

---

## 📦 Installation

```bash
git clone https://github.com/agemo-dev/remake2d.git
cd remake2d
mkdir build && cd build
cmake ..
cmake --build .
cmake --install .
```

**Requirements:** C++20 — CMake 3.32+

All other dependencies are fetched automatically via CMake FetchContent if not already installed.

**Fetched automatically:** SDL2 2.28.11 — SDL_image 2.8.6 — SDL_mixer 2.8.2 — SDL_ttf 2.24.0

---

## 🎮 Quick start

```cpp
#include <remake2d/all/bases.hpp>

int main(void) {

    rmk::Window win;
    rmk::Circle circle(win.center(), 400);

    rmk::loop.execute(win, [&] (void) {
        win.fill(circle, rmk::color::cyan);
    });

    rmk::loop.update();
}
```

---

## 📜 License

MIT — free to use, modify and distribute.