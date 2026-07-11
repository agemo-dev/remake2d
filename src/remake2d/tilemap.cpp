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

TileMap::TileMap(std::string_view path, TileMapData data)
    : m_tileset(path, Rectangle(data.center, data.size)),
      m_data(data) {}

void TileMap::_buildClipPositions(void) noexcept {
    m_clip_positions.clear();
    i16 current_id = m_counter_start;
    for (int row = 0; row < m_data.cut.y; row++) {
        for (int col = 0; col < m_data.cut.x; col++) {
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

    int col = 0, row = 0;

    for (auto& id : m_template) {
        Vec2d pos = {
            m_data.center.x + col * (m_data.clip_size.w + m_data.margin),
            m_data.center.y + row * (m_data.clip_size.h + m_data.margin)
        };

        if (m_template_physic.count(id)) {
			m_bodies[id].push_back(m_template_physic.at(id));
			m_bodies[id].back().move(pos);
        }
		
        col++;
        if (col >= m_data.cut.x) { col = 0; row++; }
    }

    m_is_built = true;
}



void TileMap::_visibleRange(Vec2d world_tl, Dim2d world_view, Vec2d& col, Vec2d& row) const noexcept {
    f32 tw = m_data.clip_size.w + m_data.margin;
    f32 th = m_data.clip_size.h + m_data.margin;

    f32 local_x = world_tl.x - m_data.center.x;
    f32 local_y = world_tl.y - m_data.center.y;

    col.x = std::max(0, (int)(local_x / tw));
    row.x = std::max(0, (int)(local_y / th));

    col.y = std::min((int)m_data.cut.x, (int)(col.x + world_view.w / tw) + 2);
    row.y = std::min((int)m_data.cut.y, (int)(row.x + world_view.h / th) + 2);
}


void TileMap::_draw(Window& win, Color color) const noexcept {
    if (m_build_future.valid()) m_build_future.wait();
    int col = 0, row = 0;
    for (auto& id : m_template) {
        Vec2d pos = {
            m_data.center.x + col * (m_data.clip_size.w + m_data.margin),
            m_data.center.y + row * (m_data.clip_size.h + m_data.margin)
        };

        if (m_clip_positions.count(id)) {
            m_tileset.clip(m_clip_positions.at(id), m_data.clip_size);
            m_tileset.move(pos);
            win.draw(m_tileset, color);
        }

        col++;
        if (col >= m_data.cut.x) { col = 0; row++; }
    }
}


void TileMap::_draw(Window& win, Color color, const Camera& cam) const noexcept {
    if (m_build_future.valid()) m_build_future.wait();
    Vec2d world_tl   = cam.center();
    f32   zoom       = cam.zoom();
    Dim2d screen     = win.size();
    Dim2d world_view = { screen.w / zoom, screen.h / zoom };

    Vec2d col, row;
    _visibleRange(world_tl, world_view, col, row);


    f32 tw = m_data.clip_size.w + m_data.margin;
    f32 th = m_data.clip_size.h + m_data.margin;

    for (int rw = row.x; rw < row.y; rw++) {
        for (int cl = col.x; cl < col.y; cl++) {
            int    idx = rw * (int)m_data.cut.x + cl;
            TileID id  = m_template[idx];

            if (!m_clip_positions.count(id)) continue;

            Vec2d world_pos = {
                m_data.center.x + cl * tw,
                m_data.center.y + rw * th
            };


            Vec2d screen_pos = {
                (world_pos.x - world_tl.x) * zoom,
                (world_pos.y - world_tl.y) * zoom
            };


            Dim2d screen_tile = {
                m_data.clip_size.w * zoom,
                m_data.clip_size.h * zoom
            };

            m_tileset.clip(m_clip_positions.at(id), m_data.clip_size);
            m_tileset.move(screen_pos);
            m_tileset.resize(screen_tile);
            win.draw(m_tileset, color);
        }
    }
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