# Date

Displaying a save's creation date, or comparing two dates to sort a leaderboard, needs a type that 
understands calendar values, not just three raw integers.

---

## Overview

`Date` is contained in the header **"remake2d/clock.hpp"**. It stores a day, month and year, and can 
parse or format itself from a string using a configurable separator and field order.

---

## Methods

```cpp
u8   day(void)   const noexcept; // get day
u8   month(void) const noexcept; // get month
i32  year(void)  const noexcept; // get year
char slice(void) const noexcept; // get current separator character

void day(u8)   noexcept; // set day
void month(u8) noexcept; // set month
void year(i32) noexcept; // set year

void format(const std::string_view& fmt); // set display format (e.g. "day/month/year")
void date(const std::string_view& str);   // parse a date string
std::string date(void) const;             // format date to string

bool dayIs(const Date& other) const noexcept; // compare with another date
```

---

## Usage

### Creating a date

```cpp
Date(u8 day, u8 month, i32 year);
```

```cpp
rmk::Date release(9, 7, 2026);
```

### Formatting and parsing

Format sets how a date reads and writes itself as a string, using `day`, `month` and `year` keywords, 
along with an optional `Month`/`Mon` for a written-out month name:

```cpp
release.format("day/Mon/year");
std::string text = release.date(); // "09/Jul/2026"
```

The same format is used to parse a string back into a `Date`:

```cpp
rmk::Date parsed;
parsed.format("day/month/year");
parsed.date("09/07/2026");
```

!!! info
    The separator is deduced automatically from whichever character appears in the format or the parsed 
	string, among `-`, `/`, `.` and space.

### Comparing dates

`dayIs` compares two dates directly, and is what powers the full set of comparison operators exposed 
through the [Math](../tools/math.md) module:

```cpp
if (release.dayIs(today)) { /* releases today */ }
```

---

[:octicons-arrow-left-24: Previous chapter](chrono.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](clock.md){ .md-button .md-button--primary }