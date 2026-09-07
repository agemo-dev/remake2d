#include <remake2d/private/draw.hpp>

#include <SDL2/SDL.h>

#include <limits>
#include <cmath>

namespace rmk {

constexpr Vertex::Vertex(f32 X, f32 Y, Color C, f32 U, f32 V) : x(X), y(Y), color(C), u(U), v(V) {}

void Printable::color(Color c) noexcept {
	m_color           = c;
	is_draw_dirty = true;
	is_fill_dirty = true;
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

Color Printable::color(void)  const noexcept { return m_color; }

namespace contour {

SDL_FPoint breaker(void) noexcept {
	f32 inf = -std::numeric_limits<f32>::infinity();
	return SDL_FPoint{ inf, inf };
}

bool isBreak(SDL_FPoint p) noexcept {
	return std::isinf(p.x) && std::isinf(p.y) && p.x < 0.0f && p.y < 0.0f;
}

} // namespace contour

const std::vector<DrawPack>& Drawable::_draw_(void) {
	if (is_draw_dirty) {
		_draw_cache_.clear();
		draw(*this);
	}
	return _draw_cache_;
}

const std::vector<VertexBatch>& Fillable::_fill_(void) {
		if (is_fill_dirty) {
			_fill_cache_.clear();
			fill(*this);
		}
		return _fill_cache_;
}


} // namespace rmk
