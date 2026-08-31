#ifndef REMAKE2D_COLOR_
#define REMAKE2D_COLOR_

#include <remake2d/concept.hpp>
#include <remake2d/numeric.hpp>
#include <remake2d/config/forward.hpp>

#include <compare>

namespace rmk {

struct Color {

public:
    byte r{0};
    byte g{0};
    byte b{0};
    byte a{255};

public:
    constexpr Color(byte, byte, byte, byte = 255);
    constexpr Color(void)                       = default;
    constexpr Color(Color&&)                    = default;
    constexpr Color(const Color&)               = default;
    constexpr Color& operator=(Color&&)         = default;
    constexpr Color& operator=(const Color&)    = default;

private:
    constexpr SDL_Color _data(void) const noexcept;

public:
    constexpr auto operator<=>(const Color&) const noexcept = default;

private:
    friend class Window;
    friend class Text;
    friend class FontManager;
    template<IsShape S> friend class Texture;
};

// HSL : Hue - Saturation - Luminosity
struct HSL {
public:
    f32 h{0.0f};
    f32 s{0.0f};
    f32 l{0.0f};

public:
    constexpr HSL(f32, f32, f32);
    constexpr HSL(void)                   = default;
    constexpr HSL(HSL&&)                  = default;
    constexpr HSL(const HSL&)             = default;
    constexpr HSL& operator=(HSL&&)       = default;
    constexpr HSL& operator=(const HSL&)  = default;

public:
    constexpr auto operator<=>(const HSL&) const noexcept = default;
};

namespace color {

inline constexpr Color red       = { 255, 0,   0,   255 };
inline constexpr Color green     = {   0, 230, 50,  255 };
inline constexpr Color blue      = {   0, 0,   255, 255 };
inline constexpr Color skyblue   = {   0, 191, 255, 255 };
inline constexpr Color purple    = { 138, 43,  226, 255 };
inline constexpr Color orange    = { 249, 115,  22, 255 };
inline constexpr Color yellow    = { 255, 215, 0,   255 };
inline constexpr Color gray      = { 128, 128, 128, 255 };
inline constexpr Color gold      = { 255, 203,   0, 255 };
inline constexpr Color darkblue  = {   0, 80,  180, 255 };
inline constexpr Color darkgreen = {   0, 120,  50, 255 };
inline constexpr Color lime      = {  34, 182, 212, 255 };
inline constexpr Color silver    = { 192, 192, 192, 255 };
inline constexpr Color maroon    = { 128, 0,   0,   255 };
inline constexpr Color pink      = { 255, 0,   128, 255 };
inline constexpr Color fuchsia   = { 255, 0,   255, 255 };
inline constexpr Color aqua      = {   0, 255, 255, 255 };
inline constexpr Color raywhite  = { 245, 245, 245, 255 };
inline constexpr Color cyan      = {   6, 182, 212, 255 };
inline constexpr Color emerald   = {   5, 150, 105, 255 };
inline constexpr Color teal      = {  13, 148, 136, 255 };
inline constexpr Color amber     = { 217, 119,   6, 255 };
inline constexpr Color indigo    = {  79,  70, 229, 255 };
inline constexpr Color black     = {   0, 0,   0,   255 };
inline constexpr Color white     = { 255, 255, 255, 255 };

} // namespace color
} // namespace rmk
#endif