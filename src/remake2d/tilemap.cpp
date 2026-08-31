#include <remake2d/tilemap.hpp>
#include <remake2d/camera.hpp>
#include <remake2d/physic.hpp>
#include <remake2d/texture.hpp>
#include <remake2d/window.hpp>
#include <remake2d/error.hpp>

#include <filesystem>
#include <utility>
#include <algorithm>
#include <chrono>
#include <string>

namespace rmk {

TileMapData::TileMapData(Vec2d c, Dim2d s, Grid2d ct, Dim2d csize, Vec2d cstart, u32 marg)
	: center(c), size(s), cut(ct), clip_size(csize), clip_start(cstart), margin(marg)   {}

TileMap::TileMap(std::string_view path, TileMapData data) : m_data(data)
    , m_tileset(path, Rectangle(data.center, data.size)) {}

void TileMap::_buildClipPositions(void) noexcept {
    m_clip_positions.clear();
    i16 current_id = m_counter_start;
    for (usize row = 0; row < m_data.cut.y; row++) {
        for (usize col = 0; col < m_data.cut.x; col++) {
            m_clip_positions[current_id] = {
                m_data.clip_start.x + col * (m_data.clip_size.w + m_data.margin),
                m_data.clip_start.y + row * (m_data.clip_size.h + m_data.margin)
            };
            current_id++;
        }
    }
}

void TileMap::counterStart(i16 start) noexcept {
    m_counter_start = start;
    _buildClipPositions();
}

void TileMap::load(TileTemplate map_template) {
    usize expected = m_data.cut.x * m_data.cut.y;
    if(map_template.size() != expected) {
        rmk_dynamicAssert(rmk::TileMapError, error::tilemap::unexcepted_tile_template);
    }
    m_id_count.clear();
    m_template = std::move(map_template);
    for (auto& id : m_template)
        m_id_count[id]++;
}

void TileMap::build(void) noexcept {
    m_is_built = false;
    m_build_future = std::async(std::launch::async, [this]() {
        _applyAttributes();
    });
}

void TileMap::_applyAttributes(void) noexcept {

    usize col = 0, row = 0;
    m_body_slot.assign(m_template.size(), NO_BODY_SLOT);

    for (usize idx = 0; idx < m_template.size(); idx++) {
        TileID id = m_template[idx];
        Vec2d pos = {
            m_data.center.x + col * (m_data.clip_size.w + m_data.margin),
            m_data.center.y + row * (m_data.clip_size.h + m_data.margin)
        };

        if (m_template_physic.count(id)) {
			m_bodies[id].push_back(m_template_physic.at(id));
			m_bodies[id].back().move(pos);
            m_body_slot[idx] = (u32)(m_bodies[id].size() - 1);
        }

        col++;
        if (col >= m_data.cut.x) { col = 0; row++; }
    }

    m_is_built = true;
}



void TileMap::fill(const Fillable& main) const noexcept {
    if (m_build_future.valid()) m_build_future.wait();

    std::vector<VertexBatch> batches;
    batches.reserve(m_template.size());

    usize col = 0, row = 0;
    f32 tw = m_data.clip_size.w + m_data.margin;
    f32 th = m_data.clip_size.h + m_data.margin;

    for (usize idx = 0; idx < m_template.size(); idx++) {
        TileID id = m_template[idx];
        Vec2d pos = {
            m_data.center.x + col * tw,
            m_data.center.y + row * th
        };

        StaticBody* tile_body = _bodyAt(idx);

        if (tile_body != nullptr && !tile_body->m_animations.empty()) {
            Animation& anim = tile_body->animation();
            anim.move(pos);
            auto batch = anim.__fill__();
            batches.insert(batches.end(), batch.begin(), batch.end());
        } else if (m_clip_positions.count(id)) {
            m_tileset.clip(m_clip_positions.at(id), m_data.clip_size);
            m_tileset.move(pos);
            auto batch = m_tileset.__fill__();
            batches.insert(batches.end(), batch.begin(), batch.end());
        }

        col++;
        if (col >= m_data.cut.x) { col = 0; row++; }
    }

    main.__fill_cache__ = std::move(batches);
}

StaticBody* TileMap::_bodyAt(usize tile_index) const noexcept {
    if (tile_index >= m_body_slot.size()) return nullptr;
    u32 slot = m_body_slot[tile_index];
    if (slot == NO_BODY_SLOT) return nullptr;

    TileID id = m_template[tile_index];
    auto   it = m_bodies.find(id);
    if (it == m_bodies.end() || slot >= it->second.size()) return nullptr;
    return &it->second[slot];
}

void TileMap::applyPhysic(TileID id) {
    auto it = m_clip_positions.find(id);
    if (it == m_clip_positions.end()) {
        rmk_dynamicAssert(rmk::TileMapError, error::tilemap::undefined_id);
    }
    m_template_physic[id] = StaticBody(m_tileset.shape());
}

void TileMap::applyPhysic(std::string_view tag) {
	std::string t(tag);
    auto it = m_tags.find(t);
    if (it == m_tags.end()) {
        rmk_dynamicAssert(rmk::TileMapError, error::tilemap::undefined_tag);
    }

	applyPhysic(it->second);
}

PhysicBody& TileMap::body(TileID id) {
    auto it = m_template_physic.find(id);
    if (it == m_template_physic.end()) {
        rmk_dynamicAssert(rmk::TileMapError, error::tilemap::undefined_id);
    }
    return it->second;
}

PhysicBody& TileMap::body(std::string_view tag) {
    std::string t(tag);
    auto it = m_tags.find(t);
    if (it == m_tags.end()) {
        rmk_dynamicAssert(rmk::TileMapError, error::tilemap::undefined_tag);
    }
    return body(it->second);
}

void TileMap::tag(std::string_view name, TileID id) {
    m_tags[std::string(name)] = id;
}

u32 TileMap::tileCount(void) const noexcept {
    return m_template.size();
}

u32 TileMap::tileCount(TileID id) const noexcept {
    auto it = m_id_count.find(id);
    if (it == m_id_count.end()) return 0;
    return it->second;
}

Vec2d TileMap::center(void)  const noexcept { return m_data.center; }
Dim2d TileMap::size(void)    const noexcept { return m_data.size;   }
Dim2d TileMap::clip(void)    const noexcept { return m_data.clip_size; }
Grid2d TileMap::cut(void)    const noexcept { return m_data.cut; }

void TileMap::move(Vec2d center) noexcept {
    m_data.center = center;
    m_tileset.move(center);
}

void TileMap::resize(Dim2d size) noexcept {
    m_data.size = size;
    m_tileset.resize(size);
}

} //namespace rmk