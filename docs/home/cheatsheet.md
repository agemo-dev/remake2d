# Cheat Sheet

---

This page lists every **public class** and their methods in **RE:MAKE 2D**.

!!! info
    **Private** engine-internal classes and methods are not listed here.

---

## Window

```cpp
Window(std::string_view title, Vec2d pos, Dim2d size); // create a new window
u32      ID(void) const noexcept;           // get the unique window identifier
Vec2d    pos(void) const noexcept;          // get window position (upper-left corner)
Dim2d    size(void) const noexcept;         // get window dimensions
Vec2d    center(void) const noexcept;       // get window center coordinates
void     move(Vec2d) noexcept;              // move window to a new position
void     resize(Dim2d) noexcept;            // resize window
void     maxSize(Dim2d) noexcept;           // set maximum window size
void     rename(std::string_view) noexcept; // change window title
void     icon(std::string_view);            // set window icon from an image file
void     resizable(bool) noexcept;          // allow or forbid user resizing
void     fullScreen(bool) noexcept;         // toggle fullscreen mode
void     border(bool) noexcept;             // show or hide window decorations
void     close(void) noexcept;              // close and destroy the window
bool     isOpen(void) const noexcept;       // check if window is still open
bool     isFocus(void) const noexcept;      // check if window has input focus
std::string title(void) noexcept;           // get current window title
void     blendMode(window::blendmode) noexcept; // set SDL blend mode

void     clear(Color = rmk::color::black, std::string_view viewport = "") noexcept; // clear window with a solid color

void     draw(const TextureBase&, Color = rmk::color::white, std::string_view viewport = "") noexcept; // draw a texture
void     draw(const TileMap&,     Color = rmk::color::white, std::string_view viewport = "") noexcept; // draw a tile map
void     draw(const Parallax&,    Color = rmk::color::white, std::string_view viewport = "") noexcept; // draw a parallax background
void     draw(const Area&,        Color = rmk::color::white, std::string_view viewport = "") noexcept; // draw rectangle outline
void     draw(const TileGrid&,    Color = rmk::color::white, std::string_view viewport = "") noexcept; // draw tile grid outline
void     draw(const Geometry&,    Color = rmk::color::white, std::string_view viewport = "") noexcept; // draw shape outline
void     draw(const PhysicBody&,  Color = rmk::color::white, std::string_view viewport = "") noexcept; // draw physics body outline

void     fill(const Area&,        Color = rmk::color::white, std::string_view viewport = "") noexcept; // draw filled rectangle
void     fill(const Geometry&,    Color = rmk::color::white, std::string_view viewport = "") noexcept; // draw filled shape
void     fill(const PhysicBody&,  Color = rmk::color::white, std::string_view viewport = "") noexcept; // draw filled physics body

void     addViewport(std::string_view, Viewport) noexcept;  // register a named viewport
void     linkCamera(std::string_view, Camera&) noexcept;    // attach a camera to a viewport
void     unlinkCamera(std::string_view) noexcept;           // detach camera from a viewport
void     removeViewport(std::string_view) noexcept;         // delete a named viewport
void     useViewport(std::string_view) noexcept;            // activate a viewport for subsequent drawing
void     resetViewport(void) noexcept;                      // restore the default full-window viewport
```

---

## Color / Vec2d / Dim2d / Fact2d / Grid2d / Area

```cpp
// Color
Color(byte R, byte G, byte B, byte A = 255); // construct from RGBA components
auto operator<=>(const Color&) const noexcept = default; // full comparison support

// Named colors inside rmk::color::
red, green, blue, skyblue, purple, orange, yellow, gray, gold,
darkblue, darkgreen, lime, silver, maroon, pink, fuchsia, aqua,
raywhite, cyan, emerald, teal, amber, indigo, black, white

// Free operators
Color operator+(const Color&, const Color&) noexcept; // component-wise addition (clamped)
Color operator-(const Color&, const Color&) noexcept; // component-wise subtraction (clamped)
Color operator%(const Color&, const Color&) noexcept; // component-wise average
```

```cpp
// Vec2d
Vec2d(f32 X, f32 Y);   // construct from X and Y
Vec2d(f32 XY);         // construct with same value for both axes
operator Fact2d(void);          // implicit conversion (negative components clamped to 0)
operator Grid2d(void);          // implicit conversion (cast to usize)
auto operator<=>(const Vec2d&) const noexcept = default; // full comparison
Vec2d operator+(const Vec2d&, const Vec2d&) noexcept; // addition
Vec2d operator-(const Vec2d&, const Vec2d&) noexcept; // subtraction
Vec2d operator*(const Vec2d&, const Vec2d&) noexcept; // multiplication
Vec2d operator/(const Vec2d&, const Vec2d&) noexcept; // division
Vec2d operator%(const Vec2d&, const Vec2d&) noexcept; // modulo (fmod)
```

```cpp
// Dim2d
Dim2d(f32 W, f32 H);   // construct from width and height
Dim2d(f32 WH);         // construct with same value for both axes
auto operator<=>(const Dim2d&) const noexcept = default; // full comparison
Dim2d operator+(const Dim2d&, const Dim2d&) noexcept; // addition
Dim2d operator-(const Dim2d&, const Dim2d&) noexcept; // subtraction
Dim2d operator*(const Dim2d&, const Dim2d&) noexcept; // multiplication
Dim2d operator/(const Dim2d&, const Dim2d&) noexcept; // division
Dim2d operator%(const Dim2d&, const Dim2d&) noexcept; // modulo (fmod)
```

