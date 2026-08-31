#ifndef REMAKE2D_TILEMAP_
#define REMAKE2D_TILEMAP_

#include <remake2d/draw.hpp>
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
public:
    Vec2d  center;
    Dim2d  size;
    Grid2d cut;
    Dim2d  clip_size;
    Vec2d  clip_start{0};
    u8     margin{0};

public:
	TileMapData(void)                           = default;
	TileMapData(TileMapData&&)                  = default;
	TileMapData(const TileMapData&)             = default;
	TileMapData& operator=(TileMapData&&)       = default;
	TileMapData& operator=(const TileMapData&)  = default;
	TileMapData(Vec2d, Dim2d, Grid2d, Dim2d, Vec2d = 0, u32 = 0);
};

class TileMap : public Fillable {

public:
    static constexpr u32 NO_BODY_SLOT = ~0u;

public:
	using TileID       = u32;
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
    mutable BodyList   m_bodies;
    std::vector<u32>   m_body_slot;
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
    void _applyAttributes(void)		noexcept;
    void _buildClipPositions(void)	noexcept;
    StaticBody* _bodyAt(usize tile_index) const noexcept;

private:
    void fill(const Fillable&) const noexcept override;

public:
    ~TileMap(void) = default;

public:
    friend class Window;
};

} // namespace rmk

#endif