# Installation

RE:MAKE 2D uses **CMake** as its build system. All dependencies are fetched automatically via **CMake FetchContent** if not already installed on your system.

**Requirements :** C++20 — CMake 3.22+

---

## Dependencies

| Library | Version |
|---|---|
| SDL2 | 2.28.5+ |
| SDL2_image | 2.8.2+ |
| SDL2_mixer | 2.8.0+ |
| SDL2_ttf | 2.20.2+ |
| Box2D | 2.4.1+ |

---

## Linux

```bash
git clone https://github.com/agemo-dev/remake2d.git
cd remake2d
mkdir build && cd build
cmake ..
cmake --build .
```

---

## Windows

Make sure **CMake** and a C++20-compatible compiler (MSVC 2022+, MinGW-w64) are installed, then:

```bash
git clone https://github.com/agemo-dev/remake2d.git
cd remake2d
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

---

## macOS

Make sure **Xcode Command Line Tools** and **CMake** are installed, then:

```bash
git clone https://github.com/agemo-dev/remake2d.git
cd remake2d
mkdir build && cd build
cmake ..
cmake --build .
```

---

## Android (Termux)

RE:MAKE 2D natively supports **Termux**. Dependencies can be installed directly via `pkg`:

```bash
pkg update && pkg upgrade -y
pkg install cmake clang sdl2 sdl2-image sdl2-mixer sdl2-ttf
```

Then:

```bash
git clone https://github.com/agemo-dev/remake2d.git
cd remake2d
mkdir build && cd build
cmake ..
cmake --build .
```

!!! info
    Termux is automatically detected by CMake. System packages are used instead of FetchContent in this case.

---

## Options

| Option | Default | Description |
|---|---|---|
| `USE_NATIVE_OPT` | `OFF` | Enable `-march=native` CPU optimizations |
| `CMAKE_BUILD_TYPE` | `Release` | Build type (`Release`, `Debug`) |
| `BUILD_SHARED_LIBS` | `OFF` | Build as shared library instead of static |

```bash
# Enable native CPU optimizations
cmake .. -DUSE_NATIVE_OPT=ON

# Build as shared library
cmake .. -DBUILD_SHARED_LIBS=ON

# Combine multiple options
cmake .. -DUSE_NATIVE_OPT=ON -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Debug
```

---

## compilation

### Compile on command line

```bash
# using clang
clang++ --std=c++20 your_project.cpp -o your_project -lremake2d -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
```

### Compile with CMake

Then add the library to your project's `CMakeLists.txt`:

```cmake
find_package(remake2d REQUIRED)
target_link_libraries(your_project PRIVATE remake2d::remake2d)
```

---

[Next chapter :octicons-arrow-right-24:](../types/numeric.md){ .md-button .md-button--primary }