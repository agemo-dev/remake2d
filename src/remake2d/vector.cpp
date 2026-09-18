#include <remake2d/vector.hpp>

#include <SDL2/SDL.h>

namespace rmk {

Vec2d::operator  Fact2d(void) { return Fact2d{ (x < 0 ? 0 : x), (y < 0 ? 0 : y) }; }
Vec2d::operator  Grid2d(void) { return Grid2d{ (usize)x , (usize)y }; }
Vec2d::operator  SDL_FPoint(void) const { return SDL_FPoint{ x, y }; }
Fact2d::operator SDL_FPoint(void) const { return SDL_FPoint{ x, y }; }
Grid2d::operator SDL_Point(void)  const { return SDL_Point{ (int)x, (int)y }; }

} // namespace rmk