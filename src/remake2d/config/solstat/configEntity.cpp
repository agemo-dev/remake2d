#include <remake2d/config/config.hpp>
#include <remake2d/physic.hpp>
#include <remake2d/script.hpp>
#include <remake2d/actor.hpp>

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
            [](PhysicBody& self) { return self.isSolid(); },
            [](PhysicBody& self, bool s) { self.isSolid(s); }
        );
        ut["link"]             = &PhysicBody::link;
        ut["move"]             = &PhysicBody::move;
        ut["rotate"]           = &PhysicBody::rotate;
        ut["scale"]            = &PhysicBody::scale;
        ut["resize"]           = &PhysicBody::resize;
        ut["center"]           = &PhysicBody::center;
        ut["size"]             = &PhysicBody::size;
        ut["linkAnimation"]    = &PhysicBody::linkAnimation;
        ut["animation"]        = &PhysicBody::animation;
        ut["onContactStart"]   = &PhysicBody::onContactStart;
        ut["onContact"]        = &PhysicBody::onContact;
        ut["onContactEnd"]     = &PhysicBody::onContactEnd;
    });

    script._registerEngineType<StaticBody, StaticBody(Geometry&)>("StaticBody", nullptr, type::base<PhysicBody>);

    script._registerEngineType<DynamicBody, DynamicBody(Geometry&)>("DynamicBody", [](SolState::Type& ut) {
        ut["mass"]             = &DynamicBody::mass;
        ut["bounce"]           = &DynamicBody::bounce;
        ut["bounceThreshold"]  = &DynamicBody::bounceThreshold;
        ut["infiniteBounce"]   = &DynamicBody::infiniteBounce;
        ut["friction"]         = &DynamicBody::friction;
        ut["gravity"]          = &DynamicBody::gravity;
        ut["isBullet"]         = &DynamicBody::isBullet;
        ut["warp"]             = &DynamicBody::warp;
        ut["limit"]            = &DynamicBody::limit;
        ut["jump"]             = &DynamicBody::jump;
        ut["velocity"]         = sol::overload(
            [](DynamicBody& self) { return self.velocity(); },
            [](DynamicBody& self, const Vec2d& v) { self.velocity(v); }
        );
        ut["move"]             = &DynamicBody::move;
        ut["onMove"]           = &DynamicBody::onMove;
        ut["onMoveUp"]         = &DynamicBody::onMoveUp;
        ut["onMoveDown"]       = &DynamicBody::onMoveDown;
        ut["onMoveLeft"]       = &DynamicBody::onMoveLeft;
        ut["onMoveRight"]      = &DynamicBody::onMoveRight;
    }, type::base<PhysicBody>);

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

    script._registerEngineType<StaticActor, StaticActor(const Geometry&)>("StaticActor", [](SolState::Type& ut) {
		ut["body"] = &StaticActor::body;
	}, type::base<ActorBase>);

    script._registerEngineType<DynamicActor, DynamicActor(const Geometry&)>("DynamicActor", [](SolState::Type& ut) {
		ut["body"] = &DynamicActor::body;
	}, type::base<ActorBase>);
}

} // namespace solstat
} // namespace config
} // namespace rmk