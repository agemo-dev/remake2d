# Text

Drawing a score counter or a dialogue box means rendering actual characters on screen, which needs a loaded font behind it rather than a plain image file.

---

## Overview

`Text` is contained in the header **"remake2d/texture.hpp"**, inheriting from `Texture<Rectangle>`. It renders a string using a font previously loaded through 
`FontManager`, exposed through the singleton `font`, and regenerates its underlying image whenever its content changes.

---

## Methods

```cpp
void anchorX(anchor::x) noexcept; // set horizontal anchor (left, center, right)
void anchorY(anchor::y) noexcept; // set vertical anchor (top, middle, bottom)

std::string text(void) const noexcept; // get current text content

void write(std::string_view);  // replace the text content
void append(std::string_view); // append text to the current content
void append(fmt);              // append a formatting token
void clear(void);               // clear the text content

void maxLengh(u16)       noexcept; // set max wrap length, in pixels
u16  maxLengh(void) const noexcept; // get max wrap length
```

---

## Usage

### Loading a font

Before a `Text` can be created, its font must be loaded once through the singleton `font`:

```cpp
rmk::font.load("arial", "arial.ttf", 24);
```

### Creating text

```cpp
Text(std::string_view font, Vec2d pos);
```

```cpp
rmk::Text label("arial", {100, 50});
label.write("Score : 0");
```

### Drawing text

```cpp
// In render loop
win.draw(label);
```

### Updating content

Write replaces the whole content, while append adds to it; both regenerate the underlying texture only when the text actually changes:

```cpp
label.write("Score : 100");
label.append(" (new high score!)");
```

### Formatting tokens

Append also accepts an `fmt` token instead of a string, for control characters that don't read naturally as plain text:

```cpp
enum fmt : u8 {
	nl,     // insert a newline
	tab,    // insert a tab
	endl,   // mark the next append as a fresh write, erasing current content first
	flush   // clear the text immediately
};
```

```cpp
label.append("Line 1");
label.append(fmt::nl);
label.append("Line 2");
```

### Anchoring

By default text grows from its top-left corner; `anchorX` and `anchorY` change the point the text is positioned around, useful for centering a label 
regardless of how long its content ends up being:

```cpp
label.anchorX(rmk::anchor::x::center);
label.anchorY(rmk::anchor::y::middle);
```

### Wrapping

`maxLengh` sets a maximum width, in pixels, beyond which the text wraps onto a new line automatically:

```cpp
label.maxLengh(200);
```

---

[:octicons-arrow-left-24: Previous chapter](texture.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](animation.md){ .md-button .md-button--primary }
