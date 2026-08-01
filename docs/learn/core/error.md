# Error

Errors are essential elements for controlling and limiting the use
of the engine's features.

---

## Overview

Errors are represented by the base class `Error`, which inherits
from the standard library's `std::exception`.

---

## Usage

### Throw

The `Error` class is used in the same way as standard exceptions,
via a `throw` :

```cpp
try {
    throw rmk::Error("error message");
}
catch (const rmk::Error& e) {
    std::cerr << e.what() << std::endl;
}
```

### Assertion

**RE:MAKE 2D** provides a macro for raising an error with a custom message
and detailed contextual information :

```cpp
#define rmk_dynamicAssert(type, msg) \
    do {\
        type err(msg); \
        err.info = std::source_location::current(); \
        std::cerr   << "\033[31m[!] error generate : "\
                    << "\n\033[34m> file     : \033[0m" << err.info.file_name() \
                    << "\n\033[34m> line     : \033[0m" << err.info.line() \
                    << "\n\033[34m> function : \033[0m" << err.info.function_name() \
                    << "\n\033[34m> type     : \033[0m" << #type \
                    << "\n\033[34m> info     : \033[0m" << err.what() << "\n"; \
                std::exit(1); \
    } while(0)
```

This macro is used internally by all engine modules. It is invoked
as follows :

```cpp
rmk_dynamicAssert(rmk::Error, "error message");
```

Unlike a standard `throw`, it provides detailed contextual information —
file, line, function — but the error cannot be caught.

---

## Error types

**RE:MAKE 2D** exposes **4 public error sub-types**, all inheriting from
`Error`, designed to cover the most common use cases :

```cpp
class LoadingError;    // file or resource loading failure
class LogicError;      // usage contrary to the module's expected logic
class RunTimeError;    // undefined error occurring at runtime
class OutOfRangeError; // out-of-range access
```

In addition, each engine module defines its own error type, allowing
precise identification of the problem's origin :

```cpp
class SystemError;
class WindowError;
class DataError;
class DateError;
class ClockError;
class LayerError;
class SceneError;
class CroutineError;
class SoundError;
class TextureError;
class ScriptError;
class PhysicError;
class TileMapError;
class SignalError;
```

!!! warning
    These types are reserved for internal use and are not intended
    to be used directly by the user.

---

[:octicons-arrow-left-24: Previous chapter](../data/script.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](signal.md){ .md-button .md-button--primary }