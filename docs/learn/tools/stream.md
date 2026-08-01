 # Stream

Printing a `Vec2d` for debugging, or building up a `Text` piece by piece as game logic runs, shouldn't require manual conversions every time.

---

## Overview

**RE:MAKE 2D** extends the standard C++ streams so its own types behave naturally with them. This lives in the header **"remake2d/stream.hpp"**,
covering two cases: printing/parsing base types through `std::ostream`/`std::istream`, and building up a `Text` through chained `<<`. `DataFile`
also gets dedicated `<<`/`>>` overloads for any type implementing `ISavable`.

---

## Printing base types

`Vec2d`, `Fact2d`, `Dim2d`, `Color` and `Area` all behave like native types in front of `std::cout`:

```cpp
std::ostream& operator<<(std::ostream&, const Vec2d&)  noexcept;
std::ostream& operator<<(std::ostream&, const Fact2d&) noexcept;
std::ostream& operator<<(std::ostream&, const Dim2d&)  noexcept;
std::ostream& operator<<(std::ostream&, const Color&)  noexcept;
std::ostream& operator<<(std::ostream&, const Area&)   noexcept;
```

```cpp
rmk::Vec2d pos = {100, 200};
std::cout << pos << "\n"; // [ x : 100  |  y : 200 ]
```

The reverse works too, reading a comma-separated line from a `std::istream`:

```cpp
std::istream& operator>>(std::istream&, Vec2d&);
std::istream& operator>>(std::istream&, Fact2d&);
std::istream& operator>>(std::istream&, Dim2d&);
std::istream& operator>>(std::istream&, Color&);
std::istream& operator>>(std::istream&, Area&);
```

```cpp
std::istringstream iss("100,200");
rmk::Vec2d pos;
iss >> pos; // { 100, 200 }
```

---

## Building up `Text`

Rather than calling `write`/`append` repeatedly, a `Text` can be built by simply chaining `<<`, just like `std::cout`:

```cpp
Text& operator<<(Text&, Text&)            noexcept;
Text& operator<<(Text&, std::string_view) noexcept;
Text& operator<<(Text&, fmt)              noexcept;
```

- The `Text` overload appends the other `Text`'s content.
- The `std::string_view` overload appends the given string.
- The `fmt` overload appends a formatting token: `fmt::nl` for a newline, `fmt::tab` for spacing, `fmt::endl` to mark the next append as a
-  replacement rather than a continuation, `fmt::flush` to clear the text.

```cpp
rmk::Text label("arial", {100, 100});

label << "Score : " << fmt::nl << "0";
```

Since any type satisfying `IsBasicType` already knows how to print itself to an `ostream`, it can also be streamed directly into a `Text`
without manual conversion:

```cpp
template<IsBasicType T> Text& operator<<(Text&, const T&) noexcept;
```

```cpp
rmk::Vec2d pos = {50, 50};
label << "Position : " << pos; // uses operator<<(ostream&, Vec2d&) internally
```

The other way around, `operator>>` replaces a `Text`'s entire content with a line read from a stream:

```cpp
std::istream& operator>>(std::istream&, Text&) noexcept;
```

```cpp
std::istringstream iss("Hello world");
iss >> label;
```

---

## Saving and loading with `DataFile`

The same idea applies to saving: any type implementing `ISavable` can be saved or reloaded with `<<`/`>>`, as a shorthand for `save`/`load`:

```cpp
template<IsSavable T> DataFile& operator<<(DataFile&, const T&);
template<IsSavable T> DataFile& operator>>(DataFile&, T&);
```

```cpp
rmk::DataFile file("save1");

file << player; // equivalent to file.save
file >> player; // equivalent to file.load
```

---

[:octicons-arrow-left-24: Previous chapter](../data/loadtype.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](concept.md){ .md-button .md-button--primary }