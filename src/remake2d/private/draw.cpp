#include <remake2d/private/draw.hpp>

#include <SDL2/SDL.h>

#include <limits>
#include <cmath>

namespace rmk {

void Printable::color(Color c) noexcept {
	m_color         = c;
	m_is_dirty      = true;
	m_is_fill_dirty = true;
}

Vertex::operator SDL_Vertex(void) const {
	SDL_Vertex sdlv;
	sdlv.position.x  = x;
	sdlv.position.y  = y;
	sdlv.color.r     = color.r;
	sdlv.color.g     = color.g;
	sdlv.color.b     = color.b;
	sdlv.color.a     = color.a;
	sdlv.tex_coord.x = u;
	sdlv.tex_coord.y = v;
	return sdlv;
}

Color Printable::color(void) const noexcept { return m_color; }

namespace contour {

SDL_FPoint breaker(void) noexcept {
	f32 inf = -std::numeric_limits<f32>::infinity();
	return SDL_FPoint{ inf, inf };
}

bool isBreak(SDL_FPoint p) noexcept {
	return std::isinf(p.x) && std::isinf(p.y) && p.x < 0.0f && p.y < 0.0f;
}

} // namespace contour

const std::vector<DrawPack>& Drawable::__draw__(void) const {
        draw(*this);
		return __draw_cache__;
}

const std::vector<VertexBatch>& Fillable::__fill__(void) const {
		fill(*this);
		return __fill_cache__;
}


} // namespace rmk
