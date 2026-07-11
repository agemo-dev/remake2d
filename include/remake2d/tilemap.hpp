#ifndef REMAKE2D_TILEMAP_
#define REMAKE2D_TILEMAP_

#include <remake2d/vector.hpp>
#include <remake2d/numeric.hpp>
#include <remake2d/texture.hpp>
#include <remake2d/concept.hpp>
#include <remake2d/config/forward.hpp>

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <future>
#include <utility>
#include <algorithm>

struct SDL_Renderer;

namespace rmk {

struct TileMapData {
    Vec2d  center;
    Dim2d  size;
    Vec2d  clip_start;
    Dim2d  clip_size;
    Grid2d cut;
    u8     margin{0};
};


class TileMap {

public:
    using TileID       = i16;
    using TileTemplate = std::vector<TileID>;
    using ClipMap      = std::map<TileID, Vec2d>;
    using CountMap     = std::map<TileID, u32>;
    using PhysicMap    = std::map<TileID, StaticBody>;
    using BodyList     = std::map<TileID, std::vector<StaticBody>>;
    using TagMap       = std::map<std::string, TileID>;

private:
    SDL_Renderer*      m_renderer;
    TileTemplate       m_template;
    TileMapData        m_data;
    ClipMap            m_clip_positions;
    CountMap           m_id_count;
    PhysicMap          m_template_physic;
    BodyList           m_bodies;
    TagMap             m_tags;
    i16                m_counter_start{0};
    bool               m_is_built{false};
    mutable Sprite     m_tileset;
    std::future<void>  m_build_future;

public:
    TileMap(std::string_view, TileMapData);
    TileMap(void)                        = delete;
    TileMap(TileMap&&)                   = default;
    TileMap(const TileMap&)              = delete;
    TileMap& operator=(TileMap&&)        = default;
    TileMap& operator=(const TileMap&)   = delete;

public:
    Dim2d  clip(void) 	   const noexcept;
    Dim2d  size(void)      const noexcept;
    Vec2d  center(void)    const noexcept;
    Grid2d cut(void)   	   const noexcept;
    u32  tileCount(void)   const noexcept;
    u32  tileCount(TileID) const noexcept;
	
public:
    void   move(Vec2d)       noexcept;
    void   resize(Dim2d)     noexcept;
    void   counterStart(i16) noexcept;
    void tag(std::string_view, TileID);

public:
    void load(TileTemplate);
    void build(void) noexcept;

public:
    void applyPhysic(TileID);
    void applyPhysic(std::string_view);

    PhysicBody& body(TileID);
    PhysicBody& body(std::string_view);

private:
    void _draw(Window&, Color) 					const noexcept;
    void _draw(Window&, Color, const Camera&)	const noexcept;

private:
    void _applyAttributes(void)		noexcept;
    void _buildClipPositions(void)	noexcept;
    void _visibleRange(Vec2d, Dim2d, Vec2d&, Vec2d&) const noexcept;

public:
    ~TileMap(void) = default;

public:
    friend class Window;
};

} // namespace rmk

#endif