```cpp
// Fact2d — scaling factor, now f32-based (was u32)
Fact2d(f32 X, f32 Y);  // construct from X and Y — negative values clamped to 0 at construction
Fact2d(f32 XY);        // construct with same value for both axes — negative clamped to 0
operator Vec2d(void);           // implicit conversion
auto operator<=>(const Fact2d&) const noexcept = default; // full comparison
Fact2d operator+(const Fact2d&, const Fact2d&) noexcept; // addition
Fact2d operator-(const Fact2d&, const Fact2d&) noexcept; // subtraction
Fact2d operator*(const Fact2d&, const Fact2d&) noexcept; // multiplication
Fact2d operator/(const Fact2d&, const Fact2d&) noexcept; // division
Fact2d operator%(const Fact2d&, const Fact2d&) noexcept; // modulo
```

```cpp
// Grid2d — id/index roles (usize-based)
Grid2d(usize X, usize Y); // construct from X and Y
Grid2d(usize XY);         // construct with same value for both axes
operator Vec2d(void);              // implicit conversion
operator SDL_Point(void);          // implicit conversion
auto operator<=>(const Grid2d&) const noexcept = default; // full comparison
Grid2d operator+(const Grid2d&, const Grid2d&) noexcept; // addition
Grid2d operator-(const Grid2d&, const Grid2d&) noexcept; // subtraction
Grid2d operator*(const Grid2d&, const Grid2d&) noexcept; // multiplication
Grid2d operator/(const Grid2d&, const Grid2d&) noexcept; // division
Grid2d operator%(const Grid2d&, const Grid2d&) noexcept; // modulo
```

!!! info
    `Vec2d` converts implicitly to both `Fact2d` and `Grid2d`, so a `Vec2d` literal can be passed anywhere either type is expected.

```cpp
// Area
Area(i32 x, i32 y, i32 w, i32 h); // construct from position and size
Area(const Vec2d& pos, const Dim2d& size); // construct from position + dimension
Vec2d center(void) const noexcept; // get area center
auto operator<=>(const Area&) const noexcept = default; // full comparison
Area operator+(const Area&, const Area&) noexcept; // addition
Area operator-(const Area&, const Area&) noexcept; // subtraction
Area operator*(const Area&, const Area&) noexcept; // multiplication
Area operator/(const Area&, const Area&) noexcept; // division
Area operator%(const Area&, const Area&) noexcept; // modulo
```

---

## Geometry (abstract class)

```cpp
u8                     points(void) const noexcept;           // number of vertices
Dim2d                  size(void) const noexcept;             // bounding box size
Vec2d                  center(void) const noexcept;           // center position
const Vec2d*           pointsPos(void) const noexcept;        // raw vertex array
template<IsShape S> S  as(void) const noexcept;               // cast to another shape type
virtual void           move(const Vec2d&) noexcept = 0;       // translate shape
virtual void           rotate(f32) noexcept = 0;              // rotate shape (radians)
virtual void           scale(const Fact2d&) noexcept = 0;     // scale shape
virtual void           resize(const Dim2d&) noexcept = 0;     // set new bounding box size
virtual void           transform(const Vec2d&, f32, const Fact2d&) noexcept = 0; // translate + rotate + scale
virtual bool           hasIntersected(const Geometry&) const noexcept = 0; // collision test (SAT)
```

---

## Shape

```cpp
Shape(const Vec2d& center, const Dim2d& size); // construct a regular polygon
// All Geometry methods above are implemented here.
// Triangulation for filled rendering is generated automatically.
```

Aliases

```cpp
using Point     = Shape<1>;   // single point
using Line      = Shape<2>;   // line segment
using Triangle  = Shape<3>;   // triangle
using Losange   = Shape<4>;   // diamond shape
using Hexagone  = Shape<6>;   // hexagon
using Ellipse   = Shape<36>;  // ellipse approximation

class Rectangle : public Shape<4>; // axis-aligned rectangle (optimized build)
class Square    : public Rectangle; // uniform rectangle
class Circle    : public Ellipse; // circle (w == h enforced)
```

---

## Signal

```cpp
// Connection management
void connect(void (*func)(Args...));             // connect a free function (persistent)
void disconnect(void (*func)(Args...));          // disconnect a free function
void connectOnce(void (*func)(Args...));         // connect a free function (one shot)
void connect(Slot& slot);                        // connect a std::function by reference
void disconnect(Slot& slot);                     // disconnect a std::function
void connectOnce(Slot& slot);                    // connect a std::function (one shot)
void join(Slot&& slot);                          // move-connect a std::function (persistent)
void joinOnce(Slot&& slot);                      // move-connect a std::function (one shot)
void joinPriority(Slot&& slot);                  // move-connect with priority execution order
void connectPriority(void (*func)(Args...));     // connect a free function with priority
void disconnectPriority(void (*func)(Args...));  // disconnect a priority free function
void connectPriority(Slot& slot);                // connect a std::function with priority
void disconnectPriority(Slot& slot);             // disconnect a priority std::function
void unablePriority(void (*func)(Args...));      // promote a normal function to priority
void unablePriority(Slot& slot);                 // promote a normal slot to priority
void disablePriority(void (*func)(Args...));     // demote a priority function to normal
void disablePriority(Slot& slot);                // demote a priority slot to normal

// Emission
void emit(Args... args);                         // manually trigger all connected callbacks

// Automatic emission (condition-based)
void bind(Condition cond, Args... defaultArgs);        // emit every frame while condition is true
void bindRising(Condition cond, Args... defaultArgs);  // emit once when condition becomes true
void bindFalling(Condition cond, Args... defaultArgs); // emit once when condition becomes false
void bindChange(Condition cond, Args... defaultArgs);  // emit on every condition change

// Lifecycle
void start(void);        // activate the signal
void stop(void);         // deactivate the signal
int  count(void) const;  // number of connected slots
void reserve(int size);  // pre-allocate internal storage
```

