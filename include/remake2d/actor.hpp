#ifndef REMAKE2D_ACTOR_
#define REMAKE2D_ACTOR_

#include <remake2d/body.hpp>
#include <remake2d/concept.hpp>
#include <remake2d/config/forward.hpp>

#include <vector>
#include <memory>
#include <algorithm>

namespace rmk {

class ActorBase : Trackable<ActorBase> {

protected:
    std::vector<Tracker<ActorBase>> m_children;
    Tracker<ActorBase>              m_parent;
    bool                            m_active{true};

public:
    ActorBase(void)                         = default;
    ActorBase(ActorBase&&)                  = default;
    ActorBase(const ActorBase&)             = default;
    ActorBase& operator=(ActorBase&&)       = default;
    ActorBase& operator=(const ActorBase&)  = default;

public:
    void addChild(ActorBase&)           noexcept;
    void removeChild(ActorBase&)        noexcept;

    Tracker<ActorBase>& parent(void)             noexcept;
    const Tracker<ActorBase>& parent(void) const noexcept;

    std::vector<Tracker<ActorBase>>& children(void) noexcept;
    const std::vector<Tracker<ActorBase>>& children(void) const noexcept;

public:
    virtual void update(void) = 0;
    void active(bool)       noexcept;
    bool active(void) const noexcept;

private:
    void _updates(void);

public:
    virtual ~ActorBase(void);

private:
    friend class Scene;
};

class Actor : public ActorBase {

public:
    Actor(void)                     = default;
    Actor(Actor&&)                  = default;
    Actor(const Actor&)             = default;
    Actor& operator=(Actor&&)       = default;
    Actor& operator=(const Actor&)  = default;

public:
	virtual void update(void) override {};

public:
	virtual ~Actor(void) = default;
};

template<IsPhysic P> class PhysicActor : public ActorBase {

public:
    P   body;

public:
    PhysicActor(const Geometry&);
    PhysicActor(void)                     		= default;
    PhysicActor(PhysicActor&&)                  = default;
    PhysicActor(const PhysicActor&)             = default;
    PhysicActor& operator=(PhysicActor&&)       = default;
    PhysicActor& operator=(const PhysicActor&)  = default;

public:
    virtual void update(void) override {};

public:
	virtual ~PhysicActor(void) = default;
};


using StaticActor   = PhysicActor<StaticBody>;
using DynamicActor  = PhysicActor<DynamicBody>;

} // namespace rmk

#include <remake2d/template/actor.tpp>

#endif