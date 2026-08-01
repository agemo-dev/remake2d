#ifndef REMAKE2D_TILEGRID_
#define REMAKE2D_TILEGRID_

#include <remake2d/vector.hpp>
#include <remake2d/config/forward.hpp>

#include <vector>

namespace rmk {

class TileGrid {
private:
    Grid2d              m_cut;
    Dim2d               m_size;
    Vec2d               m_center;
    std::vector<Area>   m_cells;

public:
    TileGrid(const Vec2d&, const Dim2d&, const Grid2d&);
    TileGrid(void)                          = default;
    TileGrid(TileGrid&&)                    = default;
    TileGrid(const TileGrid&)               = default;
    TileGrid& operator=(TileGrid&&)         = default;
    TileGrid& operator=(const TileGrid&)    = default;
    
public:
    void move(const Vec2d&)   noexcept;
    void cut(const Grid2d&)   noexcept;
    void resize(const Dim2d&) noexcept;

public:
    usize  count(void) 				const noexcept;
    Dim2d  size(void) 				const noexcept;
    Grid2d cut(void) 				const noexcept;
    Vec2d  center(void)				const noexcept;
    Area   cell(const Grid2d&)		const noexcept; 
    std::vector<Area> cells(void)	const noexcept;
    
private:
    void _build(void) 		  noexcept;
    void _draw(Window&) const noexcept;
    
private:
    friend class Window;
};

} //namespace rmk
#endif