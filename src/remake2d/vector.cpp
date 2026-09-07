#include <remake2d/vector.hpp>

#include <SDL2/SDL.h>

namespace rmk {

Vec2d::operator  SDL_FPoint(void) const { return SDL_FPoint{ x, y }; }
Fact2d::operator SDL_FPoint(void) const { return SDL_FPoint{ x, y }; }
Grid2d::operator SDL_Point(void)  const { return SDL_Point{ (int)x, (int)y }; }
Area::operator   SDL_Rect(void)   const { return SDL_Rect{ x, y, w, h }; }

std::array<Triangulation, 2> Area::toTriangulation(void) const noexcept {
    Vec2d topLeft     { (f32)x,     (f32)y };
    Vec2d topRight    { (f32)(x+w), (f32)y };
    Vec2d bottomLeft  { (f32)x,     (f32)(y+h) };
    Vec2d bottomRight { (f32)(x+w), (f32)(y+h) };

    return {
        Triangulation{ topLeft, topRight, bottomRight },
        Triangulation{ topLeft, bottomRight, bottomLeft }
    };
}

void Area::draw(const Drawable& main) noexcept {
    if (!is_draw_dirty) return;

    if (&main != this) {
        main.is_draw_dirty = false;
        main.drawn        = true;
        color(main.color());
    }

    main._draw_cache_.push_back_back(DrawPack{ color(), {
        SDL_FPoint{ (f32)x,     (f32)y     },
        SDL_FPoint{ (f32)(x+w), (f32)y     },
        SDL_FPoint{ (f32)(x+w), (f32)(y+h) },
        SDL_FPoint{ (f32)x,     (f32)(y+h) },
        SDL_FPoint{ (f32)x,     (f32)y     }
    } });
    is_draw_dirty = false;
    drawn         = true;
}

void Area::fill(const Fillable& main) noexcept {
    if (!is_fill_dirty) return;

    if (&main != this) {
        main.is_fill_dirty = false;
        main.filled        = true;
        color(main.color());
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

} // namespace rmk
