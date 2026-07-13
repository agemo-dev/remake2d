#include <remake2d/physic.hpp>
#include <remake2d/utility.hpp>
#include <remake2d/math.hpp>

#include <algorithm>
#include <atomic>

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
	physics._registerBody(this);
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

bool PhysicBody::isSolid(void) const noexcept { return m_solid; }
void PhysicBody::link(Sprite& tex) noexcept { m_texture = &tex; }

void PhysicBody::move(const Vec2d& delta) noexcept {
    
    Vec2d offset = {
        delta.x - m_shape_cache.center.x,
        delta.y - m_shape_cache.center.y
    };
	
    m_shape_cache.center = delta;
	
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
}

void PhysicBody::resize(const Dim2d& newSize) noexcept {
    Dim2d current = size();
    if (current.w == 0.0f || current.h == 0.0f) return;
    scale({ newSize.w / current.w, newSize.h / current.h });
	m_vertices_dirty = true;
	_calculateVertices();
}

void PhysicBody::linkAnimation(AnimEntry anim) {
    m_animations.emplace(anim.first, std::move(anim.second));
}

Animation& PhysicBody::animation(std::string_view name) {
    std::string n(name);
    auto it = m_animations.find(n);
    if (it == m_animations.end())
        rmk_dynamicAssert(rmk::PhysicError,
            (std::string(error::physic::animation_no_found) + n));
    return it->second;
}

void PhysicBody::_calculateVertices(void) noexcept {
    m_cached_vertices.clear();
    m_cached_contour.clear();

    if (m_shape_cache.is_circle) {
        constexpr u32 segments = 36;
        Vec2d c  = m_shape_cache.center;
        f32   r  = m_shape_cache.radius;
        Vec2d first = {c.x + r, c.y};

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
        for (u32 i = 0; i < n; i++)
            m_cached_contour.push_back({pts[i].x, pts[i].y});
        m_cached_contour.push_back({pts[0].x, pts[0].y});

        for (u32 i = 1; i < n - 1; i++) {
            m_cached_vertices.push_back({{pts[0].x, pts[0].y}, {255,255,255,255}, {0,0}});
            m_cached_vertices.push_back({{pts[i].x, pts[i].y}, {255,255,255,255}, {0,0}});
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

    Vec2d new_center = {
        pos.x * physic::meterToPixel(1.0f),
        pos.y * physic::meterToPixel(1.0f)
    };
    Vec2d delta = {
        new_center.x - m_shape_cache.center.x,
        new_center.y - m_shape_cache.center.y
    };

    if (delta.x == 0.0f && delta.y == 0.0f && angle == 0.0f) {
        m_needs_sync = false;
        return;
    }

    move(delta);
    if (angle != 0.0f) rotate(angle);

    if (m_texture) {
        m_texture->move(new_center);
        m_texture->rotate(angle);
    }

    m_needs_sync     = false;
    m_vertices_dirty = true;
}

void PhysicBody::_build(b2WorldId world) {
    _initBody(world, b2_staticBody, 0.0f, 0.0f, 0.0f);
}

void PhysicBody::_initBody(b2WorldId world, b2BodyType type,
                            f32 density, f32 friction, f32 bounce) {
    Vec2d pm = physic::pixelToMeter(m_shape_cache.center);

    b2BodyDef bodyDef    = b2DefaultBodyDef();
    bodyDef.type         = type;
    bodyDef.position     = {pm.x, pm.y};
    bodyDef.userData     = reinterpret_cast<void*>(this);
    m_body               = b2CreateBody(world, &bodyDef);

    b2ShapeDef shapeDef  = b2DefaultShapeDef();
    shapeDef.isSensor    = !m_solid;

    if (m_shape_cache.is_circle) {
        b2Circle circle = _makeB2Circle(m_shape_cache);
        m_shape_id      = b2CreateCircleShape(m_body, &shapeDef, &circle);
    } else {
        b2Polygon poly  = _makeB2Polygon(m_shape_cache);
        m_shape_id      = b2CreatePolygonShape(m_body, &shapeDef, &poly);
    }

    if (b2Shape_IsValid(m_shape_id)) {
        b2Shape_SetUserData(m_shape_id, reinterpret_cast<void*>(this));
        b2Shape_SetFriction(m_shape_id, friction);
        b2Shape_SetRestitution(m_shape_id, bounce);
    }

    m_needs_sync = true;
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
            return b2MakeBox(
                (size.w / 2.0f) / physic::pixelToMeter(1.0f),
                (size.h / 2.0f) / physic::pixelToMeter(1.0f)
            );
        }
    }

    b2Vec2 verts[b2_maxPolygonVertices];
    for (u32 i = 0; i < n; i++) {
        verts[i] = {
            (cache.points[i].x - center.x) / physic::pixelToMeter(1.0f),
            (cache.points[i].y - center.y) / physic::pixelToMeter(1.0f)
        };
    }

    b2Hull hull = b2ComputeHull(verts, n);
    if (hull.count == 0)
        rmk_dynamicAssert(rmk::PhysicError, error::physic::invalid_shape);

    return b2MakePolygon(&hull, 0.0f);
}

