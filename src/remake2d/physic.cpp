#include <remake2d/physic.hpp>
#include <remake2d/utility.hpp>
#include <remake2d/math.hpp>

#include <algorithm>
#include <atomic>

#define RESERVED_STATICS   75
#define RESERVED_DYNAMICS  100
#define RESERVED_BODIES RESERVED_DYNAMICS + RESERVED_STATICS

namespace rmk {

u64 PhysicBody::_nextId(void) {
    static std::atomic<u64> counter{1};
    return counter.fetch_add(1);
}

PhysicBody::PhysicBody(const Geometry& shape)
    : m_id(_nextId()), m_needs_sync(true)
{
    u8           n      = shape.points();
    const Vec2d* pts    = shape.pointsPos();
    Vec2d        center = shape.center();

    m_shape_cache.center    = center;
    m_shape_cache.is_circle = (n == 36);

    if (m_shape_cache.is_circle) {
        m_shape_cache.radius = shape.size().w / 2.0f;
    } else {
        m_shape_cache.points.assign(pts, pts + n);
    }

    _calculateVertices();
}

PhysicBody::PhysicBody(const PhysicBody& other)
    : m_shape_cache(other.m_shape_cache)
    , m_animations(other.m_animations)
    , m_focused_anim(other.m_focused_anim)
    , m_tag(other.m_tag)
    , m_solid(other.m_solid)
    , m_needs_sync(true)
    , m_id(_nextId())
    , m_current_angle(other.m_current_angle)
{
    _calculateVertices();
}

PhysicBody& PhysicBody::operator=(const PhysicBody& other) {
    if (this != &other) {
        _detachFromWorld();
        m_shape_cache  = other.m_shape_cache;
        m_animations   = other.m_animations;
        m_focused_anim = other.m_focused_anim;
        m_tag          = other.m_tag;
        m_solid        = other.m_solid;
        m_needs_sync   = true;
        m_vertices_dirty = true;
        m_current_angle = other.m_current_angle;
        _calculateVertices();
    }
    return *this;
}

PhysicBody::PhysicBody(PhysicBody&& other) noexcept
    : m_body(other.m_body)
    , m_shape_id(other.m_shape_id)
    , m_shape_cache(std::move(other.m_shape_cache))
    , m_cached_vertices(std::move(other.m_cached_vertices))
    , m_cached_contour(std::move(other.m_cached_contour))
    , m_vertices_dirty(other.m_vertices_dirty)
    , m_animations(std::move(other.m_animations))
    , m_focused_anim(std::move(other.m_focused_anim))
    , m_tag(std::move(other.m_tag))
    , m_solid(other.m_solid)
    , m_needs_sync(other.m_needs_sync)
    , m_id(other.m_id)
    , m_cached_velocity(other.m_cached_velocity)
    , m_current_angle(other.m_current_angle)
{
    onContact      = std::move(other.onContact);
    onContactEnd   = std::move(other.onContactEnd);
    onContactStart = std::move(other.onContactStart);

    other.m_body      = b2_nullBodyId;
    other.m_shape_id  = b2_nullShapeId;
    other.m_id        = 0;
    other.m_current_angle = 0.0f;

    physics._rebindBody(&other, this);
}

PhysicBody& PhysicBody::operator=(PhysicBody&& other) noexcept {
    if (this != &other) {
        _detachFromWorld();

        m_body            = other.m_body;
        m_shape_id        = other.m_shape_id;
        m_shape_cache     = std::move(other.m_shape_cache);
        m_cached_vertices = std::move(other.m_cached_vertices);
        m_cached_contour  = std::move(other.m_cached_contour);
        m_vertices_dirty  = other.m_vertices_dirty;
        m_animations      = std::move(other.m_animations);
        m_focused_anim    = std::move(other.m_focused_anim);
        m_tag             = std::move(other.m_tag);
        m_solid           = other.m_solid;
        m_needs_sync      = other.m_needs_sync;
        m_id              = other.m_id;
        m_cached_velocity = other.m_cached_velocity;
        m_current_angle   = other.m_current_angle;

        onContact      = std::move(other.onContact);
        onContactEnd   = std::move(other.onContactEnd);
        onContactStart = std::move(other.onContactStart);

        other.m_body     = b2_nullBodyId;
        other.m_shape_id = b2_nullShapeId;
        other.m_id       = 0;
        other.m_current_angle = 0.0f;

        physics._rebindBody(&other, this);
    }
    return *this;
}

void PhysicBody::tag(std::string_view t)  noexcept { m_tag = std::string(t); }
std::string PhysicBody::tag(void)         const noexcept { return m_tag; }
u64 PhysicBody::ID(void)                  const noexcept { return m_id; }
Vec2d PhysicBody::center(void)            const noexcept { return m_shape_cache.center; }
Dim2d PhysicBody::size(void)              const noexcept {
    if (m_shape_cache.is_circle)
        return { m_shape_cache.radius * 2.0f, m_shape_cache.radius * 2.0f };
    if (m_shape_cache.points.empty()) return {0, 0};
    f32 minX = m_shape_cache.points[0].x, maxX = minX;
    f32 minY = m_shape_cache.points[0].y, maxY = minY;
    for (auto& p : m_shape_cache.points) {
        minX = std::min(minX, p.x); maxX = std::max(maxX, p.x);
        minY = std::min(minY, p.y); maxY = std::max(maxY, p.y);
    }
    return { maxX - minX, maxY - minY };
}

void PhysicBody::isSolid(bool status) noexcept {
    m_solid = status;
    if (b2Shape_IsValid(m_shape_id))
        b2Shape_EnableSensorEvents(m_shape_id, !status);
}

bool PhysicBody::isSolid(void) const noexcept {
	return m_solid;
}

void PhysicBody::move(const Vec2d& pos) noexcept {
    Vec2d offset = {
        pos.x - m_shape_cache.center.x,
        pos.y - m_shape_cache.center.y
    };

    m_shape_cache.center = pos;

    for (auto& p : m_shape_cache.points) {
        p.x += offset.x;
        p.y += offset.y;
    }

    if (b2Body_IsValid(m_body)) {
        Vec2d pm = physic::pixelToMeter(m_shape_cache.center);
        b2Body_SetTransform(m_body, {pm.x, pm.y}, b2Body_GetRotation(m_body));
    }

    m_vertices_dirty = true;
    _calculateVertices();
}

void PhysicBody::rotate(f32 angle) noexcept {
    f32 cosA = std::cos(angle);
    f32 sinA = std::sin(angle);
    Vec2d c  = m_shape_cache.center;
    for (auto& p : m_shape_cache.points) {
        f32 x = p.x - c.x;
        f32 y = p.y - c.y;
        p.x = c.x + x * cosA - y * sinA;
        p.y = c.y + x * sinA + y * cosA;
    }
    if (b2Body_IsValid(m_body)) {
        b2Rot rot = b2MakeRot(angle);
        b2Body_SetTransform(m_body, b2Body_GetPosition(m_body), rot);
    }
    m_vertices_dirty = true;
    _calculateVertices();
}

void PhysicBody::scale(const Fact2d& s) noexcept {
    Vec2d c = m_shape_cache.center;
    for (auto& p : m_shape_cache.points) {
        p.x = c.x + (p.x - c.x) * s.x;
        p.y = c.y + (p.y - c.y) * s.y;
    }
    if (m_shape_cache.is_circle)
        m_shape_cache.radius *= std::max(s.x, s.y);

    m_vertices_dirty = true;
    _calculateVertices();
    _rebuildShape();
}

void PhysicBody::resize(const Dim2d& newSize) noexcept {
    Dim2d current = size();
    if (current.w == 0.0f || current.h == 0.0f) return;
    scale({ newSize.w / current.w, newSize.h / current.h });
}

void PhysicBody::linkAnimation(std::string_view tag, const Animation& anim) {
	std::string t(tag);
	if (m_animations.empty()) m_focused_anim = t;
    m_animations.emplace(t, anim);
}

Animation& PhysicBody::animation(std::string_view name) {
    std::string n(name);
	if (n == nil && m_focused_anim != nil) {
		return m_animations.at(m_focused_anim);
	}

    auto it = m_animations.find(n);
    if (it == m_animations.end())
        rmk_dynamicAssert(rmk::PhysicError,
            (std::string(error::physic::animation_no_found) + n));
    return it->second;
}

const Animation& PhysicBody::animation(std::string_view name) const {
    std::string n(name);
	if (n == nil && m_focused_anim != nil) {
		return m_animations.at(m_focused_anim);
	}

    auto it = m_animations.find(n);
    if (it == m_animations.end())
        rmk_dynamicAssert(rmk::PhysicError,
            (std::string(error::physic::animation_no_found) + n));
    return it->second;
}

void PhysicBody::focusAnimation(std::string_view name) {
	std::string n(name);
	auto it = m_animations.find(n);
    if (it == m_animations.end())
        rmk_dynamicAssert(rmk::PhysicError,
            (std::string(error::physic::animation_no_found) + n));
	m_focused_anim = n;

}

void PhysicBody::_calculateVertices(void) noexcept {

	if(m_focused_anim != nil) return;

    m_cached_vertices.clear();
    m_cached_contour.clear();

    if (m_shape_cache.is_circle) {
        constexpr u32 segments = 36;
        Vec2d c  = m_shape_cache.center;
        f32   r  = m_shape_cache.radius;

        for (u32 i = 0; i <= segments; i++) {
            f32   angle = 2.0f * pi * i / segments;
            Vec2d p     = { c.x + r * std::cos(angle), c.y + r * std::sin(angle) };
            m_cached_contour.push_back({p.x, p.y});
        }
        for (u32 i = 1; i < segments - 1; i++) {
            auto toVertex = [](SDL_FPoint p) -> SDL_Vertex {
                return { {p.x, p.y}, {255, 255, 255, 255}, {0, 0} };
            };
            m_cached_vertices.push_back(toVertex(m_cached_contour[0]));
            m_cached_vertices.push_back(toVertex(m_cached_contour[i]));
            m_cached_vertices.push_back(toVertex(m_cached_contour[i + 1]));
        }
    } else {
        auto& pts = m_shape_cache.points;
        u32   n   = pts.size();
        if (n == 0) { m_vertices_dirty = false; return; }
        for (u32 i = 0; i < n; i++)
            m_cached_contour.push_back({pts[i].x, pts[i].y});
        m_cached_contour.push_back({pts[0].x, pts[0].y});

        for (u32 i = 1; i + 1 < n; i++) {
            m_cached_vertices.push_back({{pts[0].x, pts[0].y},     {255,255,255,255}, {0,0}});
            m_cached_vertices.push_back({{pts[i].x, pts[i].y},     {255,255,255,255}, {0,0}});
            m_cached_vertices.push_back({{pts[i+1].x, pts[i+1].y}, {255,255,255,255}, {0,0}});
        }
    }
    m_vertices_dirty = false;
}

void PhysicBody::_sync(void) {
    if (!m_needs_sync) return;
    if (!b2Body_IsValid(m_body)) return;

    b2Vec2 pos   = b2Body_GetPosition(m_body);
    f32    angle = b2Rot_GetAngle(b2Body_GetRotation(m_body));
    f32 delta_angle = angle - m_current_angle;

    Vec2d new_center = physic::meterToPixel(Vec2d{pos.x, pos.y});

    move(new_center);
    if (delta_angle != 0.0f) rotate(delta_angle);

    for(auto& [t, a] : m_animations) {
        a.move(new_center);
        a.rotate(delta_angle);
    }

    m_current_angle  = angle;
    m_needs_sync     = false;
    m_vertices_dirty = true;
}

void PhysicBody::_build(b2WorldId world) {
    _initBody(world, b2_staticBody, 0.0f, 0.0f, 0.0f);
}

void PhysicBody::_detachFromWorld(void) noexcept {
    if (b2Body_IsValid(m_body)) {
        b2DestroyBody(m_body);
    }
    m_body     = b2_nullBodyId;
    m_shape_id = b2_nullShapeId;
}

void PhysicBody::_initBody(b2WorldId world, b2BodyType type,
                            f32 density, f32 friction, f32 bounce) {
    Vec2d pm = physic::pixelToMeter(m_shape_cache.center);

    b2BodyDef bodyDef    = b2DefaultBodyDef();
    bodyDef.type         = type;
    bodyDef.position     = {pm.x, pm.y};

    // userData points at the manager-owned Slot for this body's id, never at
    // `this`: `this` can become dangling if the owning PhysicBody lives inside
    // a container that reallocates (e.g. std::vector<DynamicBody>).
    bodyDef.userData     = reinterpret_cast<void*>(physics._slotFor(m_id));

    m_body               = b2CreateBody(world, &bodyDef);

    b2ShapeDef shapeDef  = b2DefaultShapeDef();
    shapeDef.isSensor    = !m_solid;
    shapeDef.density     = density;

    if (m_shape_cache.is_circle) {
        b2Circle circle = _makeB2Circle(m_shape_cache);
        m_shape_id      = b2CreateCircleShape(m_body, &shapeDef, &circle);
    } else {
        b2Polygon poly  = _makeB2Polygon(m_shape_cache);
        m_shape_id      = b2CreatePolygonShape(m_body, &shapeDef, &poly);
    }

    if (b2Shape_IsValid(m_shape_id)) {
        b2Shape_SetUserData(m_shape_id, reinterpret_cast<void*>(physics._slotFor(m_id)));
        b2Shape_SetFriction(m_shape_id, friction);
        b2Shape_SetRestitution(m_shape_id, bounce);
        b2Shape_EnableSensorEvents(m_shape_id, !m_solid);
        b2Shape_EnableContactEvents(m_shape_id, true);
    }
    m_current_angle = 0.0f;
    m_needs_sync    = true;
}

void PhysicBody::_rebuildShape(void) {
    if (!b2Body_IsValid(m_body)) return;

    f32 friction    = b2Shape_IsValid(m_shape_id) ? b2Shape_GetFriction(m_shape_id)    : 0.0f;
    f32 restitution = b2Shape_IsValid(m_shape_id) ? b2Shape_GetRestitution(m_shape_id) : 0.0f;
    bool sensorEv   = b2Shape_IsValid(m_shape_id) ? true : true;
    (void)sensorEv;

    if (b2Shape_IsValid(m_shape_id)) {
        b2DestroyShape(m_shape_id);
        m_shape_id = b2_nullShapeId;
    }

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.isSensor   = !m_solid;

    if (m_shape_cache.is_circle) {
        b2Circle circle = _makeB2Circle(m_shape_cache);
        m_shape_id      = b2CreateCircleShape(m_body, &shapeDef, &circle);
    } else {
        b2Polygon poly  = _makeB2Polygon(m_shape_cache);
        m_shape_id      = b2CreatePolygonShape(m_body, &shapeDef, &poly);
    }

    if (b2Shape_IsValid(m_shape_id)) {
        b2Shape_SetUserData(m_shape_id, reinterpret_cast<void*>(physics._slotFor(m_id)));
        b2Shape_SetFriction(m_shape_id, friction);
        b2Shape_SetRestitution(m_shape_id, restitution);
        b2Shape_EnableSensorEvents(m_shape_id, !m_solid);
        b2Shape_EnableContactEvents(m_shape_id, true);
    }
}

b2Polygon PhysicBody::_makeB2Polygon(const PhysicBody::ShapeCache& cache) {
    Vec2d center = cache.center;
    u32   n      = cache.points.size();

    if (n == 4) {
        Vec2d p0 = cache.points[0];
        Vec2d p2 = cache.points[2];
        Dim2d size = {
            std::abs(p2.x - p0.x),
            std::abs(p2.y - p0.y)
        };
        bool isAxisAligned =
            p0.x == center.x - size.w / 2 &&
            p0.y == center.y - size.h / 2 &&
            p2.x == center.x + size.w / 2 &&
            p2.y == center.y + size.h / 2;

        if (isAxisAligned) {
            Vec2d halfM = physic::pixelToMeter(Vec2d{size.w / 2.0f, size.h / 2.0f});
            return b2MakeBox(halfM.x, halfM.y);
        }
    }

    b2Vec2 verts[b2_maxPolygonVertices];
    for (u32 i = 0; i < n; i++) {
        Vec2d localPx = { cache.points[i].x - center.x, cache.points[i].y - center.y };
        Vec2d localM  = physic::pixelToMeter(localPx);
        verts[i] = {localM.x, localM.y};
    }

    b2Hull hull = b2ComputeHull(verts, n);
    if (hull.count == 0)
        rmk_dynamicAssert(rmk::PhysicError, error::physic::invalid_shape);

    return b2MakePolygon(&hull, 0.0f);
}

b2Circle PhysicBody::_makeB2Circle(const PhysicBody::ShapeCache& cache) {
    b2Circle circle;
    circle.center = {0.0f, 0.0f};
    circle.radius = physic::pixelToMeter(cache.radius);
    return circle;
}

PhysicBody::~PhysicBody(void) {
    physics._unregisterBody(this);
    _detachFromWorld();
}

StaticBody::StaticBody(const Geometry& shape) : PhysicBody(shape) {
	physics._registerBody(this);
}

StaticBody::StaticBody(const StaticBody& other) : PhysicBody(other) {
    physics._registerBody(this);
}

StaticBody& StaticBody::operator=(const StaticBody& other) {
    if (this != &other) {
        PhysicBody::operator=(other);
        physics._registerBody(this);
    }
    return *this;
}

StaticBody::StaticBody(StaticBody&& other) noexcept : PhysicBody(std::move(other)) {
    physics._rebindBody(&other, this);
}

StaticBody& StaticBody::operator=(StaticBody&& other) noexcept {
    if (this != &other) {
        physics._unregisterBody(this);
        PhysicBody::operator=(std::move(other));
        physics._rebindBody(&other, this);
    }
    return *this;
}

DynamicBody::DynamicBody(const Geometry& shape) : PhysicBody(shape) {
	physics._registerBody(this);
}

DynamicBody::DynamicBody(const DynamicBody& other) : PhysicBody(other) {
    m_mass             = other.m_mass;
    m_bounce           = other.m_bounce;
    m_bounce_threshold = other.m_bounce_threshold;
    m_infinite_bounce  = other.m_infinite_bounce;
    m_is_bullet        = other.m_is_bullet;
    m_gravity_enabled  = other.m_gravity_enabled;
    m_friction         = other.m_friction;
    m_warp_area        = other.m_warp_area;
    m_personal_limit   = other.m_personal_limit;

    physics._registerBody(this);
}

DynamicBody& DynamicBody::operator=(const DynamicBody& other) {
    if (this != &other) {
        PhysicBody::operator=(other);
        m_mass             = other.m_mass;
        m_bounce           = other.m_bounce;
        m_bounce_threshold = other.m_bounce_threshold;
        m_infinite_bounce  = other.m_infinite_bounce;
        m_is_bullet        = other.m_is_bullet;
        m_gravity_enabled  = other.m_gravity_enabled;
        m_friction         = other.m_friction;
        m_warp_area        = other.m_warp_area;
        m_personal_limit   = other.m_personal_limit;

        physics._registerBody(this);
    }
    return *this;
}

DynamicBody::DynamicBody(DynamicBody&& other) noexcept
    : PhysicBody(std::move(other))
    , m_mass(other.m_mass)
    , m_bounce(other.m_bounce)
    , m_bounce_threshold(other.m_bounce_threshold)
    , m_infinite_bounce(other.m_infinite_bounce)
    , m_is_bullet(other.m_is_bullet)
    , m_gravity_enabled(other.m_gravity_enabled)
    , m_friction(other.m_friction)
    , m_warp_area(other.m_warp_area)
    , m_personal_limit(other.m_personal_limit)
{
    onMove      = std::move(other.onMove);
    onMoveUp    = std::move(other.onMoveUp);
    onMoveDown  = std::move(other.onMoveDown);
    onMoveLeft  = std::move(other.onMoveLeft);
    onMoveRight = std::move(other.onMoveRight);
    physics._rebindBody(&other, this);
}

DynamicBody& DynamicBody::operator=(DynamicBody&& other) noexcept {
    if (this != &other) {
        physics._unregisterBody(this);
        PhysicBody::operator=(std::move(other));
        m_mass             = other.m_mass;
        m_bounce           = other.m_bounce;
        m_bounce_threshold = other.m_bounce_threshold;
        m_infinite_bounce  = other.m_infinite_bounce;
        m_is_bullet        = other.m_is_bullet;
        m_gravity_enabled  = other.m_gravity_enabled;
        m_friction         = other.m_friction;
        m_warp_area        = other.m_warp_area;
        m_personal_limit   = other.m_personal_limit;

        onMove      = std::move(other.onMove);
        onMoveUp    = std::move(other.onMoveUp);
        onMoveDown  = std::move(other.onMoveDown);
        onMoveLeft  = std::move(other.onMoveLeft);
        onMoveRight = std::move(other.onMoveRight);

        physics._rebindBody(&other, this);
    }
    return *this;
}

void DynamicBody::mass(f32 m) noexcept {
    m_mass = m;
    if (b2Body_IsValid(m_body) && b2Shape_IsValid(m_shape_id) && m > 0.0f) {
        b2MassData md = b2Body_GetMassData(m_body);
        if (md.mass > 0.0f) {
            f32 currentDensity = b2Shape_GetDensity(m_shape_id);
            f32 newDensity     = currentDensity * (m / md.mass);
            b2Shape_SetDensity(m_shape_id, newDensity);
            b2Body_ApplyMassFromShapes(m_body);
            m_density = newDensity;
        }
    }
    m_needs_sync = true;
}

void DynamicBody::density(f32 d) noexcept {
    m_density = d;
    if (b2Body_IsValid(m_body) && b2Shape_IsValid(m_shape_id) && d > 0.0f) {
        b2Shape_SetDensity(m_shape_id, d);
        b2Body_ApplyMassFromShapes(m_body);
        b2MassData md = b2Body_GetMassData(m_body);
        if (md.mass > 0.0f) m_mass = md.mass;
    }
    m_needs_sync = true;
}

f32 DynamicBody::density(void) const noexcept {
    return m_density;
}

f32  DynamicBody::mass(void)            const noexcept { return m_mass; }
f32  DynamicBody::bounce(void)          const noexcept { return m_bounce; }
f32  DynamicBody::bounceThreshold(void) const noexcept { return m_bounce_threshold; }
bool DynamicBody::infiniteBounce(void)  const noexcept { return m_infinite_bounce; }
f32  DynamicBody::friction(void)        const noexcept { return m_friction; }
bool DynamicBody::gravity(void)         const noexcept { return m_gravity_enabled; }
bool DynamicBody::isBullet(void)        const noexcept { return m_is_bullet; }
Area DynamicBody::warp(void)            const noexcept { return m_warp_area; }
Area DynamicBody::limit(void)           const noexcept { return m_personal_limit; }

void DynamicBody::bounce(f32 b) noexcept {
    m_bounce = std::clamp(b, 0.0f, 1.0f);
    if (b2Shape_IsValid(m_shape_id))
        b2Shape_SetRestitution(m_shape_id, m_bounce);
}

void DynamicBody::bounceThreshold(f32 t) noexcept { m_bounce_threshold = t; }

void DynamicBody::infiniteBounce(bool status) noexcept {
    m_infinite_bounce = status;
    bounceThreshold(status ? 0.0f : 0.5f);
}

void DynamicBody::friction(f32 f) noexcept {
    m_friction = f;
    if (b2Shape_IsValid(m_shape_id))
        b2Shape_SetFriction(m_shape_id, f);
}

void DynamicBody::gravity(bool status) noexcept {
    m_gravity_enabled = status;
    if (b2Body_IsValid(m_body))
        b2Body_SetGravityScale(m_body, status ? 1.0f : 0.0f);
}

void DynamicBody::isBullet(bool b) noexcept {
    m_is_bullet = b;
    if (b2Body_IsValid(m_body))
        b2Body_SetBullet(m_body, b);
}

void DynamicBody::warp(const Area& area)  noexcept { m_warp_area      = area; }
void DynamicBody::limit(const Area& area) noexcept { m_personal_limit = area; }

void DynamicBody::jump(f32 force) noexcept {
    if (b2Body_IsValid(m_body)) {
        f32 forceM = physic::pixelToMeter(force);
        b2Body_ApplyLinearImpulseToCenter(m_body, {0.0f, -forceM}, true);
        b2Body_SetAwake(m_body, true);
        m_needs_sync = true;
    }
}

Vec2d DynamicBody::velocity(void) const noexcept {
    if (!b2Body_IsValid(m_body)) return {0.0f, 0.0f};
    b2Vec2 v = b2Body_GetLinearVelocity(m_body);
    return physic::meterToPixel(Vec2d{v.x, v.y});
}

void DynamicBody::velocity(const Vec2d& v) noexcept {
    if (b2Body_IsValid(m_body)) {
        Vec2d vm = physic::pixelToMeter(v);
        b2Body_SetLinearVelocity(m_body, {vm.x, vm.y});
        b2Body_SetAwake(m_body, true);
        m_needs_sync = true;
    }
}

void DynamicBody::push(const Vec2d& force) noexcept {
    if (b2Body_IsValid(m_body)) {
        Vec2d fm = physic::pixelToMeter(force);
        b2Body_ApplyForceToCenter(m_body, {fm.x, fm.y}, true);
        b2Body_SetAwake(m_body, true);
        m_needs_sync = true;
    }
}

void DynamicBody::_applyWarp(void) {
    if (m_warp_area.w <= 0 || m_warp_area.h <= 0) return;
    if (!b2Body_IsValid(m_body)) return;

    b2Vec2 pos = b2Body_GetPosition(m_body);
    Vec2d  xy  = physic::meterToPixel(Vec2d{pos.x, pos.y});
    f32    x   = xy.x;
    f32    y   = xy.y;

    f32 left   = (f32)m_warp_area.x;
    f32 right  = (f32)m_warp_area.x + (f32)m_warp_area.w;
    f32 top    = (f32)m_warp_area.y;
    f32 bottom = (f32)m_warp_area.y + (f32)m_warp_area.h;

    bool moved = false;
    if (x < left)        { x = right; moved = true; }
    else if (x > right)  { x = left;  moved = true; }
    if (y < top)         { y = bottom; moved = true; }
    else if (y > bottom) { y = top;    moved = true; }

    if (!moved) return;

    Vec2d pm = physic::pixelToMeter(Vec2d{x, y});
    b2Body_SetTransform(m_body, {pm.x, pm.y}, b2Body_GetRotation(m_body));
    m_needs_sync = true;
}

void DynamicBody::_applyLimit(void) {
    if (m_personal_limit.w <= 0 || m_personal_limit.h <= 0) return;
    if (!b2Body_IsValid(m_body)) return;

    b2Vec2 pos = b2Body_GetPosition(m_body);
    Vec2d  xy  = physic::meterToPixel(Vec2d{pos.x, pos.y});
    f32    x   = xy.x;
    f32    y   = xy.y;

    f32  left   = (f32)m_personal_limit.x;
    f32  right  = (f32)m_personal_limit.x + (f32)m_personal_limit.w;
    f32  top    = (f32)m_personal_limit.y;
    f32  bottom = (f32)m_personal_limit.y + (f32)m_personal_limit.h;
    bool moved  = false;

    if (x < left)        { x = left;   moved = true; }
    else if (x > right)  { x = right;  moved = true; }
    if (y < top)         { y = top;    moved = true; }
    else if (y > bottom) { y = bottom; moved = true; }

    if (moved) {
        Vec2d pm = physic::pixelToMeter(Vec2d{x, y});
        b2Vec2 v = b2Body_GetLinearVelocity(m_body);
        b2Body_SetTransform(m_body, {pm.x, pm.y}, b2Body_GetRotation(m_body));
        if (x == left || x == right) v.x = 0.0f;
        if (y == top  || y == bottom) v.y = 0.0f;
        b2Body_SetLinearVelocity(m_body, v);
        m_needs_sync = true;
    }
}

void DynamicBody::_syncAndUpdate(void) {
    if (!b2Body_IsValid(m_body)) return;

    bool awake = b2Body_IsAwake(m_body);

    if (m_needs_sync || awake) {
        b2Vec2 pos   = b2Body_GetPosition(m_body);
        f32    angle = b2Rot_GetAngle(b2Body_GetRotation(m_body));
        f32 delta_angle = angle - m_current_angle;

        Vec2d new_center = physic::meterToPixel(Vec2d{pos.x, pos.y});

        PhysicBody::move(new_center);
        if (delta_angle != 0.0f) rotate(delta_angle);

        for(auto& [t, a] : m_animations) {
            a.move(new_center);
            a.rotate(delta_angle);
        }

        b2Vec2 v          = b2Body_GetLinearVelocity(m_body);
        m_cached_velocity = physic::meterToPixel(Vec2d{v.x, v.y});
        m_current_angle   = angle;
        m_needs_sync      = false;
        m_vertices_dirty  = true;
    } else {
        m_cached_velocity = {0.0f, 0.0f};
    }

    _applyWarp();
    _applyLimit();

    if (m_cached_velocity.x != 0.0f || m_cached_velocity.y != 0.0f) onMove._evaluate(this);
    if (m_cached_velocity.y < -0.01f) onMoveUp._evaluate(this);
    if (m_cached_velocity.y >  0.01f) onMoveDown._evaluate(this);
    if (m_cached_velocity.x < -0.01f) onMoveLeft._evaluate(this);
    if (m_cached_velocity.x >  0.01f) onMoveRight._evaluate(this);
}

void DynamicBody::_build(b2WorldId world) {
    _initBody(world, b2_dynamicBody, m_density, m_friction, m_bounce);
    if (b2Body_IsValid(m_body)) {
        b2Body_SetBullet(m_body, m_is_bullet);
        b2Body_SetGravityScale(m_body, m_gravity_enabled ? 1.0f : 0.0f);
        b2Body_SetFixedRotation(m_body, false);

        b2MassData md = b2Body_GetMassData(m_body);
        if (md.mass > 0.0f) m_mass = md.mass;
    }
}

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

PhysicManager::Slot* PhysicManager::_slotFor(u64 id) noexcept {
    auto it = m_slots.find(id);
    if (it != m_slots.end()) return it->second.get();

    auto slot  = std::make_unique<Slot>();
    slot->id   = id;
    Slot* raw  = slot.get();
    m_slots.emplace(id, std::move(slot));
    return raw;
}

PhysicBody* PhysicManager::_ownerOf(void* shapeUserData) noexcept {
    if (!shapeUserData) return nullptr;
    return reinterpret_cast<Slot*>(shapeUserData)->owner;
}

void PhysicManager::remove(PhysicBody& body) {
    m_body_map.erase(body.m_id);

    auto it = std::find(m_bodies.begin(), m_bodies.end(), &body);
    if (it == m_bodies.end())
        rmk_dynamicAssert(rmk::PhysicError, error::physic::body_not_found);

    body._detachFromWorld();
    m_bodies.erase(it);

    auto* dyn = dynamic_cast<DynamicBody*>(&body);
    if (dyn) {
        auto it2 = std::find(m_dynamics.begin(), m_dynamics.end(), dyn);
        if (it2 != m_dynamics.end()) m_dynamics.erase(it2);
    } else {
        auto* stc = dynamic_cast<StaticBody*>(&body);
        if (stc) {
            auto it3 = std::find(m_statics.begin(), m_statics.end(), stc);
            if (it3 != m_statics.end()) m_statics.erase(it3);
        }
    }

    m_slots.erase(body.m_id);
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

std::vector<PhysicBody*>&  PhysicManager::bodies(void)    noexcept { return m_bodies;   }
std::vector<StaticBody*>&  PhysicManager::statics(void)   noexcept { return m_statics;  }
std::vector<DynamicBody*>& PhysicManager::dynamics(void)  noexcept { return m_dynamics; }

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
    if (existing != m_body_map.end() && existing->second != body) {
        // Different object claiming an id already in use: never happens with
        // _nextId()-assigned ids, but guard defensively.
        return;
    }

    body->_build(m_world);
    m_bodies.push_back(body);

    if (auto *s = dynamic_cast<StaticBody*>(body))        m_statics.push_back(s);
    else if (auto *d = dynamic_cast<DynamicBody*>(body))  m_dynamics.push_back(d);

    m_body_map[body->m_id] = body;

    Slot* slot   = _slotFor(body->m_id);
    slot->owner  = body;
}

void PhysicManager::_unregisterBody(PhysicBody *body) noexcept {
    m_body_map.erase(body->m_id);

    auto remove = [](auto& vec, const auto *ptr) {
        vec.erase(std::remove(vec.begin(), vec.end(), ptr), vec.end());
    };

    remove(m_bodies, body);

    if (auto *s = dynamic_cast<StaticBody*>(body))        remove(m_statics, s);
    else if (auto *d = dynamic_cast<DynamicBody*>(body))  remove(m_dynamics, d);

    auto it = m_slots.find(body->m_id);
    if (it != m_slots.end() && it->second->owner == body) {
        it->second->owner = nullptr;
    }
}

void PhysicManager::_rebindBody(PhysicBody* old_addr, PhysicBody* new_addr) noexcept {
    if (new_addr->m_id == 0) return;

    auto it = m_body_map.find(new_addr->m_id);
    if (it != m_body_map.end()) it->second = new_addr;

    for (auto*& p : m_bodies)   if (p == old_addr) p = new_addr;
    for (auto*& p : m_statics)  if (p == (StaticBody*)old_addr)  p = static_cast<StaticBody*>(new_addr);
    for (auto*& p : m_dynamics) if (p == (DynamicBody*)old_addr) p = static_cast<DynamicBody*>(new_addr);

    auto sit = m_slots.find(new_addr->m_id);
    if (sit != m_slots.end()) sit->second->owner = new_addr;
}

bool PhysicManager::_isValidBody(PhysicBody *body) const {
    if (!body) return false;
    auto it = m_body_map.find(body->m_id);
    return it != m_body_map.end() && it->second == body;
}

void PhysicManager::update(void) {
    if (m_bodies.empty()) return;

    fmax tick     = delta.tick();
    f32  step     = (tick <= 0.0 || tick > 0.1) ? 1.0f / 60.0f : (f32)tick;
    constexpr i32 sub_steps = 4;

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

    for (auto* body : m_bodies) {
        if (auto* dyn = dynamic_cast<DynamicBody*>(body))
            dyn->_syncAndUpdate();
        else
            body->_sync();

        if (body->m_vertices_dirty) body->_calculateVertices();
    }
}

} // namespace rmk
