# Sound

A game without sound feels lifeless fast, whether it's the punch of a jump effect or the mood set by background music.

---

## Overview

The sound module handles both short sound effects and background music, through two types sharing the same abstract base 
`Sound`, contained in the header **"remake2d/sound.hpp"**: `Music` and `SFX`.

---

## Methods

The methods shared by every `Sound` are as follows:

```cpp
i8 playFor(void)   const noexcept; // remaining loops
u8 getVolume(void) const noexcept; // current volume

virtual void play(i8 loop = 0) = 0; // start playback
virtual void stop(void)        = 0; // stop playback
virtual void pause(void)       = 0; // pause playback
virtual void resume(void)      = 0; // resume playback
virtual void volume(u8) noexcept = 0; // set volume (1-128)

Signal<> onFinish;
```

`SFX` additionally exposes:

```cpp
void stopAll(void) noexcept;
```

---

## Usage

### Loading music and sound effects

```cpp
Music(std::string_view path, u8 volume = 64);
SFX(std::string_view path, u8 volume = 64);
```

`path` is the relative path to the audio file, resolved from the executable:

```cpp
rmk::Music theme("theme.mp3");
rmk::SFX   jump("jump.wav");
```

!!! info
    **RE:MAKE 2D** supports OGG, MP3 and FLAC formats for music, and WAV for sound effects.

### Playing

Play accepts a loop count: `0` plays once, a positive number repeats that many times, `-1` loops forever:

```cpp
theme.play(-1); // endless background music
jump.play(1);   // play twice
```

Both `Music` and `SFX` are copyable and movable, so an `SFX` instance can be reused freely across multiple calls to play.

### Volume

Volume ranges from `1` to `128`, independent of the host device's system volume, and is also exposed through the `volume` namespace:

```cpp
namespace volume {
    inline constexpr u8 min = 1;
    inline constexpr u8 max = 128;
}
```

```cpp
theme.volume(90);
```

### Pausing, resuming and stopping

```cpp
theme.pause();
theme.resume();
theme.stop();
```

### Multiple simultaneous sound effects

Each call to play on an `SFX` allocates that instance to a distinct audio channel, which means triggering the same `SFX` in rapid 
succession can result in several overlapping playbacks at once. The regular `stop` method only affects the most recent one, which is
awhere stopAll comes in: it stops every channel currently occupied by that instance.

```cpp
footstep.play();
footstep.play(); // second overlapping playback

footstep.stopAll(); // stops both
```

### Reacting to playback finishing

`onFinish` fires once playback completes, useful for chaining tracks or triggering follow-up logic:

```cpp
theme.onFinish.join([&]() {
    playNextTrack();
});
```

---

[:octicons-arrow-left-24: Previous chapter](../texture/animation.md){ .md-button }
[Next chapter :octicons-arrow-right-24:](../physic/body.md){ .md-button .md-button--primary }