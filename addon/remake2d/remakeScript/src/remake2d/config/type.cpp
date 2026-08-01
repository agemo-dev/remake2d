#include <remake2d/all/everything.hpp>

namespace rmk {
namespace config {
namespace solstat {

void initLuaType(void) noexcept {

	script._registerEngineType<
	    Vec2d,
	    Vec2d(),
	    Vec2d(f32),
	    Vec2d(f32, f32)
	>("Vec2d", nullptr, type::base<>,
	    "x", &Vec2d::x,
	    "y", &Vec2d::y
	);

	script._registerEngineType<
	    Dim2d,
	    Dim2d(),
	    Dim2d(f32),
	    Dim2d(f32, f32)
	>("Dim2d", nullptr, type::base<>,
	    "w", &Dim2d::w,
	    "h", &Dim2d::h
	);

	script._registerEngineType<
	    Fact2d,
	    Fact2d(),
	    Fact2d(f32),
	    Fact2d(f32, f32)
	>("Fact2d", nullptr, type::base<>,
	    "x", &Fact2d::x,
	    "y", &Fact2d::y
	);

	script._registerEngineType<
	    Grid2d,
	    Grid2d(),
	    Grid2d(usize),
	    Grid2d(usize, usize)
	>("Grid2d", nullptr, type::base<>,
	    "x", &Grid2d::x,
	    "y", &Grid2d::y
	);

	script._registerEngineType<
	    Area,
	    Area(),
	    Area(i32, i32, i32, i32),
	    Area(const Vec2d&, const Dim2d&)
	>("Area", [](SolState::Type& ut) {
	    ut["center"] = &Area::center;
	}, type::base<>,
	    "x", &Area::x,
	    "y", &Area::y,
	    "w", &Area::w,
	    "h", &Area::h
	);

	script._registerEngineType<
	    Triangulation,
	    Triangulation(),
	    Triangulation(const Vec2d&, const Vec2d&, const Vec2d&)
	>("Triangulation", nullptr, type::base<>,
	    "a", &Triangulation::a,
	    "b", &Triangulation::b,
	    "c", &Triangulation::c
	);

	script._registerEngineType<
	    Color,
	    Color(),
	    Color(byte, byte, byte, byte)
	>("Color", nullptr, type::base<>,
	    "r", &Color::r,
	    "g", &Color::g,
	    "b", &Color::b,
	    "a", &Color::a
	);

	script._registerEngineType<
	    HSL,
	    HSL(),
	    HSL(f32, f32, f32)
	>("HSL", nullptr, type::base<>,
	    "h", &HSL::h,
	    "s", &HSL::s,
	    "l", &HSL::l
	);

}

} // namespace solstat
} // namespace config
} // namespace rmk