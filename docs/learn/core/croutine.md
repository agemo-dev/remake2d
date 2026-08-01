# Croutine

Croutines are **RE:MAKE 2D**'s concurrency system, combining the simplicity of **coroutines** with the power of **multithreading**. They allow running tasks
asynchronously without blocking the main loop.

---

## Overview

Croutines are represented by the `Croutine` class, contained in the header **"remake2d/croutine.hpp"**. Internally, they are scheduled by a thread
pool (`CroutinePool`) that manages three priority lanes:

- **engine** : reserved for internal engine tasks
- **heavy** : for CPU-intensive background work
- **user** : for standard game logic (default)

```cpp
rmk::Croutine<>         coro; // no arguments
rmk::Croutine<f32>      coro; // one argument
rmk::Croutine<i32, f32> coro; // multiple arguments
```

---

## Methods

```cpp
template<typename F> void load(F&& function); // attach a coroutine function
void run(Args... args);                        // start the coroutine
void resume(void) noexcept;                    // resume after a pause
void stop(void) noexcept;                      // stop permanently
void wait(void) noexcept;                      // block caller until coroutine finishes
bool isRunning(void) noexcept;                 // check if currently running
u64  id(void) const noexcept;                  // get coroutine ID
u64  majorID(void) const noexcept;             // get worker thread ID
void priority(croutine::priority p) noexcept;  // set scheduling priority
void isEngine(bool v) noexcept;                // mark as engine priority
void isHeavy(bool v) noexcept;                 // mark as heavy priority
auto operator co_await(void) noexcept;         // await completion from another coroutine
```

Inside a coroutine function, three macros are available:

```cpp
rmk_pause();             // yield execution back to the scheduler (resumes next frame)
rmk_await(coro);         // suspend until another croutine finishes
rmk_lockedPause(lock);   // yield while safely releasing a PauseGuard lock
```

!!! warning
    A croutine function must return `rmk::Task` and use `rmk_pause()` to yield. A croutine that never yields will block its worker thread.

---

## Lock

When sharing data between croutines or between a croutine and the main thread, standard mutexes are not safe to use across a `rmk_pause()` — they do not
release the lock during the yield, which causes a deadlock.

**RE:MAKE 2D** provides two coroutine-safe lock types in **"remake2d/lock.hpp"**:

- `PauseGuard` : wraps a `std::mutex` for exclusive write access
- `SharedPause` : wraps a `std::shared_mutex` for shared read access

Both follow the RAII pattern — the lock is acquired on construction and released on destruction.

```cpp
// PauseGuard
explicit PauseGuard(std::mutex& mtx);  // acquire exclusive lock
void unlock(void) noexcept;            // manually release
void relock(void) noexcept;            // manually re-acquire

// SharedPause
explicit SharedPause(std::shared_mutex& mtx);  // acquire shared lock
void unlock(void) noexcept;                    // manually release
void relock(void) noexcept;                    // manually re-acquire
```

Used with the macro:

```cpp
rmk_lockedPause(lock); // release lock, yield, re-acquire on resume
```

!!! warning
    Never use `std::lock_guard` or `std::unique_lock` inside a croutine. They will not release the mutex during `rmk_pause()`, causing a deadlock.

!!! info
    Use `PauseGuard` when writing to shared data and `SharedPause` when only reading. Multiple `SharedPause` locks can coexist, but a `PauseGuard` will wait
    for all shared locks to be released first.

---

## Usage

### Basic usage

```cpp
rmk::Croutine<> coro;

coro.load([]() -> rmk::Task {
    while (true) {
        doBackgroundWork();
        rmk_pause(); // yield every frame
    }
});

coro.run();
```

### With arguments

```cpp
rmk::Croutine<f32> coro;

coro.load([](f32 duration) -> rmk::Task {
    f32 elapsed = 0.0f;
    while (elapsed < duration) {
        elapsed += rmk::delta.tick();
        rmk_pause();
    }
    std::cout << "Done after " << duration << "s\n";
});

coro.run(3.0f);
```

### Awaiting another croutine

```cpp
rmk::Croutine<> loader;
rmk::Croutine<> game;

loader.load([]() -> rmk::Task {
    loadAssets();
    rmk_pause();
});

game.load([&]() -> rmk::Task {
    rmk_await(loader); // wait for loader to finish
    startGame();
    rmk_pause();
});

loader.run();
game.run();
```

### Priority

```cpp
rmk::Croutine<> heavy;
heavy.isHeavy(true); // runs on the heavy thread, won't compete with user croutines

heavy.load([]() -> rmk::Task {
    while (true) {
        computePathfinding();
        rmk_pause();
    }
});

heavy.run();
```

### Thread-safe yield

Use `PauseGuard` when writing to shared data across a yield:

```cpp
std::mutex mtx;
rmk::Croutine<> coro;

coro.load([&]() -> rmk::Task {
    while (true) {
        rmk::PauseGuard lock(mtx);
        sharedData.update();
        rmk_lockedPause(lock); // release, yield, re-acquire
    }
});

coro.run();
```

Use `SharedPause` when multiple croutines only need to read shared data simultaneously:

```cpp
std::shared_mutex mtx;
rmk::Croutine<> reader;

reader.load([&]() -> rmk::Task {
    while (true) {
        rmk::SharedPause lock(mtx);
        processData(sharedData);
        rmk_lockedPause(lock); // release, yield, re-acquire
    }
});

reader.run();
```

---

[:octicons-arrow-left-24: Previous chapter](event.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](system.md){ .md-button .md-button--primary }