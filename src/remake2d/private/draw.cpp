#include <remake2d/private/draw.hpp>

#include <SDL2/SDL.h>

#include <limits>
#include <cmath>

namespace rmk {

void Printable::color(Color c) noexcept {
	_color(c);
}

void Printable::_color(Color c) const noexcept {
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

Vec2d breaker(void) noexcept {
	f32 inf = -std::numeric_limits<f32>::infinity();
	return Vec2d{ inf, inf };
}

bool isBreak(Vec2d p) noexcept {
	return std::isinf(p.x) && std::isinf(p.y) && p.x < 0.0f && p.y < 0.0f;
}

} // namespace contour

const std::vector<DrawPack>& Drawable::_draw_(void) const {
	if (is_draw_dirty) {
		_draw_cache_.clear();
		draw(*this);
	}
	return _draw_cache_;
}

const std::vector<VertexBatch>& Fillable::_fill_(void) const {
		if (is_fill_dirty) {
			_fill_cache_.clear();
			fill(*this);
		}
		return _fill_cache_;
}


} // namespace rmk
