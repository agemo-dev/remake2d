# Main Loop

The main loop is the heart of any graphical application. It continuously runs the game logic and rendering code until a stop condition is met,1allowing your application to stay responsive.

---

## Overview

If we run the program, we'll notice that it stops almost immediately after the window is destroyed.
To fix this, we'll use a **loop** that will prevent the program from ending until a
condition is raised. This loop will allow us to:

- keep the program active
- continuously update the game state
- display the frames

**RE:MAKE 2D** has its own render loop usable via the global variable `loop`, of
type `MainRenderLoop`, contained in the header **"remake2d/loop.hpp"**.

---

## Methods

The methods of the `MainRenderLoop` class are as follows:

```cpp
void execute(Window& win, const std::function<void(void)>& body) noexcept; 								  // loop while window is open
void execute(const std::function<bool(void)>& condition, const std::function<void(void)>& body) noexcept; // loop with custom condition

void update(void) noexcept;          // start the main loop (blocking)
bool isRunning(void) const noexcept; // check if loop is active
```

---

## Usage

### Initializing the game loop

To create the loop, we'll use the `execute` method, which has two overloads:

```cpp
void execute(const Window& window, const std::function<void(void)>& body);
void execute(const std::function<bool(void)>& condition, const std::function<void(void)>& body);
```

- `window`    : is the **control** window.
- `body`      : is the function to execute.
- `condition` : is the loop's execution condition.

Passing a **window** as the first argument keeps the program running as long as that window remains open.

You can check whether a window is still open with the `isOpen` method and explicitly close it with the `close` function,
and check whether it is hidden via `isHide`:

```cpp
// example
if(win.isOpen()) win.close();
```

!!! info
    The `isOpen` method checks whether a window **is still valid**, not whether it **is still visible**.
    The `close` method **releases the window's resources** and is implicitly called by the **Window** destructor or by
    an `onWindowClosed` event emitted when the user clicks the window's **red close button** or the **close key** on mobile. After
    it is called, the window becomes **"invalid"** within the program.

Let's create ours using the overload that takes a window as a parameter:

```cpp
// if you want a custom condition do:
// rmk::loop.execute([&] () { return myCondition; }, [&]() {});

rmk::loop.execute(win, [&](void) {});
rmk::loop.update();
```

The `update` method is what starts the loop.

!!! info
    The `MainRenderLoop` class's `isRunning` method lets you check whether `update` has been called.
    `update` also automatically handles physics updates and event polling.

### Rendering inside the game loop

Now that we've created the loop, let's use it in our program. Let's add the following lines:

```cpp
// Inside the render loop
win.clear(rmk::color::black);
win.present();
```

- `void clear(const Color& color)` : Gives the window a background color.
- `void present(void)` : Displays the rendered output on the window's screen.

The code becomes:

```cpp
#include <remake2d/window.hpp>
#include <remake2d/color.hpp>
#include <remake2d/loop.hpp>

int main(void) {
    rmk::Window win;
    
    rmk::loop.execute(win, [&](void) {
        win.clear(rmk::color::black);
        win.present();
    });
}
```

Now that we've added `clear` and `present`, we're going to... remove them! Why, you ask? Because the `update` method of `MainRenderLoop`
automatically clears every window to black and presents them every frame.
That said, you can still clear it with a different color if you want — this is just a precaution in case you forget.

So why add these lines in the first place if they're useless? Because it's good to know they exist =) .

Now, the base code for every game built with this engine will look like this:

```cpp
#include <remake2d/window.hpp>
#include <remake2d/loop.hpp>

int main(void) {
    rmk::Window win;
    rmk::loop.execute(win, [&](void) {});
    rmk::loop.update();
}
```

---

[:octicons-arrow-left-24: Previous chapter](window.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](shape.md){ .md-button .md-button--primary }