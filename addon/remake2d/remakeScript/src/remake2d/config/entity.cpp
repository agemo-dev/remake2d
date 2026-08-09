#include <remake2d/all/everything.hpp>

namespace rmk {
namespace config {
namespace solstat {

void initLuaEntity(void) noexcept {

    script._registerEngineType<PhysicBody>("PhysicBody", [](SolState::Type& ut) {
        ut["tag"] = sol::overload(
            [](PhysicBody& self) -> std::string { return self.tag(); },
            [](PhysicBody& self, std::string_view t) { self.tag(t); }
        );
        ut["ID"]               = &PhysicBody::ID;
        ut["isSolid"]          = sol::overload(
            [](PhysicBody& self) { return self.isSolid();   },
            [](PhysicBody& self, bool s) { self.isSolid(s); }
        );
        ut["move"]             = &PhysicBody::move;
        ut["rotate"]           = &PhysicBody::rotate;
        ut["scale"]            = &PhysicBody::scale;
        ut["resize"]           = &PhysicBody::resize;
        ut["center"]           = &PhysicBody::center;
        ut["size"]             = &PhysicBody::size;
        ut["linkAnimation"]    = &PhysicBody::linkAnimation;
        ut["focusAnimation"]   = &PhysicBody::focusAnimation;
        ut["animation"]        = sol::overload(
			[](PhysicBody& self)                     { return self.animation();  },
			[](PhysicBody& self, std::string_view t) { return self.animation(t); }
		);
    }, type::base<>,
		"onContact"       , &PhysicBody::onContact,
		"onContactEnd"    , &PhysicBody::onContactEnd,
		"onContactStart"  , &PhysicBody::onContactStart
	);

    script._registerEngineType<StaticBody, StaticBody(Geometry&)>("StaticBody", nullptr, type::base<PhysicBody>);

	script._registerEngineType<DynamicBody, DynamicBody(Geometry&)>("DynamicBody", [](SolState::Type& ut) {
	    ut["mass"]             = sol::overload(
	        [](DynamicBody& self) { return self.mass(); },
	        [](DynamicBody& self, f32 m) { self.mass(m); }
	    );
	    ut["density"]          = sol::overload(
	        [](DynamicBody& self) { return self.density(); },
	        [](DynamicBody& self, f32 d) { self.density(d); }
	    );
	    ut["bounce"]           = sol::overload(
	        [](DynamicBody& self) { return self.bounce(); },
	        [](DynamicBody& self, f32 b) { self.bounce(b); }
	    );
	    ut["bounceThreshold"]  = sol::overload(
	        [](DynamicBody& self) { return self.bounceThreshold(); },
	        [](DynamicBody& self, f32 t) { self.bounceThreshold(t); }
	    );
	    ut["infiniteBounce"]   = sol::overload(
	        [](DynamicBody& self) { return self.infiniteBounce(); },
	        [](DynamicBody& self, bool s) { self.infiniteBounce(s); }
	    );
	    ut["friction"]         = sol::overload(
	        [](DynamicBody& self) { return self.friction(); },
	        [](DynamicBody& self, f32 f) { self.friction(f); }
	    );
	    ut["gravity"]          = sol::overload(
	        [](DynamicBody& self) { return self.gravity(); },
	        [](DynamicBody& self, bool g) { self.gravity(g); }
	    );
	    ut["isBullet"]         = sol::overload(
	        [](DynamicBody& self) { return self.isBullet(); },
	        [](DynamicBody& self, bool b) { self.isBullet(b); }
	    );
	    ut["warp"]             = sol::overload(
	        [](DynamicBody& self) { return self.warp(); },
	        [](DynamicBody& self, const Area& a) { self.warp(a); }
	    );
	    ut["limit"]            = sol::overload(
	        [](DynamicBody& self) { return self.limit(); },
	        [](DynamicBody& self, const Area& a) { self.limit(a); }
	    );

	    ut["jump"]              = &DynamicBody::jump;
	    ut["push"]              = &DynamicBody::push;
	    ut["velocity"]          = sol::overload(
	        [](DynamicBody& self) { return self.velocity(); },
	        [](DynamicBody& self, const Vec2d& v) { self.velocity(v); }
	    );
	}, type::base<PhysicBody>,
		"onMove"      , &DynamicBody::onMove,
		"onMoveUp"    , &DynamicBody::onMoveUp,
		"onMoveDown"  , &DynamicBody::onMoveDown,
		"onMoveLeft"  , &DynamicBody::onMoveLeft,
		"onMoveRight" , &DynamicBody::onMoveRight
	);

    script._registerEngineType<StaticActor, StaticActor(const Geometry&)>("StaticActor", [](SolState::Type& ut) {
		ut["body"] = &StaticActor::body;
	}, type::base<ActorBase>);

    script._registerEngineType<DynamicActor, DynamicActor(const Geometry&)>("DynamicActor", [](SolState::Type& ut) {
		ut["body"] = &DynamicActor::body;
	}, type::base<ActorBase>);

	script._registerEngineType<ActorBase>("ActorBase", [](SolState::Type& ut) {
        ut["update"]           = &ActorBase::update;
        ut["addChild"]         = &ActorBase::addChild;
        ut["removeChild"]      = &ActorBase::removeChild;
        ut["parent"]           = &ActorBase::parent;
        ut["children"]         = &ActorBase::children;
        ut["active"]           = sol::overload(
            [](ActorBase& self) { return self.active(); },
            [](ActorBase& self, bool a) { self.active(a); }
        );
    });
}

} // namespace solstat
} // namespace config
} // namespace rmk