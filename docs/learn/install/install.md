# Installation

RE:MAKE 2D uses **CMake** as its build system. All dependencies are fetched automatically via **CMake FetchContent** if not already installed on your system.

**Requirements :** C/C++ compiler — CMake 3.32+

---

## Dependencies

| Library | Version |
|---|---|
| SDL2 | 2.30.11 |
| SDL2_image | 2.8.6 |
| SDL2_mixer | 2.8.2 |
| SDL2_ttf | 2.24.0 |

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

RE:MAKE 2D natively supports **Termux**. Dependencies can be installed directly via `pkg`automatically .

Then:

```bash
git clone https://github.com/agemo-dev/remake2d.git
cd remake2d
mkdir build && cd build
cmake ..
cmake --build .
cmake --install .
```

!!! info
    Termux is automatically detected by CMake. System packages are used instead of FetchContent in this case.

---

## Options

| Option | Default | Description |
|---|---|---|
| `BUILD_SHARED_LIBS` | `ON` | Build as shared library instead of static |
| `USE_NATIVE_OPT` | `OFF` | Enable `-march=native` CPU optimizations |
| `CMAKE_BUILD_TYPE` | `Release` | Build type (`Release`, `Debug`) |
| `BUILD_REMAKE_SCRIPT` | `ON` | Build the `remakeScript` Lua scripting add-on |

```bash
# Build as static library
cmake .. -DBUILD_SHARED_LIBS=OFF

# Enable native CPU optimizations
cmake .. -DUSE_NATIVE_OPT=ON

# Disable an add-on
cmake .. -DBUILD_REMAKE_SCRIPT=OFF

# Combine multiple options
cmake .. -DUSE_NATIVE_OPT=ON -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Debug
```

!!! info
    Disabling an add-on with `BUILD_REMAKE_SCRIPT=OFF` prevents it from being compiled entirely. If you only want to skip *linking* an add-on in your own project (while still having it available), see [Components](#components) below instead.

---

## Compilation

### Compile with CMake

Add the library to your project's `CMakeLists.txt`:

```cmake
find_package(remake2d REQUIRED)
target_link_libraries(your_project PRIVATE remake2d::remake2d)
```

---

## Components

The RE:MAKE 2D add-ons are optional **components**. They must have been built (see [Options](#options) above) to be available, but linking them into your own project is a separate, explicit choice made via `COMPONENTS`:

```cmake
# Link remake2d only
find_package(remake2d REQUIRED)

# Link remake2d + remakeScript
find_package(remake2d REQUIRED COMPONENTS Script)

# Link every add-on that was built
find_package(remake2d REQUIRED COMPONENTS All)

target_link_libraries(your_project PRIVATE remake2d::remake2d)
```

Requesting a component that was not built (e.g. `BUILD_REMAKE_SCRIPT=OFF` was used) makes `find_package` fail with a clear error, unless you use `COMPONENTS All`, which silently links whatever is available.

!!! info
	Activating a component automatically creates a macro in its name (REMAKE2D_ADDON_{Component_Name}):

```cpp
#ifdef REMAKE2D_ADDON_SCRIPT // check if remakeScript is linked
//...
#endif
```

---

[Next chapter :octicons-arrow-right-24:](../types/numeric.md){ .md-button .md-button--primary }