---

## EventManager (singleton event)

```cpp
void poll(void);                  // process all pending events (non-blocking)
void wait(void);                  // wait indefinitely for an event
void wait(time::Second);          // wait for an event with timeout
```

### Keyboard

```cpp
_EventSignal<> onPressA, onPressB, ..., onPressZ;          // key pressed (keycode)
_EventSignal<> onPress0, ..., onPress9;                    // number keys pressed
_EventSignal<> onPressSpace, onPressEnter, onPressEscape;  // special keys pressed
_EventSignal<> onPressBackspace, onPressTab, onPressCapsLock;
_EventSignal<> onPressDelete, onPressInsert, onPressHome;
_EventSignal<> onPressEnd, onPressPageUp, onPressPageDown;
_EventSignal<> onPressLeft, onPressRight, onPressUp, onPressDown; // arrow keys
_EventSignal<> onPressLShift, onPressRShift;               // shift keys
_EventSignal<> onPressLCtrl, onPressRCtrl;                 // control keys
_EventSignal<> onPressLAlt, onPressRAlt;                   // alt keys
_EventSignal<> onPressAny;                                 // any key pressed
_EventSignal<> onPressF1 .. onPressF12;                    // function keys

// Equivalent onRelease* events for key releases
// Equivalent onPressScan*, onReleaseScan* events (scancode-based, layout-independent)
```

### Text input

```cpp
_EventSignal<std::string> onTextInput;   // UTF-8 text input
_EventSignal<std::string> onTextEdit;    // composition / IME editing
```

### Mouse

```cpp
_EventSignal<Vec2d> onMouseMove;                    // mouse position
_EventSignal<Vec2d> onMouseRawMove;                 // relative mouse motion
_EventSignal<Vec2d> onLeftDown, onLeftUp;            // left button
_EventSignal<Vec2d> onRightDown, onRightUp;          // right button
_EventSignal<Vec2d> onMiddleDown, onMiddleUp;        // middle button
_EventSignal<Vec2d> onDoubleClick;                   // double click
_EventSignal<Vec2d> onWheel;                         // mouse wheel (x, y)
```

### Gamepad

```cpp
_EventSignal<i32> onPressACtrl, onPressBCtrl, onPressXCtrl, onPressYCtrl;
_EventSignal<i32> onPressStart, onPressSelect;
_EventSignal<i32> onPressLShoulder, onPressRShoulder;
_EventSignal<i32> onPressDpadLeft, onPressDpadRight, onPressDpadUp, onPressDpadDown;
_EventSignal<i32> onPressLStick, onPressRStick;
// Equivalent onRelease* events
_EventSignal<i32, i16> onAxisLeftX, onAxisLeftY;       // left stick
_EventSignal<i32, i16> onAxisRightX, onAxisRightY;     // right stick
_EventSignal<i32, i16> onAxisLTrigger, onAxisRTrigger; // triggers
_EventSignal<i32> onControllerAdded;                   // controller plugged in
_EventSignal<i32> onControllerRemoved;                 // controller unplugged
```

### Touch

```cpp
_EventSignal<Vec2d> onFingerDown;     // finger touched screen
_EventSignal<Vec2d> onFingerUp;       // finger lifted
_EventSignal<Vec2d> onFingerMove;     // finger moved
_EventSignal<Vec2d> onMultiGesture;   // multi-touch gesture
```

### Window events

```cpp
_EventSignal<u32>        onWindowClose;        // window close requested
_EventSignal<u32>        onWindowMinimized;    // window minimized
_EventSignal<u32>        onWindowMaximized;    // window maximized
_EventSignal<u32>        onWindowRestored;     // window restored
_EventSignal<u32>        onWindowShown;        // window shown
_EventSignal<u32>        onWindowHidden;       // window hidden
_EventSignal<u32>        onWindowExposed;      // window exposed
_EventSignal<u32>        onWindowFocusGained;  // window gained focus
_EventSignal<u32>        onWindowFocusLost;    // window lost focus
_EventSignal<u32>        onWindowMouseEnter;   // mouse entered window
_EventSignal<u32>        onWindowMouseLeave;   // mouse left window
_EventSignal<u32, Dim2d> onWindowResized;      // window resized
_EventSignal<u32, Vec2d> onWindowMoved;        // window moved
```

### Application

```cpp
_EventSignal<>            onQuit;             // application quit requested
_EventSignal<std::string> onDropFile;         // file dropped onto window
_EventSignal<>            onClipboardUpdate;  // clipboard changed
_EventSignal<>            onAppForeground;    // app entering foreground
_EventSignal<>            onAppBackground;    // app entering background
_EventSignal<>            onAppLowMemory;     // low memory warning
_EventSignal<>            onLocaleChanged;    // system locale changed
_EventSignal<>            onDisplayChanged;   // display configuration changed
_EventSignal<>            onRenderReset;      // render device reset
_EventSignal<>            onEventNone;        // no event occurred (idle)
```

---

## Chronometer

```cpp
void start(void) noexcept;   // start or restart the chronometer
void stop(void) noexcept;    // stop and record final time
void pause(void) noexcept;   // pause without resetting
void resume(void) noexcept;  // resume after pause
void reset(void) noexcept;   // reset all values to zero
template <typename D = time::Second>
fmax elapsedTime(void) noexcept; // get elapsed time in chosen unit
```

---

## DeltaTime (singleton delta)

