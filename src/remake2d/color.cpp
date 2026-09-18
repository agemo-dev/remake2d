#include <remake2d/color.hpp>

#include <SDL2/SDL.h>

namespace rmk {

Color::operator SDL_Color (void) const noexcept { return SDL_Color{ r, g, b, a }; }

} // namespace rmk