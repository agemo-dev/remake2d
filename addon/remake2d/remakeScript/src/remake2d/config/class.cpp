#include <remake2d/all/everything.hpp>

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
        ut["area"]           = &Window::area;
        ut["move"]           = &Window::move;
        ut["resize"]         = &Window::resize;
        ut["rename"]         = &Window::rename;
        ut["close"]          = &Window::close;
        ut["isOpen"]         = &Window::isOpen;
        ut["isFocus"]        = &Window::isFocus;
        ut["title"]          = &Window::title;
        ut["clear"]          = &Window::clear;
        ut["present"]        = &Window::present;
        ut["resizable"]      = &Window::resizable;
        ut["screenshot"]     = &Window::screenshot;
        ut["connectViewport"]    = &Window::connectViewport;
        ut["disconnectViewport"] = &Window::disconnectViewport;
        ut["draw"] = sol::overload(
            [](Window& win, const Drawable& obj)         { win.draw(obj, 0); },
            [](Window& win, const Drawable& obj, i16 l)  { win.draw(obj, l); },
        );
        ut["fill"] = sol::overload(
            [](Window& win, const Fillable& obj)         { win.fill(obj, 0); },
            [](Window& win, const Fillable& obj, i16 l)  { win.fill(obj, l); },
        );
    }, type::base<Trackable<Window>>);

	rmk::script._registerEngineType<Window::Viewport, Window::Viewport(), Window::Viewport(const Area&), Window::Viewport(const Area&, const Camera&)>("Window::Viewport", [](SolState::Type& ut) {
        ut["area"]   = Window::Viewport::area;
        ut["camera"] = [](Camera& self) { return self.camera(); };
    }, rmk::type::base<Trackable<Window::Viewport>);

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
        ut["center"]        = &Camera::center;
        ut["size"]          = &Camera::size;
        ut["offset"]        = &Camera::offset;
        ut["follow"]        = &Camera::follow;
        ut["unfollow"]      = &Camera::unfollow;
        ut["viewCenter"]    = &Camera::viewCenter;
        ut["followedPoint"] = &Camera::followedPoint;
    }, type::base<Trackable<Camera>>,
	    "onMove" , &Camera::onMove
	);

    script._registerEngineType<Geometry>("Geometry", [](SolState::Type& ut) {
        ut["move"]           = &Geometry::move;
        ut["rotate"]         = &Geometry::rotate;
        ut["resize"]         = &Geometry::resize;
        ut["scale"]          = &Geometry::scale;
        ut["transform"]      = &Geometry::transform;
        ut["center"]         = &Geometry::center;
        ut["size"]           = &Geometry::size;
        ut["points"]         = &Geometry::points;
		ut["hasIntersected"] = &Geometry::hasIntersected;
    }, type::base<Followable, Drawable, Fillable>);

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
			[](const TextureBase& self, const Geometry& g)    -> bool { return self.hasIntersected(g); },
			[](const TextureBase& self, const TextureBase& o) -> bool { return self.hasIntersected(o); }
		);
    }, type::base<Drawable, Fillable>);

    script._registerEngineType<Sprite, Sprite(std::string_view, Rectangle)>("Sprite", nullptr, type::base<TextureBase>);

    script._registerEngineType<Animation, Animation(std::string_view, Rectangle, u8, Dim2d, Vec2d, u8)>("Animation", [](SolState::Type& ut) {
        ut["play"]      = sol::overload(
			[](Animation& a)                  { a.play(); },
			[](Animation& a, i8 loop)         { a.play(loop); },
			[](Animation& a, i8 loop, u8 fps) { a.play(loop, fps); }
		);
        ut["pause"]     = &Animation::pause;
        ut["resume"]    = &Animation::resume;
        ut["stop"]      = &Animation::stop;
    }, type::base<Sprite, Trackable<Animation>>,
		"onFinish" , &Animation::onFinish,
        "onRepeat" , &Animation::onRepeat
	);

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

    script._registerEngineType<TileMapData, TileMapData(), TileMapData(Vec2d, Dim2d, Grid2d, Dim2d), TileMapData(Vec2d, Dim2d, Grid2d, Dim2d, Vec2d), TileMapData(Vec2d, Dim2d, Grid2d, Dim2d, Vec2d, Vec2d)>("TileMapData", nullptr, type::base<>,
        "center",     &TileMapData::center,
        "size",       &TileMapData::size,
        "cut",        &TileMapData::cut,
        "clip_size",  &TileMapData::clip_size,
        "clip_start", &TileMapData::clip_start,
        "margin",     &TileMapData::margin
    );

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
    }, type::base<Drawable, Fillable>);

    script._registerEngineType<TileGrid, TileGrid(const Vec2d&, const Dim2d&, const Grid2d&)>("TileGrid", [](SolState::Type& ut) {
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
    }, type::base<Drawable, Fillable>);

    script._registerEngineType<Parallax, Parallax(const Vec2d&, const Dim2d&, const std::vector<Sprite>&, const std::vector<u8>&)>("Parallax", [](SolState::Type& ut) {
        ut["move"]     = &Parallax::move;
        ut["resize"]   = &Parallax::resize;
        ut["velocity"] = sol::overload(
            [](Parallax& self) { return self.velocity(); },
            [](Parallax& self, const Vec2d& v) { self.velocity(v); }
        );
        ut["center"]   = &Parallax::center;
        ut["size"]     = &Parallax::size;
        ut["update"]   = &Parallax::update;
    }, type::base<Drawable, Fillable>);

	script._registerEngineType<Data,
		Data(),       Data(byte),
		Data(rune),   Data(imax),
		Data(fmax),   Data(bool),
		Data(Vec2d),  Data(Dim2d),
		Data(Grid2d), Data(Fact2d),
		Data(Fact2d), Data(Color),
		Data(Area),   Data(std::string_view)
	>("Data", [](SolState::Type& ut) {
		ut["get"] = sol::overload(
	        [](const Data& d, std::string_view key) -> const Data& { return d[key]; },
	        [](const Data& d, usize idx)            -> const Data& { return d[idx]; }
	    );
	    ut["map"]  = [](std::map<std::string, Data> m)  { return Data(std::move(m)); };
	    ut["list"] = [](std::vector<Data> v)            { return Data(std::move(v)); };
	});

    script._registerEngineType<DataFile, DataFile(std::string_view)>("DataFile", [](SolState::Type& ut) {
        ut["save"]   = sol::overload(
			[](DataFile& df, const Data& d)    { df.save(d); },
			[](DataFile& df, const Savable& d) { df.save(d); }
		);
        ut["load"]   = sol::overload(
			[](DataFile& df, Data *d)    { df.load(*d); },
			[](DataFile& df, Savable *d) { df.load(*d); }
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
    }, type::base<>,
        "onFileChanged" , &Script::onFileChanged
	);

    script._registerEngineType<Scene, Scene()>("Scene", [](SolState::Type& ut) {
        ut["update"]          = &Scene::update;
        ut["execute"]         = &Scene::execute;
        ut["add"]             = sol::overload(
            [](Scene& s, Actor& a, int l) { s.add(a, (i16)l); },
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
		ut["link"]   = [](Act& s, std::string_view tag, std::vector<std::string_view> tags) {
		    s.link(tag, tags);
		};
        ut["focus"]  = &Act::focus;
        ut["update"] = sol::overload(
            [](Act& a) { a.update(); },
            [](Act& a, std::string_view tag) { a.update(tag); }
        );
        ut["updates"]   = &Act::updates;
        ut["scene"]     = &Act::scene;
    });

    script._registerEngineType<Sound>("Sound", [](SolState::Type& ut) {
        ut["play"]     = sol::overload(
			[](Sound& self)         { self.play(); },
			[](Sound& self, i8 l)   { self.play(l); }
		);
        ut["stop"]     = &Sound::stop;
        ut["pause"]    = &Sound::pause;
        ut["resume"]   = &Sound::resume;
        ut["volume"]   = &Sound::volume;
    }, type::base<>,
		"onFinish" , &SFX::onFinish,
		"onRepeat" , &SFX::onRepeat
	);

    script._registerEngineType<Music, Music(std::string_view, u8)>("Music", nullptr, type::base<Sound>);

    script._registerEngineType<SFX, SFX(std::string_view, u8)>("SFX", [](SolState::Type& ut) {
        ut["stopAll"]    = &SFX::stopAll;
    }, type::base<Sound>);

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
        ut["repeat"]      = &Timer::repeat;
        ut["stop"]        = &Timer::stop;
        ut["limit"]       = sol::overload(
			[](Timer& t) { return t.limit(); },
			[](Timer& t, fmax l) { t.limit(l); }
		);
        ut["isActive"]    = &Timer::isActive;
        ut["isElapsed"]   = &Timer::isElapsed;
        ut["elapsedTime"] = &Timer::elapsedTime;
    }, rmk::type::base<>,
		"onTimeout" , &Timer::onTimeout
	);
}

} // namespace solstat
} // namespace config
} // namespace rmk