```cpp
fmax tick(void) const noexcept;    // delta time of current frame (seconds)
fmax FPS(void) const noexcept;     // current frames per second
fmax maxFPS(void) const noexcept;  // frame rate cap
void maxFPS(fmax) noexcept;        // set frame rate cap (clamped 30–1440)
void update(void);                 // advance delta and dispatch signals (called by loop)
```

---

## Timer / TimerManager (singleton timer)

```cpp
// Time unit
namespace time {

using Nanosecond  = std::chrono::duration<fmax, std::nano>;
using Microsecond = std::chrono::duration<fmax, std::micro>;
using Millisecond = std::chrono::duration<fmax, std::milli>;
using Second      = std::chrono::duration<fmax, std::ratio<1>>;
using Minute      = std::chrono::duration<fmax, std::ratio<60>>;
using Hour        = std::chrono::duration<fmax, std::ratio<3600>>;
using Day         = std::chrono::duration<fmax, std::ratio<86400>>;
using Week        = std::chrono::duration<fmax, std::ratio<604800>>;
using Month       = std::chrono::duration<fmax, std::ratio<2629800>>;   // 30.44 days
using Year        = std::chrono::duration<fmax, std::ratio<31557600>>;  // 365.25 days

void delay(time::Second) noexcept; // sleep for any time
}

// Timer
Timer(void);
Timer(fmax limit);
Timer(time::Second limit);

void limit(fmax) noexcept;             // set limit (seconds)
void limit(time::Second) noexcept;     // set limit
fmax limit(void) const noexcept;       // get limit

void start(void) noexcept;    // start / restart the timer
void stop(void) noexcept;     // stop and reset
void pause(void) noexcept;    // pause without resetting
void resume(void) noexcept;   // resume after pause
void repeat(bool) noexcept;   // auto-restart on timeout

bool isActive(void)  const noexcept;   // check if currently running
bool isElapsed(void) const noexcept;   // check if timeout occurred
fmax elapsedTime(void) const noexcept; // current elapsed time

_TimerSignal<> onTimeout; // emitted once when the timer elapses
```

---

## Date

```cpp
Date(u8 day, u8 month, i32 year); // construct a date
u8   day(void) const noexcept;    // get day
u8   month(void) const noexcept;  // get month
i32  year(void) const noexcept;   // get year
char slice(void) const noexcept;  // get current separator character
void day(u8) noexcept;            // set day
void month(u8) noexcept;          // set month
void year(i32) noexcept;          // set year
void format(const std::string_view& fmt);   // set display format (e.g. "day/month/year")
void date(const std::string_view& str);     // parse a date string
std::string date(void) const;              // format date to string
bool dayIs(const Date& other) const noexcept; // compare with another date
// Operators ==, !=, <, >, <=, >=
```

---

## Clock

```cpp
Clock(u8 hour, u8 minute, u8 second); // construct a time
u8   hour(void) const noexcept;       // get hour
u8   minute(void) const noexcept;     // get minute
u8   second(void) const noexcept;     // get second
char slice(void) const noexcept;      // get current separator
void hour(const u8&) noexcept;        // set hour
void minute(const u8&) noexcept;      // set minute
void second(const u8&) noexcept;      // set second
void format(const std::string_view& fmt); // set display format (e.g. "hour:minute:second")
void time(const std::string_view& str);   // parse a time string
std::string time(void) const;             // format time to string
bool timeIs(const Clock& other) const noexcept; // compare with another time
// Operators ==, !=, <, >, <=, >=
```

---

## System (singleton system)

```cpp
void init(void);    // initialize SDL and all subsystems (must be called before Window)
bool isInit(void);  // check if system is initialized

struct Info {
    u32         ramMB(void);           // system RAM in megabytes
    u8          cpuCount(void);        // number of logical CPU cores
    std::string platform(void);        // platform name
    u8          displayCount(void);    // number of connected displays
    u16         channelCount(void);    // number of allocated audio channels
    Dim2d       screenSize(u8 = 0);    // resolution of a display
    Date        currentDay(void);      // current system date
    Clock       currentTime(void);     // current system time
};
struct Setup {
    void scalingMode(std::string_view);          // set render scaling quality
    void backend(std::string_view);              // set render backend
    void audioMode(std::string_view);            // set audio resampling mode
    void audioCategory(std::string_view);        // set audio category
    void mouseRelativeMode(std::string_view);    // set mouse relative mode
    void mobileOrientation(std::span<std::string_view>); // set allowed orientations
    void allocateChannels(u16);                  // set number of audio channels
};
struct Toggle {
    void vsync(bool);                  // enable/disable vsync
    void blockOnPause(bool);           // block on pause (Android)
    void gameController(bool);         // enable/disable game controller support
    void hideHomeIndicator(bool);      // hide home indicator (iOS)
    void relativeMouseMode(bool);      // enable/disable relative mouse mode
    void materialAcceleration(bool);   // enable/disable framebuffer acceleration
    void accelerometerAsJoystick(bool); // use accelerometer as joystick
};
Info   info;    // system information
Setup  setup;   // system setup (must be called before init)
Toggle toggle;  // system toggles (must be called before init)
```

---

## PhysicManager (singleton physics)

```cpp
f32   gravitationalConstant(void) const noexcept;  // get gravity
void  gravitationalConstant(f32) noexcept;         // set gravity
f32   pixelsPerMeter(void) const noexcept;         // get pixel-to-meter ratio
void  pixelsPerMeter(f32) noexcept;                // set pixel-to-meter ratio
Area  world(void) const noexcept;                  // get world boundaries
void  world(const Area&) noexcept;                 // set world boundaries (creates boundary walls)
void  remove(PhysicBody&);                         // remove a body from the simulation
const std::vector<PhysicBody*>&  bodies(void) const noexcept;   // all physics bodies
const std::vector<StaticBody*>&  statics(void) const noexcept;  // static bodies only
const std::vector<DynamicBody*>& dynamics(void) const noexcept; // dynamic bodies only
```

