#include <remake2d/config/config.hpp>
#include <remake2d/all/types.hpp>
#include <remake2d/stream.hpp>
#include <remake2d/script.hpp>
#include <remake2d/math.hpp>

namespace rmk {
namespace config {
namespace solstat {

void initLuaType(void) noexcept {

    script._registerEngineType<
        Vec2d,
        Vec2d(),
        Vec2d(f32),
        Vec2d(f32, f32)
    >("Vec2d", [](SolState::Type& ut) {
		ut["x"]	= &Vec2d::x;
		ut["y"]	= &Vec2d::y;
	});

    script._registerEngineType<
        Dim2d,
        Dim2d(),
        Dim2d(f32),
        Dim2d(f32, f32)
    >("Dim2d", [](SolState::Type& ut) {
		ut["w"]	= &Dim2d::w;
		ut["h"]	= &Dim2d::h;
	});

	script._registerEngineType<
	    Fact2d,
	    Fact2d(),
	    Fact2d(f32),
	    Fact2d(f32, f32)
	>("Fact2d", [](SolState::Type& ut) {
		ut["x"]	= &Fact2d::x;
		ut["y"]	= &Fact2d::y;
	});
	
	script._registerEngineType<
	    Grid2d,
	    Grid2d(),
	    Grid2d(usize),
	    Grid2d(usize, usize)
	>("Grid2d", [](SolState::Type& ut) {
		ut["x"]	= &Grid2d::x;
		ut["y"]	= &Grid2d::y;
	});

    script._registerEngineType<
        Area,
        Area(),
        Area(i32, i32, i32, i32),
        Area(const Vec2d&, const Dim2d&)
    >("Area", [](SolState::Type& ut) {
		ut["x"]		 = &Area::x;
		ut["y"]		 = &Area::y;
		ut["w"]		 = &Area::w;
		ut["h"]		 = &Area::h;
        ut["center"] = &Area::center;
    });

    script._registerEngineType<
        Triangulation,
        Triangulation(),
        Triangulation(const Vec2d&, const Vec2d&, const Vec2d&)
    >("Triangulation", [](SolState::Type& ut) {
		ut["a"]	= &Triangulation::a;
		ut["b"]	= &Triangulation::b;
		ut["c"]	= &Triangulation::c;
	});

    script._registerEngineType<
        Color,
        Color(),
        Color(byte, byte, byte, byte)
    >("Color", [](SolState::Type& ut) {
		ut["r"]	= &Color::r;
		ut["g"]	= &Color::g;
		ut["b"]	= &Color::b;
		ut["a"]	= &Color::a;
	});

    script._registerEngineType<
        HSL,
        HSL(),
        HSL(f32, f32, f32)
    >("HSL", [](SolState::Type& ut) {
		ut["h"]	= &HSL::h;
		ut["s"]	= &HSL::s;
		ut["l"]	= &HSL::l;
	});
}

} // namespace solstat
} // namespace config
} // namespace rmk