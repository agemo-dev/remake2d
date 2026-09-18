#ifndef REMAKE2D_ACTOR_
#define REMAKE2D_ACTOR_

#include <remake2d/concept.hpp>
#include <remake2d/private/body.hpp>
#include <remake2d/config/forward.hpp>

#include <vector>
#include <memory>
#include <algorithm>

namespace rmk {

class Actor : public Trackable<Actor> {

protected:
    std::vector<UnsafeTracker<Actor>> m_children;
    UnsafeTracker<Actor>              m_parent;
    bool                              m_active{true};

public:
    Actor(void)                     = default;
    Actor(Actor&&)                  = default;
    Actor(const Actor&)             = default;
    Actor& operator=(Actor&&)       = default;
    Actor& operator=(const Actor&)  = default;

public:
    void addChild(Actor&)           noexcept;
    void removeChild(Actor&)        noexcept;

    UnsafeTracker<Actor>& parent(void)             noexcept;
    const UnsafeTracker<Actor>& parent(void) const noexcept;

    std::vector<UnsafeTracker<Actor>>& children(void)             noexcept;
    const std::vector<UnsafeTracker<Actor>>& children(void) const noexcept;

public:
    virtual void update(void) = 0;
    void active(bool)       noexcept;
    bool active(void) const noexcept;

private:
    void _updates(void);

public:
    virtual ~Actor(void);

private:
    friend class Scene;
};

template<IsPhysic P> class PhysicActor : public Actor {

public:
    P   body;

public:
    PhysicActor(const Geometry&);

public:
    virtual void update(void) override = 0;

public:
    rmk_heritableBaseClass(PhysicActor);
};


using StaticActor   = PhysicActor<StaticBody>;
using DynamicActor  = PhysicActor<DynamicBody>;

} // namespace rmk

#include <remake2d/template/actor.tpp>

#endif