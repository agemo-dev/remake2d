#include <remake2d/tilegrid.hpp>
#include <remake2d/window.hpp>

namespace rmk {

TileGrid::TileGrid(const Vec2d& center, const Dim2d& size, const Grid2d& cut)
        : m_center(center), m_size(size), m_cut(cut) {
    _build();
}

void TileGrid::move(const Vec2d& center) noexcept {
    m_center = center;
    _build();
}

void TileGrid::cut(const Grid2d& cut) noexcept {
    m_cut = cut;
    _build();
}

void TileGrid::resize(const Dim2d& size) noexcept {
    m_size = size;
    _build();
}

usize TileGrid::count(void) const noexcept {
    return m_cut.x * m_cut.y;
}

Dim2d TileGrid::size(void) const noexcept {
    return m_size;
}

Grid2d TileGrid::cut(void) const noexcept {
    return m_cut;
}

Vec2d TileGrid::center(void) const noexcept {
    return m_center;
}

Area TileGrid::cell(const Grid2d& coo) const noexcept {
    return m_cells[coo.y * m_cut.x + coo.x];
}

std::vector<Area> TileGrid::cells(void) const noexcept {
    return m_cells;
}

void TileGrid::_build(void) noexcept {
    int count = int(m_cut.x * m_cut.y);
    m_cells.clear();
    Vec2d vlast = { 0, 0 };
    Dim2d csize = { m_size.w / m_cut.x, m_size.h / m_cut.y };
    Vec2d start = { m_center.x - m_size.w / 2, m_center.y - m_size.h / 2 };
    
    for(int i = 0; i < count; i++) {
        vlast.x = i % m_cut.x;
        vlast.y = i / m_cut.x;
        Area cell = {   i32(start.x + vlast.x * csize.w), 
                        i32(start.y + vlast.y * csize.h),
                        i32(csize.w), i32(csize.h)
                    };
        m_cells.push_back(cell);
    }
}

void TileGrid::_draw(Window& win) const noexcept {
    for(const auto& c : m_cells) {
        SDL_Rect r = c;
        SDL_RenderDrawRect(win.m_renderer, &r);
    }
}

} //namespace rmk