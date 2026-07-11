# Timer

Waiting three seconds before spawning a wave, or firing an event once a cooldown expires, shouldn't require manually 
accumulating delta time in a variable every frame.

---

## Overview

`Timer` is contained in the header **"remake2d/time.hpp"**. It counts down toward a set limit, and emits a signal once 
elapsed, without needing to be polled manually every frame.

---

## Methods

```cpp
void limit(fmax)         noexcept; // set limit, converse in seconds
void limit(time::Second) noexcept; // set limit
fmax limit(void)    const noexcept; // get limit

void start(void)  noexcept; // start or restart the timer
void stop(void)   noexcept; // stop and reset
void pause(void)  noexcept; // pause without resetting
void resume(void) noexcept; // resume after pause
void repeat(bool) noexcept; // auto-restart on timeout

bool isActive(void)    const noexcept; // check if currently running
bool isElapsed(void)   const noexcept; // check if timeout occurred
fmax elapsedTime(void) const noexcept; // current elapsed time

_TimerSignal<> onTimeout;
```

---

## Usage

### Creating a timer

A limit can be passed directly at construction, or set afterwards:

```cpp
Timer(void);
Timer(fmax limit);
Timer(time::Second limit);
```

```cpp
rmk::Timer cooldown(3); // 3 seconds
```

### Starting and controlling

Start resets the elapsed time and begins counting; pause and resume freeze and unfreeze it without losing progress:

```cpp
cooldown.start();

if (playerStunned) cooldown.pause();
else cooldown.resume();
```

### Reacting to timeout

`onTimeout` fires once when the timer reaches its limit, which is where most of the actual logic belongs rather than 
polling isElapsed every frame:

```cpp
cooldown.onTimeout.join([&]() {
    spawnEnemyWave();
});
```

### Repeating

Repeat makes a timer restart itself automatically as soon as it elapses, useful for anything on a fixed interval, like
a wave spawner or a periodic heal tick:

```cpp
cooldown.repeat(true);
```

---

[:octicons-arrow-left-24: Previous chapter](delta.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](chrono.md){ .md-button .md-button--primary }
