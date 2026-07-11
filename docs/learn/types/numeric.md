# Numeric Types

---

**RE:MAKE 2D** offers a wide variety of numeric types available in the
header **"remake2d/numeric.hpp"**. These types are organized as follows:

##Kind Type
| **Category** | **Type** |
|---|---|
|Signed integers | `i8`, `i16`, `i32`, `i64`, `imax` |
|Signed least integers | `ileast8`, `ileast16`, `ileast32`, `ileast64` |
|Unsigned integers | `byte`, `rune`, `u8`, `u16`, `u32`, `u64`, `umax` |
|Unsigned least integers | `uleast8`, `uleast16`, `uleast32`, `uleast64` |
|Signed fast integers | `ifast8`, `ifast16`, `ifast32`, `ifast64` |
|Unsigned fast integers | `ufast8`, `ufast16`, `ufast32`, `ufast64` |
|Floating-point | `f32`, `f64`, `fmax` |
|Pointers and size | `usize`, `ptrdiff`, `addr` |
| | |

---

## Usage

```cpp
#include <remake2d/numeric.hpp>

int main(void) {

    rmk::i32 signed_integer          = 42000; // stored on 32 bits
    rmk::u64 unsigned_long_integer   = 200; // stored on 64 bits
    rmk::ifast32 fast_integer        = 2300; // stored on 32 bits with fast access
    rmk::byte byt   = 100; // stored on 8 bits, equivalent to a char
    rmk::rune run   = 50000; // stored on 16 bits
    rmk::f32 flt    = 25.4f; // stored on 32 bits
    rmk::f64 dbl    = 45.03005; // stored on 64 bits

}
```

!!! info
    The `byte` and `rune` types are equivalent to `u8` and `u16`.

!!! warning
    The `f32` and `f64` types are mostly stored on **32 and 64 bits**, but
    this may vary in rare cases.

---

## Arithmetic

The numeric types of RE:MAKE 2D support all the mathematical operations
available to their **C numeric** equivalents (`int`, `char`, `float`, `double`, …).
Example:

```cpp
#include <remake2d/numeric.hpp>

int main(void) {
    rmk::i32 num = 100;
    rmk::f64 dbl = 10.505;
    
    num += 10; // 110
    num = num - 50; // 60
    num = num * num; // 360
    
    dbl = dbl - 0.005; // 10.500;
    dbl = dbl % 10; // ERROR: modulo cannot be applied to double
    
}
```

---

[Next chapter :octicons-arrow-right-24:](vector.md){ .md-button .md-button--primary }