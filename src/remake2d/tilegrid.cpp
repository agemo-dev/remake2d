#include <remake2d/tilegrid.hpp>
#include <remake2d/window.hpp>

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

std::vector<Area>& TileGrid::cells(void) noexcept {
    return m_cells;
}

const std::vector<Area>& TileGrid::cells(void) const noexcept {
    return m_cells;
}

void TileGrid::_build(void) noexcept {
    const usize count = m_cut.x * m_cut.y;
    const Dim2d csize = { m_size.w / m_cut.x, m_size.h / m_cut.y };
    const Vec2d start = { m_center.x - m_size.w / 2, m_center.y - m_size.h / 2 };

    if (m_cells.size() != count) {
        m_cells.clear();
        m_cells.resize(count);
    }

    for (usize i = 0; i < count; i++) {
        Area& cell = m_cells[i];
        const i32 cx = i % m_cut.x;
        const i32 cy = i / m_cut.x;

        cell.x = i32(start.x + cx * csize.w);
        cell.y = i32(start.y + cy * csize.h);
        cell.w = i32(csize.w);
        cell.h = i32(csize.h);

        cell.is_draw_dirty = true;
        cell.is_fill_dirty = true;
    }

    is_draw_dirty = true;
    is_fill_dirty = true;
}

void TileGrid::draw(const Printable& main) const noexcept {

    from(main);

    for (const auto& cell : m_cells) cell.draw(main);

    is_draw_dirty = false;
    drawn         = true;
}

void TileGrid::fill(const Printable& main) const noexcept {

    from(main);

    for (const auto& cell : m_cells) cell.fill(main);

    is_fill_dirty = false;
    filled        = true;
}

} //namespace rmk