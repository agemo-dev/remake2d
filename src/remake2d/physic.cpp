
#include <remake2d/physic.hpp>
#include <remake2d/tracker.hpp>
#include <remake2d/utility.hpp>
#include <remake2d/math.hpp>

#include <algorithm>
#include <atomic>

namespace rmk {

constexpr usize  RESERVED_STATICS  = 75;
constexpr usize  RESERVED_DYNAMICS = 100;
constexpr usize  RESERVED_BODIES   = RESERVED_DYNAMICS + RESERVED_STATICS;

constexpr u16 MAX_SUB_STEPS_PER_FRAME = 5;

PhysicManager::PhysicManager(void) {
    b2WorldDef worldDef  = b2DefaultWorldDef();
    worldDef.gravity     = {0.0f, m_gravity};
    m_world              = b2CreateWorld(&worldDef);

	m_bodies  .reserve(RESERVED_BODIES);
	m_statics .reserve(RESERVED_STATICS);
	m_dynamics.reserve(RESERVED_DYNAMICS);
}

PhysicManager::~PhysicManager(void) {
    if (b2World_IsValid(m_world)) b2DestroyWorld(m_world);
}

PhysicManager& PhysicManager::getInstance(void) {
    static PhysicManager instance;
    return instance;
}

PhysicBody* PhysicManager::_ownerOf(void* shapeUserData) noexcept {
    if (!shapeUserData) return nullptr;
    return reinterpret_cast<Slot<PhysicBody>*>(shapeUserData)->ptr;
}

void PhysicManager::remove(PhysicBody& body) {
    m_body_map.erase(body.m_id);

    auto it = std::find_if(m_bodies.begin(), m_bodies.end(),
        [&body](Tracker<PhysicBody>& t) { return t.locate() == &body; });
    if (it == m_bodies.end())
        rmk_dynamicAssert(rmk::PhysicError, error::physic::body_not_found);

    body._detachFromWorld();
    m_bodies.erase(it);

    auto* dyn = dynamic_cast<DynamicBody*>(&body);
    if (dyn) {
        auto it2 = std::find_if(m_dynamics.begin(), m_dynamics.end(),
            [dyn](Tracker<DynamicBody>& t) { return t.locate() == dyn; });
        if (it2 != m_dynamics.end()) m_dynamics.erase(it2);
    } else {
        auto* stc = dynamic_cast<StaticBody*>(&body);
        if (stc) {
            auto it3 = std::find_if(m_statics.begin(), m_statics.end(),
                [stc](Tracker<StaticBody>& t) { return t.locate() == stc; });
            if (it3 != m_statics.end()) m_statics.erase(it3);
        }
    }
}

Area PhysicManager::world(void) const noexcept { return m_world_size; }

void PhysicManager::gravitationalConstant(f32 g) noexcept {
    m_gravity = g;
    if (b2World_IsValid(m_world))
        b2World_SetGravity(m_world, {0.0f, g});
}

f32 PhysicManager::gravitationalConstant(void) const noexcept { return m_gravity; }

void PhysicManager::pixelsPerMeter(f32 ptm) noexcept { m_ptm = ptm > 0.0f ? ptm : 32.0f; }
f32  PhysicManager::pixelsPerMeter(void)    const noexcept { return m_ptm; }

void PhysicManager::useFixedStep(bool s) noexcept { m_use_fixed_step = s; m_accumulator = 0.0; }
bool PhysicManager::useFixedStep(void)   const noexcept { return m_use_fixed_step; }

void PhysicManager::fixedStep(f32 s) noexcept { m_fixed_step = s > 0.0f ? s : 1.0f / 60.0f; }
f32  PhysicManager::fixedStep(void)  const noexcept { return m_fixed_step; }

std::vector<Tracker<PhysicBody>>  PhysicManager::bodies(void)    noexcept { return m_bodies;   }
std::vector<Tracker<StaticBody>>  PhysicManager::statics(void)   noexcept { return m_statics;  }
std::vector<Tracker<DynamicBody>> PhysicManager::dynamics(void)  noexcept { return m_dynamics; }

void PhysicManager::world(const Area& area) noexcept {
    if (m_world_size.x == area.x && m_world_size.y == area.y &&
        m_world_size.w == area.w && m_world_size.h == area.h) return;

    m_world_size = area;
    _rebuildBoundary();
}

void PhysicManager::_rebuildBoundary(void) {
    // Destroy previous boundary walls first: each StaticBody unregisters and
    // detaches its own Box2D body via its destructor.
    m_boundary_walls.clear();

    if (m_world_size.w <= 0 || m_world_size.h <= 0) return;

    f32 thick  = 50.0f;
    f32 half_t = thick / 2.0f;

    f32 wx = (f32)m_world_size.x;
    f32 wy = (f32)m_world_size.y;
    f32 ww = (f32)m_world_size.w;
    f32 wh = (f32)m_world_size.h;

    f32 cx = wx + ww / 2.0f;
    f32 cy = wy + wh / 2.0f;

    f32 wall_left   = wx - half_t;
    f32 wall_right  = wx + ww + half_t;
    f32 wall_top    = wy - half_t;
    f32 wall_bottom = wy + wh + half_t;

    f32 extended_height = wh + (thick * 2.0f);

    auto addWall = [&](const Vec2d& center, const Dim2d& dim) {
        Rectangle rect(center, dim);
        auto wall = std::make_unique<StaticBody>(rect);
        wall->isSolid(true);
        m_boundary_walls.push_back(std::move(wall));
    };

    addWall({cx, wall_top},    {ww, thick});
    addWall({cx, wall_bottom}, {ww, thick});
    addWall({wall_left,  cy},  {thick, extended_height});
    addWall({wall_right, cy},  {thick, extended_height});
}

void PhysicManager::_registerBody(PhysicBody *body) noexcept {
    // If this id was already registered (e.g. copy-assignment reusing a
    // still-registered body, or a stale entry), detach the old Box2D body
    // first so we never leak or double-register the same id.
    auto existing = m_body_map.find(body->m_id);
    if (existing != m_body_map.end() && existing->second.locate() != body) {
        // Different object claiming an id already in use: never happens with
        // _nextId()-assigned ids, but guard defensively.
        return;
    }

    body->_build(m_world);
    m_bodies.push_back(body->tracker());

    if (auto* s = dynamic_cast<StaticBody*>(body))       m_statics.push_back(s->tracker());
    else if (auto* d = dynamic_cast<DynamicBody*>(body)) m_dynamics.push_back(d->tracker());

    m_body_map[body->m_id] = body->tracker();
}

void PhysicManager::_unregisterBody(PhysicBody *body) noexcept {
    m_body_map.erase(body->m_id);

    m_bodies.erase(std::remove_if(m_bodies.begin(), m_bodies.end(),
        [body](Tracker<PhysicBody>& t) { return t.locate() == body; }), m_bodies.end());

    if (auto* s = dynamic_cast<StaticBody*>(body)) {
        m_statics.erase(std::remove_if(m_statics.begin(), m_statics.end(),
            [s](Tracker<StaticBody>& t) { return t.locate() == s; }), m_statics.end());
    } else if (auto* d = dynamic_cast<DynamicBody*>(body)) {
        m_dynamics.erase(std::remove_if(m_dynamics.begin(), m_dynamics.end(),
            [d](Tracker<DynamicBody>& t) { return t.locate() == d; }), m_dynamics.end());
    }
}

bool PhysicManager::_isValidBody(PhysicBody *body) const {
    if (!body) return false;
    auto it = m_body_map.find(body->m_id);
    return it != m_body_map.end() && it->second.locate() == body;
}

void PhysicManager::_stepAndDispatch(f32 step, i32 sub_steps) noexcept {
    b2World_Step(m_world, step, sub_steps);

    b2ContactEvents events = b2World_GetContactEvents(m_world);

    for (int i = 0; i < events.beginCount; ++i) {
        auto* a = _ownerOf(b2Shape_GetUserData(events.beginEvents[i].shapeIdA));
        auto* b = _ownerOf(b2Shape_GetUserData(events.beginEvents[i].shapeIdB));
        if (a && b && _isValidBody(a) && _isValidBody(b)) {
            a->onContactStart._evaluate(a, b);
            b->onContactStart._evaluate(b, a);
        }
    }

    for (int i = 0; i < events.hitCount; ++i) {
        auto* a = _ownerOf(b2Shape_GetUserData(events.hitEvents[i].shapeIdA));
        auto* b = _ownerOf(b2Shape_GetUserData(events.hitEvents[i].shapeIdB));
        if (a && b && _isValidBody(a) && _isValidBody(b)) {
            a->onContact._evaluate(a, b);
            b->onContact._evaluate(b, a);
        }
    }

    for (int i = 0; i < events.endCount; ++i) {
        auto* a = _ownerOf(b2Shape_GetUserData(events.endEvents[i].shapeIdA));
        auto* b = _ownerOf(b2Shape_GetUserData(events.endEvents[i].shapeIdB));
        if (a && b && _isValidBody(a) && _isValidBody(b)) {
            a->onContactEnd._evaluate(a, b);
            b->onContactEnd._evaluate(b, a);
        }
    }
}

void PhysicManager::update(void) {
    if (m_bodies.empty()) return;

    fmax tick = delta.tick();
    constexpr i32 sub_steps = 4;

    if (m_use_fixed_step) {
        fmax clamped = (tick <= 0.0 || tick > 1.0) ? m_fixed_step : tick;
        m_accumulator += clamped;

        i32 executed = 0;
        while (m_accumulator >= m_fixed_step && executed < MAX_SUB_STEPS_PER_FRAME) {
            _stepAndDispatch(m_fixed_step, sub_steps);
            m_accumulator -= m_fixed_step;
            ++executed;
        }
    } else {
        f32 step = (tick <= 0.0 || tick > 0.1) ? 1.0f / 60.0f : (f32)tick;
        _stepAndDispatch(step, sub_steps);
    }

    for (auto& t : m_bodies) {
        PhysicBody* body = t.locate();
        if (!body) continue;

        if (auto* dyn = dynamic_cast<DynamicBody*>(body))
            dyn->_syncAndUpdate();
        else
            body->_sync();

        if (body->m_vertices_dirty) body->_calculateVertices();
    }
}

} // namespace rmk