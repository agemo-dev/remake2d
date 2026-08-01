# Random

Hard to imagine a game without some randomness: where the next enemy spawns, what it drops, which shade to give an effect. 
**RE:MAKE 2D** centralizes all of it into one shared generator.

---

## Overview

The `Random` class exposes a singleton `random`, contained in the header **"remake2d/random.hpp"**. It wraps a Mersenne Twister 
engine, seeded automatically at startup using a hardware-based seed when available, so it's ready without any setup.

---

## Methods

The methods of the `Random` class are as follows:

```cpp
u32  seed(void) noexcept;                   // get current seed
void seed(u32) noexcept;                    // set seed
u32  randSeed(void) noexcept;               // generate a hardware random seed
void rollSeed(void) noexcept;               // generate and apply a new random seed
bool chance(f32) noexcept;                  // true with given probability (0.0-1.0)
std::vector<u8> dice(u8 n, u8 faces) noexcept; // roll n dice with given face count

template<IsBasicType T = i32> T rand(const T&, const T&) noexcept; // random value in range
template<typename T> T choice(const std::span<T>&) noexcept;       // random element from span

Color mixColor(f32 saturation = 1.0f, f32 lightness = 0.5f) noexcept; // random vivid color (HSL)
std::string choice(const std::span<std::string_view>&) noexcept;     // random string from span
```

!!! info
    `rand` is a template overloaded for `i32`, `f32`, `Vec2d`, `Dim2d` and `Color`. For `Color`, the range is interpolated 
	through HSL rather than RGB, to produce a coherent hue gradient instead of a meaningless component mix.

---

## Usage

### Seed

Reproducing a run exactly for debugging, or guaranteeing a fresh experience each launch, comes down to the generator's seed:

```cpp
rmk::random.seed(42);   // fixed seed, reproducible sequence
rmk::random.rollSeed(); // random seed, based on hardware entropy
```

### Random numbers

```cpp
rmk::i32 n = rmk::random.rand(0, 10);      // integer in [0, 10]
rmk::f32 f = rmk::random.rand(0.0f, 1.0f); // float in [0.0, 1.0]
```

### Random vectors and dimensions

Spawning an object at a random position within an area, or giving it a variable size, uses the same `rand`, simply applied 
to `Vec2d`/`Dim2d`:

```cpp
rmk::Vec2d pos = rmk::random.rand(rmk::Vec2d{0, 0}, rmk::Vec2d{800, 600});
rmk::Dim2d dim = rmk::random.rand(rmk::Dim2d{10, 10}, rmk::Dim2d{50, 50});
```

### Random colors

```cpp
// interpolated between two colors, through HSL
rmk::Color c1 = rmk::random.rand(rmk::color::red, rmk::color::blue);

// fully random vivid color
rmk::Color c2 = rmk::random.mixColor(1.0f, 0.5f);
```

### Chance and dice

```cpp
if (rmk::random.chance(0.25f)) { /* 25% chance */ }

auto rolls = rmk::random.dice(3, 6); // roll 3 six-sided dice
```

### Choice

```cpp
std::vector<int> nums = {1, 2, 3, 4, 5};
int n = rmk::random.choice<int>(nums);

std::vector<std::string_view> names = {"Alice", "Bob", "Carl"};
std::string name = rmk::random.choice(names);
```

!!! info
    `choice` returns a default-constructed value if the given span is empty.

---

[:octicons-arrow-left-24: Previous chapter](utility.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](scene.md){ .md-button .md-button--primary }