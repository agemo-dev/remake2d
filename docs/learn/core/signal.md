# Signal

Signals are essential objects for communication between modules, notably for **physics** and **events**, as well as for **timers**.

---

## Overview

Signals are represented by the `Signal` class, contained in the header **"remake2d/signal.hpp"**. They allow calling every **connected** or **joined** function upon **emission**.

```cpp
Signal<Args...> signal; // Args = types of arguments passed to connected functions
```

---

## Methods

The methods of the `Signal` class are the following:

```cpp
// Connection management
void connect(void (*func)(Args... args));              // connect a free function (persistent)
void disconnect(void (*func)(Args... args));           // disconnect a free function
void connectOnce(void (*func)(Args... args));          // connect a free function (one shot)
void connect(Slot& slot);                              // connect a std::function by reference
void disconnect(Slot& slot);                          // disconnect a std::function
void connectOnce(Slot& slot);                         // connect a std::function (one shot)
void join(Slot&& slot);                               // move-connect a lambda (persistent)
void joinOnce(Slot&& slot);                           // move-connect a lambda (one shot)
void joinPriority(Slot&& slot);                       // move-connect with priority execution
void connectPriority(void (*func)(Args... args));     // connect a free function with priority
void disconnectPriority(void (*func)(Args... args));  // disconnect a priority free function
void connectPriority(Slot& slot);                     // connect a std::function with priority
void disconnectPriority(Slot& slot);                  // disconnect a priority std::function
void unablePriority(void (*func)(Args... args));      // promote a normal function to priority
void unablePriority(Slot& slot);                      // promote a normal slot to priority
void disablePriority(void (*func)(Args... args));     // demote a priority function to normal
void disablePriority(Slot& slot);                     // demote a priority slot to normal

// Emission
void emit(Args... args);                              // manually trigger all connected callbacks

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

!!! info
    Priority slots and functions are called before regular ones. Within the same priority level, execution order follows the order of connection.

---

## Usage

### Manual emission

The simplest use case — connect a function and emit manually:

```cpp
rmk::Signal<f32> onHealthChanged;

onHealthChanged.join([](f32 hp) {
    std::cout << "HP : " << hp << "\n";
});

onHealthChanged.emit(75.0f);
```

### Automatic emission

Signals can be bound to a condition and emit automatically each frame:

```cpp
rmk::Signal<> onLowHealth;

onLowHealth.bind([&]() { return hp < 20.0f; });
onLowHealth.join([&]() { playWarningSound(); });
```

You can also control the edge detection:

```cpp
onLowHealth.bindRising([&]()  { return hp < 20.0f; }); // fires once when hp drops below 20
onLowHealth.bindFalling([&]() { return hp < 20.0f; }); // fires once when hp goes back above 20
onLowHealth.bindChange([&]()  { return hp < 20.0f; }); // fires on both transitions
```

### Priority

Priority slots are called before regular ones, useful for engine-critical callbacks:

```cpp
onHealthChanged.joinPriority([&](f32 hp) { updateHUD(hp); }); // called first
onHealthChanged.join([&](f32 hp) { checkDeath(hp); });         // called after
```

### One-shot connection

```cpp
onHealthChanged.joinOnce([](f32 hp) {
    std::cout << "First hit at : " << hp << "\n";
}); // disconnected automatically after first emission
```

---

[:octicons-arrow-left-24: Previous chapter](error.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](event.md){ .md-button .md-button--primary }