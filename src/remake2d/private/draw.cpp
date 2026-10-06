#include <remake2d/private/draw.hpp>

#include <SDL2/SDL.h>

#include <limits>
#include <cmath>

namespace rmk {

void Printable::color(Color c) noexcept {
    if (m_color != c) {
        m_color       = c;
        is_draw_dirty = true;
        is_fill_dirty = true;
    }
}

void Printable::layer(i16 l) noexcept {
    m_layer  = l;
}

void Printable::from(const Printable& main) const noexcept {
    if (&main == this) return;
    _inheritData(main.color(), main.layer());
}

void Printable::_inheritData(Color c, i16 layer) const noexcept {

    if (overridden) return;

    m_layer  = layer;

    if (m_color != c) {
        m_color       = c;
        is_draw_dirty = true;
        is_fill_dirty = true;
    }
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
i16   Printable::layer(void)  const noexcept { return m_layer; }

namespace contour {

Vec2d breaker(void) noexcept {
    f32 inf = -std::numeric_limits<f32>::infinity();
    return Vec2d{ inf, inf };
}

bool isBreak(Vec2d p) noexcept {
    return std::isinf(p.x) && std::isinf(p.y) && p.x < 0.0f && p.y < 0.0f;
}

} // namespace contour

const std::vector<DrawPack>& Printable::_draw_(void) const {
    // Do NOT clear _draw_cache_ here: draw() relies on it already
    // holding last frame's content so clean children can be left
    // untouched and dirty ones patched in place (see Area::draw).
    // Wiping it unconditionally on every call would defeat that —
    // a clean child's overlay/copy would land on an empty cache.
    _draw_deep_ = 0;
    if (is_draw_dirty) {
        draw(*this);

        // A child that disappeared mid-collection is caught by the
        // next surviving child noticing the gap ahead of it (see
        // Area::draw's erase-on-gap comment) — but a child that
        // disappeared from the very END of a collection (e.g. a
        // Snake's tail, popped with nobody left after it to run and
        // notice) leaves nobody to trigger that erase. _draw_deep_
        // is the running total of everything that actually got
        // re-spliced this pass, so anything in _draw_cache_ beyond
        // that point is exactly that leftover tail.
        if (_draw_deep_ < _draw_cache_.size()) {
            _draw_cache_.erase(_draw_cache_.begin() + _draw_deep_, _draw_cache_.end());
        }
    }
    return _draw_cache_;
}

const std::vector<VertexBatch>& Printable::_fill_(void) const {
    // Same reasoning as _draw_() above, for _fill_cache_.
    _fill_deep_ = 0;
    if (is_fill_dirty) {
        fill(*this);
        if (_fill_deep_ < _fill_cache_.size()) {
            _fill_cache_.erase(_fill_cache_.begin() + _fill_deep_, _fill_cache_.end());
        }
    }
    return _fill_cache_;
}


} // namespace rmk