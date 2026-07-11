# Events

**Events** are essential objects that allow you to interact with the user's peripherals.

---

## Overview

To interact with the different machine events, **RE:MAKE 2D** offers the singleton `event` of type `EventManager`, storing various **event signals** of type `_EventSignal`.

`_EventSignal` inherits from all methods of the `Signal` class, but also has its own method:

```cpp
bool isActive(void) const noexcept;
```

Which allows checking if an event is active during the analysis of another one. This is particularly useful for key combinations:

```cpp
rmk::event.onPressS.join([&](void) {
    if(rmk::event.onPressLCtrl.isActive()) save();
});
```

!!! warning
    isActive() is preferred for key combinations because `event` handles one SDL event at a time. Two keys pressed simultaneously will be processed in separate calls,
    so a combination like Ctrl+S cannot be detected by connecting both events independently.

---

## Different events

The `EventManager` class covers 300+ events, handling keyboard, mouse, gamepad, touch, window and application events. 
Each event is a `_EventSignal` instance that can be connected just like a regular `Signal`.

### Keyboard

```cpp
_EventSignal<> onPressA .. onPressZ;
_EventSignal<> onPress0 .. onPress9;
_EventSignal<> onPressSpace, onPressEnter, onPressEscape;
_EventSignal<> onPressBackspace, onPressTab, onPressCapsLock;
_EventSignal<> onPressDelete, onPressInsert, onPressHome;
_EventSignal<> onPressEnd, onPressPageUp, onPressPageDown;
_EventSignal<> onPressLeft, onPressRight, onPressUp, onPressDown;
_EventSignal<> onPressLShift, onPressRShift;
_EventSignal<> onPressLCtrl, onPressRCtrl;
_EventSignal<> onPressLAlt, onPressRAlt;
_EventSignal<> onPressAny;
_EventSignal<> onPressF1 .. onPressF12;

// Equivalent onRelease* events
// Equivalent onPressScan*, onReleaseScan* events (scancode-based, layout-independent)
```

### Text input

```cpp
_EventSignal<std::string> onTextInput;   // UTF-8 text input
_EventSignal<std::string> onTextEdit;    // IME composition
```

### Mouse

```cpp
_EventSignal<Vec2d> onMouseMove;         // absolute position
_EventSignal<Vec2d> onMouseRawMove;      // relative motion
_EventSignal<Vec2d> onLeftDown, onLeftUp;
_EventSignal<Vec2d> onRightDown, onRightUp;
_EventSignal<Vec2d> onMiddleDown, onMiddleUp;
_EventSignal<Vec2d> onDoubleClick;
_EventSignal<Vec2d> onWheel;             // scroll delta (x, y)
```

### Gamepad

```cpp
_EventSignal<i32> onPressACtrl, onPressBCtrl, onPressXCtrl, onPressYCtrl;
_EventSignal<i32> onPressStart, onPressSelect;
_EventSignal<i32> onPressLShoulder, onPressRShoulder;
_EventSignal<i32> onPressDpadLeft, onPressDpadRight, onPressDpadUp, onPressDpadDown;
_EventSignal<i32> onPressLStick, onPressRStick;
// Equivalent onRelease* events
_EventSignal<i32, i16> onAxisLeftX, onAxisLeftY;       // i32 = controller id, i16 = axis value
_EventSignal<i32, i16> onAxisRightX, onAxisRightY;
_EventSignal<i32, i16> onAxisLTrigger, onAxisRTrigger;
_EventSignal<i32> onControllerAdded;
_EventSignal<i32> onControllerRemoved;
```

### Touch

```cpp
_EventSignal<Vec2d> onFingerDown;    // normalized position (0.0 - 1.0)
_EventSignal<Vec2d> onFingerUp;
_EventSignal<Vec2d> onFingerMove;
_EventSignal<Vec2d> onMultiGesture;
```

### Window

```cpp
_EventSignal<u32>        onWindowClose;
_EventSignal<u32>        onWindowMinimized, onWindowMaximized, onWindowRestored;
_EventSignal<u32>        onWindowShown, onWindowHidden, onWindowExposed;
_EventSignal<u32>        onWindowFocusGained, onWindowFocusLost;
_EventSignal<u32>        onWindowMouseEnter, onWindowMouseLeave;
_EventSignal<u32, Dim2d> onWindowResized;    // u32 = window id, Dim2d = new size
_EventSignal<u32, Vec2d> onWindowMoved;      // u32 = window id, Vec2d = new position
```

### Application

```cpp
_EventSignal<>            onQuit;
_EventSignal<std::string> onDropFile;
_EventSignal<>            onClipboardUpdate;
_EventSignal<>            onAppForeground, onAppBackground, onAppLowMemory;
_EventSignal<>            onLocaleChanged, onDisplayChanged, onRenderReset;
_EventSignal<>            onEventNone;
```

---

## Usage

Connecting a function to an event works exactly the same way as with signals:

```cpp
rmk::event.onQuit.connect(foo);
rmk::event.onPressSpace.join([&](void) { player.jump(400.0f); });
rmk::event.onLeftDown.join([&](Vec2d pos) { shoot(pos); });
rmk::event.onPressACtrl.join([&](i32 id) { player.jump(400.0f); });
```

---

## Methods

The `EventManager` provides three ways to read events each frame, depending on how you want your loop to behave.

```cpp
void poll(void);                        // process all pending events (non-blocking)
void wait(void);                        // wait indefinitely for an event
void wait(time::Millisecond timeout);   // wait for an event with timeout
```

!!! info
    By default, `loop` uses `poll` to read events. To use a different mode, call it manually inside the main loop:

```cpp
rmk::loop.execute(win, [&]() {
    rmk::event.wait();
});
```

---

[:octicons-arrow-left-24: Previous chapter](signal.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](croutine.md){ .md-button .md-button--primary }