---

## PhysicBody

```cpp
void        tag(std::string_view) noexcept;         // set identifying tag
std::string tag(void) const noexcept;                // get tag
u64         ID(void) const noexcept;                 // get unique body ID
void        isSolid(bool) noexcept;                  // set solid or sensor
bool        isSolid(void) const noexcept;            // check if solid
void        link(Sprite&) noexcept;                  // attach a sprite texture
void        move(const Vec2d&) noexcept;             // move body
void        rotate(f32) noexcept;                    // rotate body
void        scale(const Fact2d&) noexcept;           // scale body
void        resize(const Dim2d&) noexcept;           // resize body
Vec2d       center(void) const noexcept;             // get center position
Dim2d       size(void) const noexcept;               // get size
void        linkAnimation(AnimEntry); 				// attach an animation
Animation&  animation(std::string_view);            // get animation by name

// Parameter : PhysicBody *self, PhysicBody *other
_PhysicSignal<PhysicBody*, PhysicBody*> onContact;       // emitted on contact persist
_PhysicSignal<PhysicBody*, PhysicBody*> onContactStart;  // emitted on contact begin
_PhysicSignal<PhysicBody*, PhysicBody*> onContactEnd;    // emitted on contact end
```

---

## StaticBody

```cpp
explicit StaticBody(const Geometry& shape); // create and register a static physics body
```

---

## DynamicBody

```cpp
explicit DynamicBody(const Geometry& shape); // create and register a dynamic physics body
void  mass(f32) noexcept;              // set mass (kg)
void  bounce(f32) noexcept;            // set restitution 0.0–1.0
void  bounceThreshold(f32) noexcept;   // set minimum velocity for bounce
void  infiniteBounce(bool) noexcept;   // toggle perfect bouncing
void  friction(f32) noexcept;          // set friction coefficient
void  gravity(bool) noexcept;          // enable/disable gravity
void  isBullet(bool) noexcept;         // enable bullet (CCD) mode for fast objects
void  warp(const Area&) noexcept;      // set screen-wrapping area
void  limit(const Area&) noexcept;     // set movement boundaries
void  jump(f32) noexcept;              // apply upward impulse
Vec2d velocity(void) const noexcept;   // get linear velocity
void  velocity(const Vec2d&) noexcept; // set linear velocity directly
void  move(const Vec2d&) noexcept;     // apply continuous force

// Parameter : DynamicBody *self
_PhysicSignal<DynamicBody*> onMove;                // emitted while moving
_PhysicSignal<DynamicBody*> onMoveUp;              // emitted while moving upward
_PhysicSignal<DynamicBody*> onMoveDown;            // emitted while moving downward
_PhysicSignal<DynamicBody*> onMoveLeft;            // emitted while moving left
_PhysicSignal<DynamicBody*> onMoveRight;           // emitted while moving right
```

---

## Sound / Music / SFX

```cpp
// Sound (abstract base)
i8       playFor(void) const noexcept;        // remaining loops
u8       getVolume(void) const noexcept;      // current volume
virtual void play(i8 = 0) = 0;                // start playback (0=once, -1=infinite, n=n times)
virtual void stop(void) = 0;                  // stop playback
virtual void pause(void) = 0;                 // pause playback
virtual void resume(void) = 0;                // resume playback
virtual void volume(u8) noexcept = 0;         // set volume (1–128)

// Music
Music(std::string_view path, u8 volume = 64); // load music file
void play(i8 loop = 0) override;              // start music
void stop(void) override;                     // stop music
void pause(void) override;                    // pause music
void resume(void) override;                   // resume music
void volume(u8) noexcept override;            // set music volume

// SFX
SFX(std::string_view path, u8 volume = 64);  // load sound effect file
void play(i8 loop = 0) override;              // play sound
void stop(void) override;                     // stop sound on current channel
void stopAll(void);                           // stop all channels playing this SFX
void pause(void) override;                    // pause sound
void resume(void) override;                   // resume sound
void volume(u8) noexcept override;            // set sound volume
```

---

## TextureBase (abstract)

```cpp
virtual void          move(const Vec2d&) noexcept = 0;                     // translate texture
virtual void          rotate(f32) noexcept = 0;                             // rotate texture (radians)
virtual void          scale(const Fact2d&) noexcept = 0;                    // scale texture
virtual void          resize(const Dim2d&) noexcept = 0;                    // resize texture
virtual void          transform(const Vec2d&, f32, const Fact2d&) noexcept = 0; // translate + rotate + scale
virtual Vec2d         center(void) const noexcept = 0;                      // get center position
virtual Dim2d         size(void) const noexcept = 0;                        // get current size
virtual Dim2d         realSize(void) const noexcept = 0;                    // get original image size
virtual const Geometry& shape(void) const noexcept = 0;                     // get bounding shape
virtual void          unclip(void) noexcept = 0;                            // remove clipping rectangle
virtual void          clip(const Vec2d&, const Dim2d&) noexcept = 0;        // set clipping rectangle
virtual bool          hasIntersected(const Geometry&) const noexcept = 0;   // collision test
virtual bool          hasIntersected(const TextureBase&) const noexcept;    // collision test (texture vs texture)
virtual const SDL_Rect* getClipRect(void) const noexcept = 0;               // get current clip rect
virtual std::vector<SDL_Vertex> vertices(void) const noexcept = 0;          // get transformed vertices
```

