#include <remake2d/area.hpp>

#include <SDL2/SDL.h>
#include <algorithm>

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

void Area::draw(const Printable& main) const noexcept {
    bool toRoot = (&main == static_cast<const Printable*>(this));

    // Self is clean AND (we're our own root, or we already hold a
    // stable, up-to-date slot in main from a previous frame): nothing
    // to regenerate and nothing to re-splice into mainCache — leave it
    // exactly as it was left last time. This is the case that makes
    // the whole mechanism cheap: most children, most frames, stop here.
    // deep still has to advance past our untouched slot so whoever is
    // called after us lands at the right spot.
    // Same reasoning as Area::fill.
    if (!is_draw_dirty && !toRoot && _draw_id_ >= 0 && _main_id_ == main._mainId()) {
        main._draw_deep_ += _last_point_count_;
        return;
    }
    if (!is_draw_dirty && toRoot) {
        main._draw_deep_ += _last_point_count_;
        return;
    }

    auto& cache     = _draw_cache_;
    auto& mainCache = main._draw_cache_;

    u32  deep        = main._draw_deep_;
    bool isFirstTime = _draw_id_ < 0 || _main_id_ != main._mainId();

    if (!toRoot) {
        main.drawn = true;
        _inheritData(main.color(), main.layer()); // inherit main's color

        if (_main_id_ != main._mainId()) {
            // force a fresh insert below instead of overlaying stale data.
            _main_id_          = main._mainId();
            _draw_id_          = -1;
            _last_point_count_ = 0;
        }

        // Gap in front of us = one or more earlier siblings vanished
        // since last frame (never called draw() again this pass, so
        // nobody erased their leftover packs). The running counter only
        // advances for siblings that actually ran, so if it's now
        // behind where we last recorded ourselves, that difference IS
        // their stale data sitting right before our slot. Erasing it
        // slides our own (and everyone after us) back into place.
        if (!isFirstTime && deep < (u32)_draw_id_) {
            mainCache.erase(mainCache.begin() + deep, mainCache.begin() + _draw_id_);
        }
    }

    if (is_draw_dirty) {
        cache.clear();
        cache.push_back(DrawPack{ color(), {
            Vec2d{ (f32)x,     (f32)y     },
            Vec2d{ (f32)(x+w), (f32)y     },
            Vec2d{ (f32)(x+w), (f32)(y+h) },
            Vec2d{ (f32)x,     (f32)(y+h) },
            Vec2d{ (f32)x,     (f32)y     }
        } });
        is_draw_dirty = false;
    }

    _current_point_count_ = (u32)cache.size();

    if (!toRoot) {
        if (isFirstTime) {
            mainCache.insert(mainCache.begin() + deep, cache.begin(), cache.end());
        } else {
            u32 overlap = std::min(_last_point_count_, _current_point_count_);
            std::copy(
                cache.begin(), cache.begin() + overlap,
                mainCache.begin() + deep
            );

            if (_last_point_count_ < _current_point_count_) {
                mainCache.insert(
                    mainCache.begin() + deep + _last_point_count_,
                    cache.begin() + _last_point_count_, cache.end()
                );
            } else if (_last_point_count_ > _current_point_count_) {
                mainCache.erase(
                    mainCache.begin() + deep + _current_point_count_,
                    mainCache.begin() + deep + _last_point_count_
                );
            }
        }

        _draw_id_          = (i32)deep;
    }

    // Runs for toRoot too: main._draw_deep_ must always reflect the
    // running total so _draw_() (the pass entry point) can trim any
    // leftover stale tail at the end — see its comment.
    _last_point_count_ = _current_point_count_;
    main._draw_deep_   = (toRoot ? 0 : deep) + _current_point_count_;

    drawn = true;
}

void Area::fill(const Printable& main) const noexcept {
    bool toRoot = (&main == static_cast<const Printable*>(this));

    // See Area::draw for why this early exit is both safe and the
    // common case: a clean child with a stable existing slot needs no
    // work at all beyond letting deep flow past it. The reparent check
    // (_main_fill_id_ mismatch) must gate this too — a clean object
    // hand fed to a *different* main still needs the full path below to
    // insert into that main's cache for the first time.
    if (!is_fill_dirty && !toRoot && _fill_id_ >= 0 && _main_fill_id_ == main._mainId()) {
        main._fill_deep_ += _last_vertex_count_;
        return;
    }
    if (!is_fill_dirty && toRoot) {
        main._fill_deep_ += _last_vertex_count_;
        return;
    }

    auto& cache     = _fill_cache_;
    auto& mainCache = main._fill_cache_;

    u32  deep        = main._fill_deep_;
    bool isFirstTime = _fill_id_ < 0 || _main_fill_id_ != main._mainId();

    if (!toRoot) {
        main.filled = true;
        _inheritData(main.color(), main.layer()); // inherit main's data

        if (_main_fill_id_ != main._mainId()) {
            // force a fresh insert below instead of overlaying stale data.
            _main_fill_id_      = main._mainId();
            _fill_id_           = -1;
            _last_vertex_count_ = 0;
        }

        if (!isFirstTime && deep < (u32)_fill_id_) {
            mainCache.erase(mainCache.begin() + deep, mainCache.begin() + _fill_id_);
        }
    }

    if (is_fill_dirty) {
        cache.clear();
        auto triangles = toTriangulation();

        VertexBatch batch;
        batch.texture = nullptr;
        batch.vertices.reserve(triangles.size() * 3);

        for (const auto& tri : triangles) {
            batch.vertices.push_back(Vertex{ tri.a.x, tri.a.y, color() });
            batch.vertices.push_back(Vertex{ tri.b.x, tri.b.y, color() });
            batch.vertices.push_back(Vertex{ tri.c.x, tri.c.y, color() });
        }

        cache.push_back(std::move(batch));
        is_fill_dirty = false;
    }

    _current_vertex_count_ = (u32)cache.size();

    if (!toRoot) {
        if (isFirstTime) {
            mainCache.insert(mainCache.begin() + deep, cache.begin(), cache.end());
        } else {
            u32 overlap = std::min(_last_vertex_count_, _current_vertex_count_);
            std::copy(
                cache.begin(), cache.begin() + overlap,
                mainCache.begin() + deep
            );

            if (_last_vertex_count_ < _current_vertex_count_) {
                mainCache.insert(
                    mainCache.begin() + deep + _last_vertex_count_,
                    cache.begin() + _last_vertex_count_, cache.end()
                );
            } else if (_last_vertex_count_ > _current_vertex_count_) {
                mainCache.erase(
                    mainCache.begin() + deep + _current_vertex_count_,
                    mainCache.begin() + deep + _last_vertex_count_
                );
            }
        }

        _fill_id_ = (i32)deep;
    }

    // Runs for toRoot too — see Area::draw's matching comment.
    _last_vertex_count_ = _current_vertex_count_;
    main._fill_deep_    = (toRoot ? 0 : deep) + _current_vertex_count_;

    filled = true;
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