b2Circle PhysicBody::_makeB2Circle(const PhysicBody::ShapeCache& cache) {
    b2Circle circle;
    circle.center = {0.0f, 0.0f};
    circle.radius = cache.radius / physic::pixelToMeter(1.0f);
    return circle;
}

PhysicBody::~PhysicBody(void) {
	physics._unregisterBody(this);
}
	
StaticBody::StaticBody(const Geometry& shape) : PhysicBody(shape) {}

StaticBody::StaticBody(const StaticBody& other) : PhysicBody(other) {
	physics._registerBody(this);
}

StaticBody& StaticBody::operator=(const StaticBody& other) {
    if (this != &other) {
        if (b2Body_IsValid(m_body)) physics.remove(*this);
        PhysicBody::operator=(other);

		auto& map = physics.m_body_map;
		if (map.find(m_id) == map.end()) {
			physics._registerBody(this);
		}
	}
    return *this;
}

DynamicBody::DynamicBody(const Geometry& shape) : PhysicBody(shape) {}

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

		auto& map = physics.m_body_map;
		if (map.find(m_id) == map.end()) {
			physics._registerBody(this);
		}
	}
    return *this;
}

void DynamicBody::mass(f32 m) noexcept {
    m_mass = m;
    if (b2Body_IsValid(m_body)) {
        b2MassData md = b2Body_GetMassData(m_body);
        md.mass = m;
        b2Body_SetMassData(m_body, md);
    }
    m_needs_sync = true;
}

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
        b2Body_ApplyLinearImpulseToCenter(
            m_body, {0.0f, -force / physic::pixelToMeter(1.0f)}, true);
        m_needs_sync = true;
    }
}

Vec2d DynamicBody::velocity(void) const noexcept {
    if (!b2Body_IsValid(m_body)) return {0.0f, 0.0f};
    b2Vec2 v = b2Body_GetLinearVelocity(m_body);
    return { v.x * physic::meterToPixel(1.0f), v.y * physic::meterToPixel(1.0f) };
}

void DynamicBody::velocity(const Vec2d& v) noexcept {
    if (b2Body_IsValid(m_body)) {
        b2Body_SetLinearVelocity(m_body, {
            v.x / physic::pixelToMeter(1.0f),
            v.y / physic::pixelToMeter(1.0f)
        });
        m_needs_sync = true;
    }
}

void DynamicBody::move(const Vec2d& delta) noexcept {
    if (b2Body_IsValid(m_body)) {
        b2Body_ApplyForceToCenter(m_body, {
            delta.x / physic::pixelToMeter(1.0f),
            delta.y / physic::pixelToMeter(1.0f)
        }, true);
        m_needs_sync = true;
    }
}

