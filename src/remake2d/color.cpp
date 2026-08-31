#include <remake2d/color.hpp>

#include <SDL2/SDL.h>

namespace rmk {

constexpr Color::Color(byte R, byte G, byte B, byte A) : r(R), g(G), b(B), a(A) {}
constexpr SDL_Color Color::_data(void) const noexcept { return SDL_Color{ r, g, b, a }; }

constexpr HSL::HSL(f32 H, f32 S, f32 L) : h(H), s(S), l(L) {}

} // namespace rmk