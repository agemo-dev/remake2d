#include <remake2d/tilegrid.hpp>
#include <remake2d/window.hpp>

#include <SDL2/SDL.h>

namespace rmk {

TileGrid::TileGrid(const Vec2d& center, const Dim2d& size, const Grid2d& cut)
        : m_cut(cut), m_size(size), m_center(center) {
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

    __dirty__ = true;
}

void TileGrid::draw(const Drawable& main) const noexcept {
    if (!__is_dirty__) return;
    std::vector<SDL_FPoint> contour;
    contour.reserve(m_cells.size() * 6);

    for (const auto& cell : m_cells) {
        SDL_FPoint tl{ (f32)cell.x,             (f32)cell.y };
        SDL_FPoint tr{ (f32)(cell.x + cell.w),  (f32)cell.y };
        SDL_FPoint br{ (f32)(cell.x + cell.w),  (f32)(cell.y + cell.h) };
        SDL_FPoint bl{ (f32)cell.x,             (f32)(cell.y + cell.h) };

        contour.push_back(tl);
        contour.push_back(tr);
        contour.push_back(br);
        contour.push_back(bl);
        contour.push_back(tl);
        contour.push_back(contour::breaker());
    }

    main.__draw_cache__ = { DrawPack{ m_color, std::move(contour) } };
    __is_fill_dirty__ = false;
}

} //namespace rmk