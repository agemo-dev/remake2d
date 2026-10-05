#include <remake2d/all/everything.hpp>
#include <remake2d/config/otracker.hpp>

namespace rmk {
namespace config {
namespace solstat {

void initLuaTrait(void) noexcept {

    script._registerEngineType<Trackable>      ("Trackable", [](SolState::Type& ut) {
        ut["tracker"] = &Trackable::tracker;
    });

    script._registerEngineType<Followable>     ("Followable", [](SolState::Type& ut) {
        ut["center"] = &Followable::center;
    }, rmk::type::base<Trackable>);

    script._registerEngineType<Updatable>      ("Updatable", [](SolState::Type& ut) {
        ut["update"] = &Updatable::update;
    }, rmk::type::base<Trackable>);

    script._registerEngineType<Savable>        ("Savable", [](SolState::Type& ut) {
        ut["sdata"] = &Savable::sdata;
        ut["ldata"] = &Savable::ldata;
    });

    script._registerEngineType<Printable>      ("Printable", [](SolState::Type& ut) {
        ut["color"] = sol::overload(
            [](Printable& self)          { return self.color(); },
            [](Printable& self, Color c) { self.color(c); }
        );
        ut["layer"] = sol::overload(
            [](Printable& self)          { return self.layer(); },
            [](Printable& self, i16 l)   { self.layer(l); }
        );
    }, rmk::type::base<>,

        "filled", &Printable::filled,
        "drawn" , &Printable::drawn,

        "overridden", &Printable::overridden,

        "is_draw_dirty" , &Printable::is_draw_dirty,
        "is_fill_dirty" , &Printable::is_fill_dirty
    );

    script._registerEngineType<Actor>("Actor", [](SolState::Type& ut) {
        ut["update"]        = &Actor::update;
        ut["addChild"]      = &Actor::addChild;
        ut["removeChild"]   = &Actor::removeChild;
        ut["parent"]        = [](Actor& self) -> UnsafeTracker<Actor>               { return self.parent();   };
        ut["children"]      = [](Actor& self) -> std::vector<UnsafeTracker<Actor>>& { return self.children(); };
        ut["active"]        = sol::overload(
            [](Actor& self)         { return self.active(); },
            [](Actor& self, bool a) { self.active(a);       }
        );
    }, rmk::type::base<Trackable>);

    script._registerEngineType<StaticActor, StaticActor(const Geometry&)>("StaticActor", nullptr, type::base<Actor, Trackable>,
        "body", &StaticActor::body
    );

    script._registerEngineType<DynamicActor, DynamicActor(const Geometry&)>("DynamicActor", nullptr, type::base<Actor, Trackable>,
        "body", &DynamicActor::body
    );
}

} // namespace solstat
} // namespace config
} // namespace rmk