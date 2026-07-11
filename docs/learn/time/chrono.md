# Chronometer

Measuring how long a level took to complete, or how long a player has been alive, calls for something that just counts up freely,
without a fixed limit like a `Timer` has.

---

## Overview

`Chronometer` is contained in the header **"remake2d/time.hpp"**. It measures elapsed time from a start point, and can be paused
and resumed without losing accumulated time.

---

## Methods

```cpp
void start(void)  noexcept; // start or restart the chronometer
void stop(void)   noexcept; // stop and record final time
void pause(void)  noexcept; // pause without resetting
void resume(void) noexcept; // resume after pause
void reset(void)  noexcept; // reset all values to zero

template <typename D = time::Second>
fmax elapsedTime(void) noexcept;
```

---

## Usage

### Measuring elapsed time

Start begins counting from zero, and elapsedTime reads the current duration, defaulting to seconds:

```cpp
rmk::Chronometer runTime;
runTime.start();

// later
rmk::fmax seconds = runTime.elapsedTime();
```

A different unit can be requested through the template parameter, using the duration types from `time::` such as `time::Millisecond`:

```cpp
rmk::fmax ms = runTime.elapsedTime<rmk::time::Millisecond>();
```

### Pausing

Pause freezes the count without losing what's already elapsed; resume picks back up from there:

```cpp
runTime.pause();  // menu opened
runTime.resume(); // menu closed
```

### Stopping and resetting

Stop records a final time without clearing it, while reset brings everything back to zero, ready for a fresh start:

```cpp
runTime.stop();
runTime.reset();
```

---

[:octicons-arrow-left-24: Previous chapter](timer.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](date.md){ .md-button .md-button--primary }