!!! warning
    `opacity()` has been **removed** from `TextureBase`/`Texture<S>` (and `color()`/`opacity()` removed from `Text`). Tinting and transparency
    are now driven exclusively by the `Color` argument passed to `Window::draw()` / `Window::fill()`; textures are internally generated white
    so SDL's color-mod correctly reproduces the requested tint, including alpha.

Internally, each `Texture<S>` now caches, per renderer, a nested `TextureData { SDL_Texture* texture; Color current_color; }` (held in a
`mutable m_textures` map) so that repeated `draw()` calls with the same color skip redundant `SDL_SetTextureColorMod`/`SDL_SetTextureAlphaMod`
calls — this also makes multi-window color caching correct per-renderer.

---

## Texture / Sprite / Image

```cpp
Texture(std::string_view path, const S& shape); // load texture from file for a given shape
// All TextureBase methods above are implemented here.

// Sprite alias (Rectangle shape)
Sprite(std::string_view path, const Rectangle&); // rectangular sprite
```

---

## Text

```cpp
Text(std::string_view font_name, Vec2d pos); // create text at position using a loaded font

void anchorX(anchor::x) noexcept;     // set horizontal anchor (left, center, right)
void anchorY(anchor::y) noexcept;     // set vertical anchor (top, middle, bottom)
std::string text(void) const noexcept; // get current text content

void write(std::string_view);   // set text content
void append(std::string_view);  // append text to current content
void append(fmt);               // append a formatting token (see below)
void clear(void);               // clear text content

void maxLengh(u16) noexcept;         // set max wrap length (pixels)
u16  maxLengh(void) const noexcept;  // get max wrap length
```

```cpp
// fmt tokens usable with Text::append(fmt)
enum fmt : u8 {
    nl,     // insert a newline
    tab,    // insert a tab (4 spaces)
    endl,   // mark next append() as a fresh write (erase current content first)
    flush   // clear the text immediately
};
```

```cpp
// Stream-style operators (remake2d/stream.hpp)
Text& operator<<(Text&, Text&)            noexcept; // append another Text's content
Text& operator<<(Text&, std::string_view) noexcept; // append a string
Text& operator<<(Text&, fmt)               noexcept; // append a fmt token
template<IsBasicType T> Text& operator<<(Text&, const T&) noexcept; // append any basic/arithmetic type (stringified)
```

---

## FontManager (singleton font)

```cpp
void load(std::string_view tag, std::string_view path, u8 size); // load a font from file
```

---

## Animation

```cpp
Animation(std::string_view path, const Rectangle& shape,
          u8 total_clips, Dim2d clip_size,
          Vec2d start_pos = {0,0}, u8 spacing = 0); // create sprite sheet animation
void play(i8 loop = 0, u8 fps = 12); // start animation (0=once, -1=infinite, n=n times)
void pause(void) noexcept;           // pause animation
void resume(void) noexcept;          // resume animation
void stop(void) noexcept;            // stop and reset to first frame
```

---

## Camera

```cpp
Camera(void);                                                    // default camera
Camera(const Vec2d& center, const Dim2d& size, const Dim2d& limit); // camera with bounds
void  zoom(f32) noexcept;                                        // set zoom level
void  move(const Vec2d&) noexcept;                               // move camera
void  limit(const Dim2d&) noexcept;                              // set world boundaries
void  resize(const Dim2d&) noexcept;                             // set viewport size
void  smoothing(f32) noexcept;                                   // set smoothing factor (0.0–1.0)
f32   zoom(void) const noexcept;                                 // get zoom
Dim2d size(void) const noexcept;                                 // get viewport size
Vec2d center(void) const noexcept;                               // get camera position
Dim2d limit(void) const noexcept;                                // get world boundaries
Vec2d offset(void) const noexcept;                               // get last frame offset
Vec2d followedPoint(void) const noexcept;                        // get tracked point
f32   smoothing(void) const noexcept;                            // get smoothing factor
void  follow(Vec2d&) noexcept;                                   // track a point
void  follow(Geometry&) noexcept;                                // track a shape
void  follow(PhysicBody&) noexcept;                              // track a physics body
Signal<> onMove;                                                 // emitted when camera moves
```

---

## TileMap

```cpp
TileMap(std::string_view tileset_path, TileMapData data); // create tile map
// TileMapData { Vec2d center; Dim2d size; Vec2d clip_start; Dim2d clip_size; Grid2d cut; u8 margin; }
using TileID       = i16;                          // tile identifier type
using TileTemplate = std::vector<TileID>;          // tile layout type

Vec2d  center(void) const noexcept;                // get map position
Dim2d  size(void) const noexcept;                  // get map size
Dim2d  clip(void) const noexcept;                  // get tile clip size
Grid2d cut(void) const noexcept;                   // get grid dimensions

void   move(Vec2d) noexcept;                       // move map
void   resize(Dim2d) noexcept;                     // resize map
void   counterStart(i16) noexcept;                 // set starting tile ID offset
void   tag(std::string_view, TileID);              // assign a tag to a tile ID

void   load(TileTemplate);                         // load tile layout
void   build(void) noexcept;                       // build physics and animations (async)

void        applyPhysic(TileID);                   // apply physics to a tile type
void        applyPhysic(std::string_view);         // apply physics to a tagged tile type
PhysicBody& body(TileID);                          // get physics body by tile ID
PhysicBody& body(std::string_view);                // get physics body by tag

u32         tileCount(void) const noexcept;        // total number of tiles
u32         tileCount(TileID) const noexcept;      // count of a specific tile type
```

