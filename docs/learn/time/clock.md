# Clock

A save file listing "played for 03:45:12" reads a lot better than a raw number of seconds; `Clock` handles that
hour/minute/second formatting the same way `Date` handles calendar dates.

---

## Overview

`Clock` is contained in the header **"remake2d/clock.hpp"**. It stores an hour, minute and second, and can parse 
or format itself from a string using a configurable separator and field order.

---

## Methods

```cpp
u8   hour(void)   const noexcept; // get hour
u8   minute(void) const noexcept; // get minute
u8   second(void) const noexcept; // get second
char slice(void)  const noexcept; // get current separator

void hour(const u8&)   noexcept; // set hour
void minute(const u8&) noexcept; // set minute
void second(const u8&) noexcept; // set second

void format(const std::string_view& fmt); // set display format (e.g. "hour:minute:second")
void time(const std::string_view& str);   // parse a time string
std::string time(void) const;             // format time to string

bool timeIs(const Clock& other) const noexcept; // compare with another time
```

---

## Usage

### Creating a clock

```cpp
Clock(u8 hour, u8 minute, u8 second);
```

```cpp
rmk::Clock playTime(3, 45, 12);
```

### Formatting and parsing

Format sets how the clock reads and writes itself, using `hour`, `minute` and `second` keywords:

```cpp
playTime.format("hour:minute:second");
std::string text = playTime.time(); // "03:45:12"
```

The same format parses a string back into a `Clock`:

```cpp
rmk::Clock parsed;
parsed.format("hour:minute:second");
parsed.time("03:45:12");
```

### Comparing times

timeIs compares two clocks directly, and is what powers the full set of comparison operators exposed through 
the [Math](../tools/math.md) module:

```cpp
if (playTime.timeIs(bestTime)) { /* tied with best time */ }
```

---

[:octicons-arrow-left-24: Previous chapter](date.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](../tools/math.md){ .md-button .md-button--primary }