void DynamicBody::_applyWarp(void) {
    if (m_warp_area.w <= 0 || m_warp_area.h <= 0) return;
    if (!b2Body_IsValid(m_body)) return;

    b2Vec2 pos = b2Body_GetPosition(m_body);
    f32 x = physic::meterToPixel(pos.x);
    f32 y = physic::meterToPixel(pos.y);

    f32 left   = (f32)m_warp_area.x;
    f32 right  = (f32)m_warp_area.x + (f32)m_warp_area.w;
    f32 top    = (f32)m_warp_area.y;
    f32 bottom = (f32)m_warp_area.y + (f32)m_warp_area.h;

    if (x < left)        x = right;
    else if (x > right)  x = left;
    if (y < top)         y = bottom;
    else if (y > bottom) y = top;

    Vec2d pm = physic::pixelToMeter(Vec2d(x, y));
    b2Body_SetTransform(m_body, {pm.x, pm.y}, b2Body_GetRotation(m_body));
    m_needs_sync = true;
}

void DynamicBody::_applyLimit(void) {
    if (m_personal_limit.w <= 0 || m_personal_limit.h <= 0) return;
    if (!b2Body_IsValid(m_body)) return;

    b2Vec2 pos = b2Body_GetPosition(m_body);
    f32 x = physic::meterToPixel(pos.x);
    f32 y = physic::meterToPixel(pos.y);

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
        Vec2d pm = physic::pixelToMeter(Vec2d(x, y));
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

    if (m_needs_sync) {
        b2Vec2 pos   = b2Body_GetPosition(m_body);
        f32    angle = b2Rot_GetAngle(b2Body_GetRotation(m_body));

        Vec2d new_center = {
            pos.x * physic::meterToPixel(1.0f),
            pos.y * physic::meterToPixel(1.0f)
        };
        Vec2d delta = {
            new_center.x - m_shape_cache.center.x,
            new_center.y - m_shape_cache.center.y
        };

        if (delta.x != 0.0f || delta.y != 0.0f) PhysicBody::move(delta);
        if (angle != 0.0f) PhysicBody::rotate(angle);

        if (m_texture) {
            m_texture->move(new_center);
            m_texture->rotate(angle);
        }

        b2Vec2 v         = b2Body_GetLinearVelocity(m_body);
        m_cached_velocity = {
            v.x * physic::meterToPixel(1.0f),
            v.y * physic::meterToPixel(1.0f)
        };
        m_needs_sync     = false;
        m_vertices_dirty = true;
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
    _initBody(world, b2_dynamicBody, m_mass, m_friction, m_bounce);
    if (b2Body_IsValid(m_body)) {
        b2Body_SetBullet(m_body, m_is_bullet);
        b2Body_SetGravityScale(m_body, m_gravity_enabled ? 1.0f : 0.0f);
    }
}


PhysicManager::PhysicManager(void) {
    b2WorldDef worldDef  = b2DefaultWorldDef();
    worldDef.gravity     = {0.0f, m_gravity};
    m_world              = b2CreateWorld(&worldDef);
}

PhysicManager::~PhysicManager(void) {
    if (b2World_IsValid(m_world)) b2DestroyWorld(m_world);
}

PhysicManager& PhysicManager::getInstance(void) {
    static PhysicManager instance;
    return instance;
}

void PhysicManager::remove(PhysicBody& body) {
    m_body_map.erase(body.m_id);

    auto it = std::find(m_bodies.begin(), m_bodies.end(), &body);
    if (it == m_bodies.end())
        rmk_dynamicAssert(rmk::PhysicError, error::physic::body_not_found);

    if (b2Body_IsValid((*it)->m_body)) {
        b2DestroyBody((*it)->m_body);
        (*it)->m_body     = b2_nullBodyId;
        (*it)->m_shape_id = b2_nullShapeId;
    }
    m_bodies.erase(it);

    auto* dyn = dynamic_cast<DynamicBody*>(&body);
    if (dyn) {
        auto it2 = std::find(m_dynamics.begin(), m_dynamics.end(), dyn);
        if (it2 != m_dynamics.end()) m_dynamics.erase(it2);
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

const std::vector<PhysicBody*>&  PhysicManager::bodies(void)   const noexcept { return m_bodies;   }
const std::vector<StaticBody*>&  PhysicManager::statics(void)  const noexcept { return m_statics;  }
const std::vector<DynamicBody*>& PhysicManager::dynamics(void) const noexcept { return m_dynamics; }

void PhysicManager::world(const Area& area) noexcept {
    if (m_world_size.x == area.x && m_world_size.y == area.y &&
        m_world_size.w == area.w && m_world_size.h == area.h) return;
	
    m_boundary_walls.clear();
    m_world_size = area;

    if (m_world_size.w > 0 && m_world_size.h > 0) {
		f32 thick  = 30.0f;
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
		
		auto createWall = [](const Vec2d& center, const Dim2d& dim) -> StaticBody {
		    Rectangle rect(center, dim);
		    StaticBody wall(rect);
		    wall.isSolid(true);
		    return wall;
		};
		
		f32 extended_height = wh + (thick * 2.0f);
		m_boundary_walls.push_back(createWall({cx, wall_top},    {ww, thick}));
		m_boundary_walls.push_back(createWall({cx, wall_bottom}, {ww, thick}));
		m_boundary_walls.push_back(createWall({wall_left,  cy},  {thick, extended_height}));
		m_boundary_walls.push_back(createWall({wall_right, cy},  {thick, extended_height}));
		

    }
}

void PhysicManager::_registerBody(PhysicBody *body) noexcept {
    body->_build(m_world);
    m_bodies.push_back(body);

    	 if (auto *s = dynamic_cast<StaticBody*>(body))  m_statics.push_back(s);
	else if (auto *d = dynamic_cast<DynamicBody*>(body)) m_dynamics.push_back(d);

    m_body_map[body->m_id] = body;
}

void PhysicManager::_unregisterBody(PhysicBody *body) noexcept {
    m_body_map.erase(body->m_id);

    auto remove = [](auto& vec, const auto *ptr) {
        vec.erase(std::remove(vec.begin(), vec.end(), ptr), vec.end());
    };

    remove(m_bodies, body);

         if (auto *s = dynamic_cast<StaticBody*>(body))  remove(m_statics, s);
    else if (auto *d = dynamic_cast<DynamicBody*>(body)) remove(m_dynamics, d);
}

bool PhysicManager::_isValidBody(PhysicBody *body) const {
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
        auto* a = reinterpret_cast<PhysicBody*>(b2Shape_GetUserData(events.beginEvents[i].shapeIdA));
        auto* b = reinterpret_cast<PhysicBody*>(b2Shape_GetUserData(events.beginEvents[i].shapeIdB));
        if (a && b && _isValidBody(a) && _isValidBody(b)) {
            a->onContactStart._evaluate(a, b);
            b->onContactStart._evaluate(b, a);
        }
    }

    for (int i = 0; i < events.hitCount; ++i) {
        auto* a = reinterpret_cast<PhysicBody*>(b2Shape_GetUserData(events.hitEvents[i].shapeIdA));
        auto* b = reinterpret_cast<PhysicBody*>(b2Shape_GetUserData(events.hitEvents[i].shapeIdB));
        if (a && b && _isValidBody(a) && _isValidBody(b)) {
            a->onContact._evaluate(a, b);
            b->onContact._evaluate(b, a);
        }
    }

    for (int i = 0; i < events.endCount; ++i) {
        auto* a = reinterpret_cast<PhysicBody*>(b2Shape_GetUserData(events.endEvents[i].shapeIdA));
        auto* b = reinterpret_cast<PhysicBody*>(b2Shape_GetUserData(events.endEvents[i].shapeIdB));
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