---

## TileGrid

```cpp
TileGrid(const Vec2d& center, const Dim2d& size, const Grid2d& cut); // create a grid
void move(const Vec2d&) noexcept;     // move grid center
void cut(const Grid2d&) noexcept;     // change grid divisions
void resize(const Dim2d&) noexcept;   // resize grid

usize         count(void) const noexcept;           // total number of cells
Dim2d         size(void) const noexcept;            // grid size
Grid2d        cut(void) const noexcept;             // current grid divisions
Vec2d         center(void) const noexcept;          // grid center
Area          cell(const Grid2d&) const noexcept;   // get a specific cell area by (col, row)
std::vector<Area> cells(void) const noexcept;       // get all cell areas
```

---

## Parallax

```cpp
Parallax(const Vec2d& center, const Dim2d& size,
         const SpriteList& sprites, const QuotientList& quotients); // create parallax background
// SpriteList    = std::span<Sprite>
// QuotientList  = std::span<u8>  (speed reduction percentages per layer)

void  move(const Vec2d&) noexcept;     // move background
void  resize(const Dim2d&) noexcept;   // resize background
void  velocity(const Vec2d&) noexcept; // set scroll speed
Vec2d velocity(void) const noexcept;   // get scroll speed
Dim2d size(void) const noexcept;       // get size
Vec2d center(void) const noexcept;     // get center
```

---

## Data / Save

```cpp
// Data (sum type)
Data(byte); Data(rune); Data(imax); Data(fmax); Data(bool);
Data(Vec2d); Data(Fact2d); Data(Grid2d); Data(Dim2d); Data(Color); Data(Area);
Data(std::string_view);
Data(std::vector<Data>);
Data(std::map<std::string, Data>);
static Data map(std::span<std::pair<const std::string, Data>>); // create map
static Data list(std::span<Data>);                              // create list

const Data& operator[](std::string_view key) const;  // access map field by key
const Data& operator[](usize idx) const;              // access list element by index
template<typename T> T get(std::string_view key) const; // typed field access
template<typename T> operator T(void) const;             // implicit conversion

// ISavable (interface)
virtual Data sdata(void) const = 0;   // serialize to Data
virtual void ldata(const Data&) = 0;  // load from Data

// DataFile
DataFile(std::string_view name);           // open or create a data file
void save(const Data&);                    // save raw Data
void load(Data&);                          // load raw Data
void save(const ISavable&);                // save a savable object
void load(ISavable&);                      // load into a savable object
template<IsSavable T> friend DataFile& operator<<(DataFile&, const T&); // stream-save a savable object
template<IsSavable T> friend DataFile& operator>>(DataFile&, T&);       // stream-load a savable object
void        remove(void) noexcept;         // delete the file
bool        exist(void) const noexcept;    // check if file exists
std::string path(void) const noexcept;     // get full file path
std::string name(void) const noexcept;     // get file name

// SaveManager (singleton data)
std::string root(void) const noexcept;        // get root directory
void        root(std::string_view) noexcept;  // set root directory
bool        isInitialized(void) const noexcept; // check if initialized
```

---

## Script

```cpp
// SolState (singleton script)
template<typename T, typename... Ctors>
void registerType(std::string_view name, std::function<void(SolState::Type&)> init = nullptr); // register C++ type to Lua
template<typename T> void loadVar(std::string_view id, T& data) noexcept; // expose C++ variable to Lua
std::string loadedTypes(void) const noexcept;      // list all registered type names
bool        isLoadedType(std::string_view) const noexcept; // check if a type is registered

// Script
Script(std::string_view lua_file);            // load a Lua script
template<typename T> T get(std::string_view); // get a Lua variable by name
void update(void);                            // run the script
Signal<> onFileChanged;                       // emitted when script file changes on disk
```

---

## Scene & Actor

```cpp
// ActorBase
virtual void update(void) = 0;                                // update logic (override in derived)
void             addChild(ActorBase*) noexcept;               // add child actor
void             removeChild(ActorBase*) noexcept;            // remove child actor
ActorBase*       parent(void) const noexcept;                 // get parent actor
const std::vector<ActorBase*>& children(void) const noexcept; // get child actors
void             active(bool) noexcept;                       // enable or disable actor
bool             active(void) const noexcept;                 // check if active

// Actor (concrete, no physics)
// PhysicActor<P> (StaticActor, DynamicActor)
PhysicActor(const Geometry&); // create actor with physics body
P body; // public physics body member

using StaticActor  = PhysicActor<StaticBody>;
using DynamicActor = PhysicActor<DynamicBody>;

// Scene
void execute(const Frame&);               // set main update function
void add(ActorBase&, i16 layer = 0);      // add actor to a layer
void add(const Frame&, i16 layer = 0);    // add function to a layer
void remove(ActorBase&);                  // remove actor from scene
void update(void);                        // run one frame
void enable(void) noexcept;               // enable scene updates
void disable(void) noexcept;              // disable scene updates
bool isEnabled(void) const noexcept;      // check if scene is enabled
void setLayerActive(i16 layer, bool);     // enable/disable all actors on a layer
void setActorActive(ActorBase&, bool);    // enable/disable a specific actor

// Act
void add(std::string_view name, Scene&);                               // register a scene
void link(std::string_view name, std::span<std::string_view> scenes);  // group scenes under a tag
void focus(std::string_view);                                          // activate a scene or link
void update(void) const;                                               // update focused scene(s)
void update(std::string_view) const;                                   // update a specific scene or link
void updates(void) const;                                              // update all scenes
Scene* scene(std::string_view) const;                                  // get scene by tag
```

