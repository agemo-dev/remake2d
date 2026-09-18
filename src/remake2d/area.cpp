#include <remake2d/area.hpp>

#include <SDL2/SDL.h>

namespace rmk {

Vec2d Area::pos(void) const noexcept {
    return { (f32)x, (f32)y };
}

Dim2d Area::size(void) const noexcept {
    return { (f32)w, (f32)h };
}

Vec2d Area::center(void) const noexcept {
    return { (f32)(x + w / 2), (f32)(y + h / 2) };
}

std::array<Triangulation, 2> Area::toTriangulation(void) const noexcept {
    Vec2d topLeft     { (f32)x,     (f32)y };
    Vec2d topRight    { (f32)(x+w), (f32)y };
    Vec2d bottomLeft  { (f32)x,     (f32)(y+h) };
    Vec2d bottomRight { (f32)(x+w), (f32)(y+h) };

    return {
        Triangulation{ topLeft, topRight, bottomRight   },
        Triangulation{ topLeft, bottomRight, bottomLeft }
    };
}

void Area::draw(const Drawable& main) const noexcept {
    if (!is_draw_dirty) return;

    if (&main != static_cast<const Drawable*>(this)) {
        main.is_draw_dirty = false;
        main.drawn         = true;
        _color(main.color());
    }

    main._draw_cache_.push_back(DrawPack{ color(), {
        Vec2d{ (f32)x,     (f32)y     },
        Vec2d{ (f32)(x+w), (f32)y     },
        Vec2d{ (f32)(x+w), (f32)(y+h) },
        Vec2d{ (f32)x,     (f32)(y+h) },
        Vec2d{ (f32)x,     (f32)y     }
    } });
    is_draw_dirty = false;
    drawn         = true;
}

void Area::fill(const Fillable& main) const noexcept {
    if (!is_fill_dirty) return;

    if (&main != static_cast<const Fillable*>(this)) {
        main.is_fill_dirty = false;
        main.filled        = true;
        _color(main.color());
    }

    auto triangles = toTriangulation();

    VertexBatch batch;
    batch.texture = nullptr;
    batch.vertices.reserve(triangles.size() * 3);

    for (const auto& tri : triangles) {
        batch.vertices.push_back(Vertex{ tri.a.x, tri.a.y, color() });
        batch.vertices.push_back(Vertex{ tri.b.x, tri.b.y, color() });
        batch.vertices.push_back(Vertex{ tri.c.x, tri.c.y, color() });
    }

    main._fill_cache_.push_back(batch);
    is_fill_dirty = false;
    filled        = true;
}

bool Area::operator<(const Area& other)  const noexcept {
    return this->w < other.w && this->h < other.h;
}

bool Area::operator>(const Area& other)  const noexcept {
    return this->w > other.w && this->h > other.h;
}

bool Area::operator==(const Area& other) const noexcept {
    return this->w == other.w && this->h == other.h;
}

bool Area::operator<=(const Area& other) const noexcept {
    return this->w <= other.w && this->h <= other.h;
}

bool Area::operator>=(const Area& other) const noexcept {
    return this->w >= other.w && this->h >= other.h;
}

Area::operator SDL_Rect(void) const { return SDL_Rect{ x, y, w, h }; }

} // namespace rmk