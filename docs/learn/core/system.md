# System

The system module allows modifying the engine's internal behavior in order to optimize its interaction with the host platform and extend its capabilities.

---

## Overview

The `system` singleton of type `System` is the central tool for controlling the engine at a low level. It is organized around three sub-classes:

- `Setup` — engine behavior configuration at startup
- `Toggle` — enabling or disabling system-level features
- `Info` — access to host platform information

---

## Setup

The `Setup` sub-class, accessible via `system.setup`, allows configuring how **RE:MAKE 2D** interacts with the host system.

### Methods

```cpp
struct Setup {
    void scalingMode(std::string_view);                  // set render scaling quality
    void backend(std::string_view);                      // set render backend
    void audioMode(std::string_view);                    // set audio resampling mode
    void audioCategory(std::string_view);                // set audio category
    void mouseRelativeMode(std::string_view);            // set mouse relative mode
    void mobileOrientation(std::span<std::string_view>); // set allowed orientations
    void allocateChannels(u16);                          // set number of audio channels
};
```

Each of these methods comes with predefined values suited to the most common use cases.

### Scaling mode

The scaling mode defines the interpolation method used when images are resized relative to their original dimensions.
Three modes are available:

```cpp
namespace scaling {
    inline constexpr const char *nearest     = "0"; // no interpolation
    inline constexpr const char *linear      = "1"; // bilinear filtering
    inline constexpr const char *anisotropic = "2"; // anisotropic filtering
}
```

### Backend

The `backend` method allows selecting the rendering engine to use, depending on its availability on the target platform:

```cpp
namespace backend {
    inline constexpr const char *opengl   = "opengl";
    inline constexpr const char *opengles = "opengles2";
    inline constexpr const char *direct3d = "direct3D";
    inline constexpr const char *vulkan   = "vulkan";
    inline constexpr const char *metal    = "metal";
    inline constexpr const char *software = "software";
}
```

### Audio mode

The audio mode determines the **resampling quality** applied during sound playback:

```cpp
namespace audiomode {
    inline constexpr const char *fast   = "fast";
    inline constexpr const char *medium = "medium";
    inline constexpr const char *best   = "best";
}
```

### Audio category

The audio category informs the operating system about the nature of the sounds produced by the application, allowing the OS to apply platform-specific optimizations:

```cpp
namespace audiocategory {
    inline constexpr const char *media = "media";
    inline constexpr const char *voice = "voice";
    inline constexpr const char *alarm = "alarm";
}
```

### Mobile orientation

This property, specific to mobile platforms, defines the screen orientations supported by the application:

```cpp
namespace orientation {
    inline constexpr const char *normal    = "Portrait";
    inline constexpr const char *reverse   = "PortraitUpSideDown";
    inline constexpr const char *right     = "LandscapeRight";
    inline constexpr const char *left      = "LandscapeLeft";
    inline constexpr const char *portrait  = "Portrait PortraitUpSideDown";
    inline constexpr const char *landscape = "LandscapeRight LandscapeLeft";
    inline constexpr const char *any       = "Portrait PortraitUpSideDown LandscapeRight LandscapeLeft";
}
```

By default, the orientation is set to `normal` (portrait).

### Channels

The `allocateChannels` method defines the number of audio channels allocated for sound effect playback. This value ranges from `32` to `256`, as defined in
the `channel` enumeration:

```cpp
enum class channel : u16 {
    min = 32,
    mid = 128,
    max = 256
};
```

By default, **RE:MAKE 2D** allocates **128 channels**.

---

## Toggle

The `Toggle` sub-class, accessible via `system.toggle`, allows enabling or disabling specific features of the host system.

### Methods

```cpp
struct Toggle {
    void vsync(bool);                   // enable/disable vertical synchronization
    void relativeMouseMode(bool);       // enable/disable relative mouse mode
    void gameController(bool);          // enable/disable game controller support
    void blockOnPause(bool);            // enable/disable blocking on pause (Android)
    void hideHomeIndicator(bool);       // enable/disable home indicator (iOS)
    void materialAcceleration(bool);    // enable/disable hardware acceleration
    void accelerometerAsJoystick(bool); // enable/disable accelerometer as joystick (mobile)
};
```

### VSync

Enables or disables **vertical synchronization**, which caps the frame rate to the screen's refresh rate in order to prevent visual tearing. Enabled by default.

### Relative mouse mode

In relative mode, mouse coordinates are no longer expressed as absolute screen positions, but as **relative movement** since the last frame.
This mode is particularly suited to first-person games or any context requiring smooth camera control. Two sub-modes are available:

```cpp
namespace relmode {
    inline constexpr const char *raw  = "0"; // raw input, no acceleration
    inline constexpr const char *warp = "1"; // cursor warping
}
```

### Game controller

Enables or disables **game controller support**, including SDL2 joystick, haptic feedback, and game controller subsystems.

### Block on pause

Specific to **Android**, this option blocks application execution when it moves to the background, in accordance with the Android application lifecycle.

### Hide home indicator

Specific to **iOS**, this option hides the home indicator to provide a full-screen experience free of visual interruption.

### Material acceleration

Enables or disables **hardware framebuffer acceleration**. When active, rendering is delegated to the GPU, improving overall performance.

### Accelerometer as joystick

Specific to **mobile platforms**, this option allows the device's **accelerometer** to be used as a joystick,
providing an alternative control scheme without an external peripheral.

---

## Info

The `Info` sub-class, accessible via `system.info`, exposes information about the host platform.

### Methods

```cpp
struct Info {
    std::string platform(void);     // host platform name
    u8    cpuCount(void);           // number of logical CPU cores
    u32   ramMB(void);              // total system RAM in megabytes
    u8    displayCount(void);       // number of connected displays
    u16   channelCount(void);       // number of allocated audio channels
    Dim2d screenSize(u8 = 0);       // screen resolution of the given display (0 = main screen, 1 = secondary screen, n = nth screen)
    Date  currentDay(void);         // current date
    Clock currentTime(void);        // current time
};
```

This information is accessible at any point after system initialization, and can be useful for adapting game
behavior to the hardware configuration of the host device.

---

## System methods

The `System` class exposes only two methods, dedicated to initializing the engine's dependencies and the engine itself:

```cpp
void init(void);    // initialize all dependencies
bool isInit(void);  // check whether the engine has been initialized
```

Calling `init` is mandatory before using any module of **RE:MAKE 2D**. It becomes optional if a **window** is created first, as the window
takes care of calling it automatically.

!!! warning
    All methods belonging to the `Setup` and `Toggle` sub-classes must be called **before** `init`. Any call made after this point will be ignored.

!!! info
    Some methods of the `Setup` and `Toggle` sub-classes may have no effect depending on the host system, as their behavior is contingent
    on the features available on the target platform.
	
---

[:octicons-arrow-left-24: Previous chapter](croutine.md){ .md-button }