---

## Croutines

```cpp
// Croutine<Args...> and Croutine<>
template<typename F> void load(F&& function); // attach a coroutine function
void run(Args... args);                        // start with arguments
void run(void);                                // start without arguments (Croutine<> only)
void resume(void) noexcept;                    // resume after pause
void stop(void) noexcept;                      // stop permanently
void wait(void) noexcept;                      // block caller until coroutine finishes
bool isRunning(void) noexcept;                 // check if currently running
u64  id(void) const noexcept;                  // get coroutine ID
u64  majorID(void) const noexcept;             // get worker thread ID
void priority(croutine::priority) noexcept;    // set scheduling priority (engine, heavy, user)
void isEngine(bool) noexcept;                  // mark as engine priority
void isHeavy(bool) noexcept;                   // mark as heavy priority
auto operator co_await(void) noexcept;         // await completion from another coroutine

// Macros
rmk_pause();            // yield execution back to the scheduler
rmk_await(coro);        // await another croutine
rmk_lockedPause(lock);  // yield while releasing a PauseGuard lock
```

---

## Lock (coroutine-safe locks)

```cpp
// PauseGuard — wraps a std::mutex for exclusive access across a rmk_pause()
explicit PauseGuard(std::mutex&); // lock mutex on construction
void unlock(void) noexcept;       // manually unlock
void relock(void) noexcept;       // manually re-lock

// SharedPause — wraps a std::shared_mutex for shared read access across a rmk_pause()
explicit SharedPause(std::shared_mutex&); // shared lock on construction
void unlock(void) noexcept;               // manually unlock
void relock(void) noexcept;               // manually re-lock

// Macro
rmk_lockedPause(lock); // release lock, yield, re-acquire on resume
```

---

## MainRenderLoop (singleton loop)

```cpp
void execute(Window& win, const std::function<void(void)>& body) noexcept; // loop while window is open
void execute(const std::function<bool(void)>& cond, const std::function<void(void)>& body) noexcept; // loop with custom condition
void update(void) noexcept;                             // start the main loop (blocking)
bool isRunning(void) const noexcept;                    // check if loop is active
```

---

## Random (singleton random)

```cpp
u32  seed(void) noexcept;                   // get current seed
void seed(u32) noexcept;                    // set seed
u32  randSeed(void) noexcept;               // generate a hardware random seed
void rollSeed(void) noexcept;               // generate and apply a new random seed
template<IsBasicType T = i32> T rand(const T& min, const T& max) noexcept; // random integer/float
template<> Vec2d  rand<Vec2d>(const Vec2d& min, const Vec2d& max) noexcept;   // random vector
template<> Fact2d rand<Fact2d>(const Fact2d& min, const Fact2d& max) noexcept;   // random vector
template<> Grid2d rand<Grid2d>(const Grid2d& min, const Grid2d& max) noexcept;   // random vector
template<> Dim2d  rand<Dim2d>(const Dim2d& min, const Dim2d& max) noexcept;   // random dimensions
template<> Color  rand<Color>(const Color& min, const Color& max) noexcept;   // random color in HSL range
Color mixColor(f32 saturation = 1.0f, f32 lightness = 0.5f) noexcept;        // random vivid color (HSL)
template<typename T> T choice(const std::span<T>&) noexcept;                 // random element from span
std::string choice(const std::span<std::string_view>&) noexcept;             // random string from span
std::vector<u8> dice(u8 n, u8 faces) noexcept; // roll n dice with given face count
bool chance(f32 probability) noexcept;          // true with given probability (0.0–1.0)
```

---

## Miscellaneous Utilities

```cpp
// Nil
constexpr Nil nil; // universal null/zero placeholder
// Operators: all types comparable to nil via == and !=
// (nil == T{} evaluates to true)

// Layer helpers (returns i16 layer value)
namespace layer {
    i16 ground(u8 wall); // wall - 256  (background elements)
    i16 world(u8 wall);  // wall        (game world objects)
    i16 sky(u8 wall);    // wall + 256  (foreground effects)
    i16 ui(u8 wall);     // wall + 511  (user interface)
    i16 log(u8 wall);    // wall + 767  (debug overlay)
    inline constexpr i16 min = -256;
    inline constexpr i16 max = 1023;
}

// Angle utilities
namespace angle {
    fmax degToRad(fmax degrees) noexcept;
    fmax radToDeg(fmax radians) noexcept;
    namespace literal {
        fmax operator""_deg(fmax) noexcept; // degrees to radians literal
        fmax operator""_rad(fmax) noexcept; // radians to degrees literal
    }
}

// File utilities
namespace file {
    std::string jump(std::span<std::string_view> paths) noexcept; // return first existing file path
}

// Time literals
namespace time::literal {
    operator""_ns; operator""_us; operator""_ms;
    operator""_s;  operator""_min; operator""_h;
    operator""_d;  operator""_w; operator""_mo; operator""_y;
}

// Physics unit conversion
namespace physic {
    f32   pixelToMeter(f32) noexcept;
    f32   meterToPixel(f32) noexcept;
    Vec2d pixelToMeter(const Vec2d&) noexcept;
    Vec2d meterToPixel(const Vec2d&) noexcept;
    Dim2d pixelToMeter(const Dim2d&) noexcept;
    Dim2d meterToPixel(const Dim2d&) noexcept;
}

// Constants
inline constexpr fmax pi      = 3.14159265358979;
inline constexpr bool on      = true,  off     = false;
inline constexpr bool isTrue  = true,  isFalse = false;
inline constexpr u8   success = 0,     fail    = 1;
```