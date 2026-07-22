#include <remake2d/config/config.hpp>
#include <remake2d/window.hpp>
#include <remake2d/camera.hpp>
#include <remake2d/shape.hpp>
#include <remake2d/texture.hpp>
#include <remake2d/tilemap.hpp>
#include <remake2d/tilegrid.hpp>
#include <remake2d/parallax.hpp>
#include <remake2d/utility.hpp>
#include <remake2d/stream.hpp>
#include <remake2d/actor.hpp>
#include <remake2d/scene.hpp>
#include <remake2d/sound.hpp>
#include <remake2d/script.hpp>
#include <remake2d/data.hpp>
#include <remake2d/time.hpp>
#include <remake2d/loop.hpp>

namespace rmk {
namespace config {
namespace solstat {

void initLuaClass(void) noexcept {

    script._registerEngineType<Window,
        Window(),
        Window(std::string_view),
        Window(std::string_view, Vec2d),
        Window(std::string_view, Vec2d, Dim2d)
    >("Window", [](SolState::Type& ut) {
        ut["ID"]             = &Window::ID;
        ut["pos"]            = &Window::pos;
        ut["size"]           = &Window::size;
        ut["center"]         = &Window::center;
        ut["move"]           = &Window::move;
        ut["resize"]         = &Window::resize;
        ut["rename"]         = &Window::rename;
        ut["close"]          = &Window::close;
        ut["isOpen"]         = &Window::isOpen;
        ut["isFocus"]        = &Window::isFocus;
        ut["clear"]          = &Window::clear;
        ut["present"]        = &Window::present;
        ut["addViewport"]    = &Window::addViewport;
        ut["useViewport"]    = &Window::useViewport;
        ut["resetViewport"]  = &Window::resetViewport;
        ut["removeViewport"] = &Window::removeViewport;
        ut["linkCamera"]     = &Window::linkCamera;
        ut["unlinkCamera"]   = &Window::unlinkCamera;
        ut["draw"] = sol::overload(
            [](Window& win, TextureBase& tex, Color c)  { win.draw(tex, c); },
            [](Window& win, TileMap     & tile, Color c) { win.draw(tile, c); },
            [](Window& win, Parallax    & para, Color c) { win.draw(para, c); },
            [](Window& win, Area        & a, Color c)   { win.draw(a, c); },
            [](Window& win, Geometry    & g, Color c)   { win.draw(g, c); },
            [](Window& win, PhysicBody  & b, Color c)   { win.draw(b, c); },
            [](Window& win, TileGrid    & g, Color c)   { win.draw(g, c); },
            [](Window& win, TextureBase& tex, Color c, std::string_view v)  { win.draw(tex, c, v); },
            [](Window& win, TileMap     & tile, Color c, std::string_view v) { win.draw(tile, c, v); },
            [](Window& win, Parallax    & para, Color c, std::string_view v) { win.draw(para, c, v); },
            [](Window& win, Area        & a, Color c, std::string_view v)   { win.draw(a, c, v); },
            [](Window& win, Geometry    & g, Color c, std::string_view v)   { win.draw(g, c, v); },
            [](Window& win, PhysicBody  & b, Color c, std::string_view v)   { win.draw(b, c, v); },
            [](Window& win, TileGrid    & g, Color c, std::string_view v)   { win.draw(g, c, v); }
        );
        ut["fill"] = sol::overload(
            [](Window& win, Area       & a, Color c)   { win.fill(a, c); },
            [](Window& win, Geometry   & g, Color c)   { win.fill(g, c); },
            [](Window& win, PhysicBody & b, Color c)   { win.fill(b, c); },
            [](Window& win, Area       & a, Color c, std::string_view v)   { win.fill(a, c, v); },
            [](Window& win, Geometry   & g, Color c, std::string_view v)   { win.fill(g, c, v); },
            [](Window& win, PhysicBody & b, Color c, std::string_view v)   { win.fill(b, c, v); }
        );
    });

    script._registerEngineType<Camera,
        Camera(),
        Camera(const Vec2d&, const Dim2d&, const Dim2d&)
    >("Camera", [](SolState::Type& ut) {
        ut["move"]      = &Camera::move;
        ut["zoom"]      = sol::overload(
            [](Camera& self) { return self.zoom(); },
            [](Camera& self, f32 z) { self.zoom(z); }
        );
        ut["resize"]    = &Camera::resize;
        ut["limit"]     = sol::overload(
            [](Camera& self) { return self.limit(); },
            [](Camera& self, const Dim2d& l) { self.limit(l); }
        );
        ut["smoothing"] = sol::overload(
            [](Camera& self) { return self.smoothing(); },
            [](Camera& self, f32 s) { self.smoothing(s); }
        );
        ut["center"]    = &Camera::center;
        ut["size"]      = &Camera::size;
        ut["offset"]    = &Camera::offset;
        ut["follow"]    = sol::overload(
            [](Camera& cam, Vec2d& p) { cam.follow(p); },
            [](Camera& cam, Geometry& g) { cam.follow(g); },
            [](Camera& cam, PhysicBody& b) { cam.follow(b); }
        );
        ut["onMove"]    = &Camera::onMove;
    });
	
    script._registerEngineType<Geometry>("Geometry", [](SolState::Type& ut) {
        ut["move"]        = &Geometry::move;
        ut["rotate"]      = &Geometry::rotate;
        ut["resize"]      = &Geometry::resize;
        ut["scale"]       = &Geometry::scale;
        ut["transform"]   = &Geometry::transform;
        ut["center"]      = &Geometry::center;
        ut["size"]        = &Geometry::size;
        ut["points"]      = &Geometry::points;
		ut["hasIntersected"] = &Geometry::hasIntersected;
    });

    script._registerEngineType<Point, Point(), Point(const Vec2d&)>("Point", nullptr, type::base<Geometry>);

    script._registerEngineType<Line, Line(), Line(const Vec2d&, const Dim2d&)>("Line", nullptr, type::base<Geometry>);

    script._registerEngineType<Triangle, Triangle(), Triangle(const Vec2d&, const Dim2d&)>("Triangle", nullptr, type::base<Geometry>);

    script._registerEngineType<Rectangle, Rectangle(), Rectangle(const Vec2d&, const Dim2d&)>("Rectangle", nullptr, type::base<Geometry>);

    script._registerEngineType<Square, Square(), Square(const Vec2d&, f32)>("Square", nullptr, type::base<Geometry>);

    script._registerEngineType<Circle, Circle(), Circle(const Vec2d&, f32)>("Circle", nullptr, type::base<Geometry>);

    script._registerEngineType<Ellipse, Ellipse(), Ellipse(const Vec2d&, const Dim2d&)>("Ellipse", nullptr, type::base<Geometry>);

    script._registerEngineType<Losange, Losange(), Losange(const Vec2d&, const Dim2d&)>("Losange", nullptr, type::base<Geometry>);
	
    script._registerEngineType<Hexagone, Hexagone(), Hexagone(const Vec2d&, const Dim2d&)>("Hexagone", nullptr, type::base<Geometry>);

    script._registerEngineType<TextureBase>("TextureBase", [](SolState::Type& ut) {
        ut["move"]     = &TextureBase::move;
        ut["rotate"]   = &TextureBase::rotate;
        ut["resize"]   = &TextureBase::resize;
        ut["scale"]    = &TextureBase::scale;
        ut["clip"]     = &TextureBase::clip;
        ut["unclip"]   = &TextureBase::unclip;
        ut["center"]   = &TextureBase::center;
        ut["size"]     = &TextureBase::size;
        ut["realSize"] = &TextureBase::realSize;
		ut["hasIntersected"] = sol::overload(
			[](const TextureBase& self, const Geometry& g) { self.hasIntersected(g); },
			[](const TextureBase& self, const TextureBase& o) { self.hasIntersected(o); }
		);
    });

    script._registerEngineType<Sprite, Sprite(std::string_view, Rectangle)>("Sprite", nullptr, type::base<TextureBase>);

    script._registerEngineType<Animation,
        Animation(std::string_view, Rectangle, u8, Dim2d, Vec2d, u8)
    >("Animation", [](SolState::Type& ut) {
        ut["play"]   = [](Animation& a, int loop, int fps) { a.play((i8)loop, (u8)fps); };
        ut["pause"]  = &Animation::pause;
        ut["resume"] = &Animation::resume;
        ut["stop"]   = &Animation::stop;
    }, type::base<Sprite>);

    script._registerEngineType<Text, Text(std::string_view, Vec2d)>("Text", [](SolState::Type& ut) {
        ut["write"]	       = &Text::write;
        ut["append"]	   = sol::overload(
								[](Text& self, fmt f)   		   { self.append(f); },
								[](Text& self, std::string_view s) { self.append(s); }
		);
        ut["clear"]   	   = &Text::clear;
        ut["anchorX"]  	   = &Text::anchorX;
        ut["anchorY"]  	   = &Text::anchorY;
        ut["text"]     	   = &Text::text;
        ut["maxLengh"]     = sol::overload(
								[](Text& self)        { return self.maxLengh(); },
								[](Text& self, u16 l) { self.maxLengh(l); }
		);
    }, type::base<TextureBase>);

    script._registerEngineType<TileMapData, TileMapData()>("TileMapData", [](SolState::Type& ut) {
        ut["center"]     = &TileMapData::center;
        ut["size"]       = &TileMapData::size;
        ut["clip_start"] = &TileMapData::clip_start;
        ut["clip_size"]  = &TileMapData::clip_size;
        ut["cut"]        = &TileMapData::cut;
        ut["margin"]     = &TileMapData::margin;
    });

    script._registerEngineType<TileMap, TileMap(std::string_view, TileMapData)>("TileMap", [](SolState::Type& ut) {
        ut["load"]          = &TileMap::load;
        ut["build"]         = &TileMap::build;
        ut["move"]          = &TileMap::move;
        ut["resize"]        = &TileMap::resize;
        ut["counterStart"]  = &TileMap::counterStart;
        ut["tag"]           = &TileMap::tag;
        ut["applyPhysic"]   = sol::overload(
            [](TileMap& tm, i16 id)               { tm.applyPhysic(id); },
            [](TileMap& tm, std::string_view tag) { tm.applyPhysic(tag); }
        );
        ut["body"]          = sol::overload(
            [](TileMap& tm, i16 id)               -> PhysicBody& { return tm.body(id); },
            [](TileMap& tm, std::string_view tag) -> PhysicBody& { return tm.body(tag); }
        );
        ut["tileCount"]     = sol::overload(
            [](TileMap& tm) { return tm.tileCount(); },
            [](TileMap& tm, TileMap::TileID id) { return tm.tileCount(id); }
        );
        ut["clip"]          = &TileMap::clip;
        ut["center"]        = &TileMap::center;
        ut["size"]          = &TileMap::size;
        ut["cut"]           = &TileMap::cut;
    });

    script._registerEngineType<TileGrid,
        TileGrid(const Vec2d&, const Dim2d&, const Grid2d&)
    >("TileGrid", [](SolState::Type& ut) {
        ut["move"]   = &TileGrid::move;
        ut["cut"]    = sol::overload(
            [](TileGrid& self) { return self.cut(); },
            [](TileGrid& self, const Grid2d& c) { self.cut(c); }
        );
        ut["resize"] = &TileGrid::resize;
        ut["count"]  = &TileGrid::count;
        ut["size"]   = &TileGrid::size;
        ut["center"] = &TileGrid::center;
        ut["cell"]   = &TileGrid::cell;
        ut["cells"]  = &TileGrid::cells;
    });

    script._registerEngineType<Parallax,
        Parallax(const Vec2d&, const Dim2d&, std::span<Sprite>, std::span<u8>)
    >("Parallax", [](SolState::Type& ut) {
        ut["move"]     = &Parallax::move;
        ut["resize"]   = &Parallax::resize;
        ut["velocity"] = sol::overload(
            [](Parallax& self) { return self.velocity(); },
            [](Parallax& self, const Vec2d& v) { self.velocity(v); }
        );
        ut["center"]   = &Parallax::center;
        ut["size"]     = &Parallax::size;
    });

    script._registerEngineType<DataFile, DataFile(std::string_view)>("DataFile", [](SolState::Type& ut) {
        ut["save"]   = sol::overload(
			[](DataFile& df, const Data& d)     { df.save(d); },
			[](DataFile& df, const ISavable& d) { df.save(d); }
		);
        ut["load"]   = sol::overload(
			[](DataFile& df, Data *d)     { df.load(*d); },
			[](DataFile& df, ISavable *d) { df.load(*d); }
		);
        ut["path"]   = &DataFile::path;
        ut["name"]   = &DataFile::name;
        ut["exist"]  = &DataFile::exist;
        ut["remove"] = &DataFile::remove;
    });

    script._registerEngineType<Script, Script(std::string_view)>("Script", [](SolState::Type& ut) {
        ut["update"]   = &Script::update;
        ut["get"]       = [](Script& s, std::string_view id, sol::this_state ts) -> sol::object {
            sol::state_view lua(ts);
            try {
                return sol::make_object(lua, s.get<sol::object>(id));
            } catch(...) {
                return sol::lua_nil;
            }
        };
        ut["onFileChanged"] = &Script::onFileChanged;
    });

    script._registerEngineType<Scene, Scene()>("Scene", [](SolState::Type& ut) {
        ut["update"]          = &Scene::update;
        ut["execute"]         = &Scene::execute;
        ut["add"]             = sol::overload(
            [](Scene& s, ActorBase& a, int l) { s.add(a, (i16)l); },
            [](Scene& s, const Scene::Frame& f, int l) { s.add(f, (i16)l); }
        );
        ut["remove"]          = &Scene::remove;
        ut["enable"]          = &Scene::enable;
        ut["disable"]         = &Scene::disable;
        ut["isEnabled"]       = &Scene::isEnabled;
        ut["setLayerActive"]  = &Scene::setLayerActive;
        ut["setActorActive"]  = &Scene::setActorActive;
    });

    script._registerEngineType<Act, Act()>("Act", [](SolState::Type& ut) {
        ut["add"]    = &Act::add;
        ut["link"]   = [](Act& a, std::string_view tag, sol::variadic_args va) {
            std::vector<std::string_view> v;
            for(const auto& e : va) v.push_back(e.as<std::string_view>());
            a.link(tag, v);
        };
        ut["focus"]  = &Act::focus;
        ut["update"] = sol::overload(
            [](Act& a) { a.update(); },
            [](Act& a, std::string_view tag) { a.update(tag); }
        );
        ut["updates"]   = &Act::updates;
        ut["scene"]     = &Act::scene;
    });

    script._registerEngineType<Music, Music(std::string_view, u8)>("Music", [](SolState::Type& ut) {
        ut["play"]     = &Music::play;
        ut["stop"]     = &Music::stop;
        ut["pause"]    = &Music::pause;
        ut["resume"]   = &Music::resume;
        ut["volume"]   = &Music::volume;
        ut["onFinish"] = &Music::onFinish;
    });

    script._registerEngineType<SFX, SFX(std::string_view, u8)>("SFX", [](SolState::Type& ut) {
        ut["play"]       = &SFX::play;
        ut["stop"]       = &SFX::stop;
        ut["stopAll"]    = &SFX::stopAll;
        ut["pause"]      = &SFX::pause;
        ut["resume"]     = &SFX::resume;
        ut["volume"]     = &SFX::volume;
        ut["onFinish"]   = &SFX::onFinish;
    });

    script._registerEngineType<Date, Date(), Date(u8, u8, i32)>("Date", [](SolState::Type& ut) {
        ut["day"]    = sol::overload(
            [](Date& d) { return d.day(); },
            [](Date& d, u8 v) { d.day(v); }
        );
        ut["month"]  = sol::overload(
            [](Date& d) { return d.month(); },
            [](Date& d, u8 v) { d.month(v); }
        );
        ut["year"]   = sol::overload(
            [](Date& d) { return d.year(); },
            [](Date& d, i32 v) { d.year(v); }
        );
        ut["format"] = &Date::format;
        ut["date"]   = sol::overload(
            [](Date& d) { return d.date(); },
            [](Date& d, std::string_view s) { d.date(s); }
        );
    });

    script._registerEngineType<Clock, Clock(), Clock(u8, u8, u8)>("Clock", [](SolState::Type& ut) {
        ut["hour"]   = sol::overload(
            [](Clock& c) { return c.hour(); },
            [](Clock& c, const u8& v) { c.hour(v); }
        );
        ut["minute"] = sol::overload(
            [](Clock& c) { return c.minute(); },
            [](Clock& c, const u8& v) { c.minute(v); }
        );
        ut["second"] = sol::overload(
            [](Clock& c) { return c.second(); },
            [](Clock& c, const u8& v) { c.second(v); }
        );
        ut["format"] = &Clock::format;
        ut["time"]   = sol::overload(
            [](Clock& c) { return c.time(); },
            [](Clock& c, std::string_view s) { c.time(s); }
        );
    });

    script._registerEngineType<Chronometer, Chronometer()>("Chronometer", [](SolState::Type& ut) {
        ut["start"]       = &Chronometer::start;
        ut["pause"]       = &Chronometer::pause;
        ut["resume"]      = &Chronometer::resume;
        ut["stop"]        = &Chronometer::stop;
        ut["reset"]       = &Chronometer::reset;
        ut["elapsedTime"] = [](Chronometer& c) { return c.elapsedTime(); };
    });
	
    script._registerEngineType<Timer, Timer(), Timer(fmax)>("Timer", [](SolState::Type& ut) {
        ut["start"]       = &Timer::start;
        ut["pause"]       = &Timer::pause;
        ut["resume"]      = &Timer::resume;
        ut["stop"]        = &Timer::stop;
        ut["limit"]       = sol::overload(
			[](Timer& t) { return t.limit(); },
			[](Timer& t, fmax l) { t.limit(l); }
		);
        ut["isActive"]    = &Timer::isActive;
        ut["isElapsed"]   = &Timer::isElapsed;
        ut["elapsedTime"] = &Timer::elapsedTime;
    });
}

} // namespace solstat
} // namespace config
} // namespace rmk