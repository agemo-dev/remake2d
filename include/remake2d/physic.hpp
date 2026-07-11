#ifndef REMAKE2D_PHYSIC_
#define REMAKE2D_PHYSIC_

#include <remake2d/shape.hpp>
#include <remake2d/error.hpp>
#include <remake2d/signal.hpp>
#include <remake2d/numeric.hpp>
#include <remake2d/texture.hpp>
#include <remake2d/concept.hpp>
#include <remake2d/config/forward.hpp>
#include <remake2d/box2d/include/box2d/box2d.h>

#include <map>
#include <string>
#include <vector>
#include <utility>
#include <functional>

namespace rmk {
	
class PhysicBody {
public:
    using AnimEntry = std::pair<std::string, Animation>;
    using AnimMap   = std::map<std::string, Animation>;

protected:
    struct ShapeCache {
        std::vector<Vec2d> points;
        Vec2d              center;
        bool               is_circle{false};
        f32                radius{0.0f};
    };

protected:
    b2BodyId                m_body{b2_nullBodyId};
    b2ShapeId               m_shape_id{b2_nullShapeId};
    ShapeCache              m_shape_cache;
    std::vector<SDL_Vertex> m_cached_vertices;
    std::vector<SDL_FPoint> m_cached_contour;
    bool                    m_vertices_dirty{true};
    Sprite*                 m_texture{nullptr};
    AnimMap                 m_animations;
    std::string             m_tag;
    bool                    m_solid{true};
    u64                     m_id{0};
    Vec2d                   m_cached_velocity{0.0f, 0.0f};
    bool                    m_needs_sync{true};

public:
    _PhysicSignal<PhysicBody*, PhysicBody*> onContact;
    _PhysicSignal<PhysicBody*, PhysicBody*> onContactEnd;
    _PhysicSignal<PhysicBody*, PhysicBody*> onContactStart;

public:
    PhysicBody(const Geometry&);
    PhysicBody(const PhysicBody&)            = default;
    PhysicBody& operator=(const PhysicBody&) = default;
    PhysicBody(void)                         = default;
    PhysicBody(PhysicBody&&)                 = default;
    PhysicBody& operator=(PhysicBody&&)      = default;

public:
    void isSolid(bool)          noexcept;
    void link(Sprite&)          noexcept;
    void tag(std::string_view)  noexcept;
    void move(const Vec2d&)     noexcept;
    void rotate(f32)            noexcept;
    void scale(const Fact2d&)   noexcept;
    void resize(const Dim2d&)   noexcept;

    bool        isSolid(void)   const noexcept;
    u64         ID(void)        const noexcept;
    Vec2d       center(void)    const noexcept;
    Dim2d       size(void)      const noexcept;
    std::string tag(void)       const noexcept;

public:
    void       linkAnimation(AnimEntry);
    Animation& animation(std::string_view);

protected:
    void         _sync(void);
    static u64   _nextId(void);
    virtual void _build(b2WorldId);
    void         _calculateVertices(void) noexcept;
    void         _initBody(b2WorldId, b2BodyType, f32, f32, f32);

protected:
	b2Circle  _makeB2Circle(const PhysicBody::ShapeCache&);
	b2Polygon _makeB2Polygon(const PhysicBody::ShapeCache&);

public:
    virtual ~PhysicBody(void);

private:
    friend class Window;
    friend class Camera;
    friend class PhysicManager;
};


class StaticBody : public PhysicBody {
public:
    StaticBody(const StaticBody&);
    explicit StaticBody(const Geometry&);
    StaticBody& operator=(const StaticBody&);

public:
    StaticBody(void)                         = default;
    StaticBody(StaticBody&&)                 = default;
    StaticBody& operator=(StaticBody&&)      = default;
    virtual ~StaticBody(void) override       = default;

protected:
    using PhysicBody::_build;

private:
    friend class Window;
    friend class PhysicManager;
};


class DynamicBody : public PhysicBody {
protected:
    f32  m_mass{1.0f};
    f32  m_bounce{0.0f};
    f32  m_bounce_threshold{0.5f};
    bool m_infinite_bounce{false};
    bool m_is_bullet{false};
    bool m_gravity_enabled{true};
    f32  m_friction{0.0f};
    Area m_warp_area{0, 0, 0, 0};
    Area m_personal_limit{0, 0, 0, 0};

public:
    _PhysicSignal<DynamicBody*> onMove;
    _PhysicSignal<DynamicBody*> onMoveUp;
    _PhysicSignal<DynamicBody*> onMoveDown;
    _PhysicSignal<DynamicBody*> onMoveLeft;
    _PhysicSignal<DynamicBody*> onMoveRight;

public:
    explicit DynamicBody(const Geometry&);
    DynamicBody(const DynamicBody&);
    DynamicBody& operator=(const DynamicBody&);

public:
    DynamicBody(void)                        = default;
    DynamicBody(DynamicBody&&)               = default;
    DynamicBody& operator=(DynamicBody&&)    = default;
    virtual ~DynamicBody(void) override      = default;

public:
    void  mass(f32)              noexcept;
    void  bounce(f32)            noexcept;
    void  bounceThreshold(f32)   noexcept;
    void  infiniteBounce(bool)   noexcept;
    void  friction(f32)          noexcept;
    void  gravity(bool)          noexcept;
    void  isBullet(bool)         noexcept;
    void  warp(const Area&)      noexcept;
    void  limit(const Area&)     noexcept;

    void  jump(f32)              noexcept;
    void  move(const Vec2d&)     noexcept;
    void  velocity(const Vec2d&) noexcept;
    Vec2d velocity(void)   const noexcept;

protected:
    void _applyWarp(void);
    void _applyLimit(void);
    void _syncAndUpdate(void);
    void _build(b2WorldId) override;

private:
    friend class Window;
    friend class PhysicManager;
};


class PhysicManager {
private:
    b2WorldId                            m_world{b2_nullWorldId};
    std::vector<PhysicBody*>             m_bodies;
    std::vector<StaticBody*>             m_statics;
    std::vector<DynamicBody*>            m_dynamics;
    std::unordered_map<u64, PhysicBody*> m_body_map;
    f32                                  m_gravity{9.8f};
    f32                                  m_ptm{32.0f};
    Area                                 m_world_size{0, 0, 0, 0};
    std::vector<StaticBody>              m_boundary_walls;

public:
    static PhysicManager& getInstance(void);

public:
    void remove(PhysicBody&);
    void world(const Area&)          noexcept;
    void gravitationalConstant(f32)  noexcept;
    void pixelsPerMeter(f32)         noexcept;

    Area world(void)                 const noexcept;
    f32  pixelsPerMeter(void)        const noexcept;
    f32  gravitationalConstant(void) const noexcept;

    const std::vector<PhysicBody*>&  bodies(void)   const noexcept;
    const std::vector<StaticBody*>&  statics(void)  const noexcept;
    const std::vector<DynamicBody*>& dynamics(void) const noexcept;

    void update(void);

private:
    PhysicManager(void);
    ~PhysicManager(void);
    PhysicManager(const PhysicManager&)            = delete;
    PhysicManager& operator=(const PhysicManager&) = delete;

    void _rebuildBoundary(void);
    bool _isValidBody(PhysicBody*)    const;
    void _registerBody(PhysicBody*)   noexcept;
    void _unregisterBody(PhysicBody*) noexcept;

private:
	friend class PhysicBody;
    friend class StaticBody;
    friend class DynamicBody;
};

inline PhysicManager& physics = PhysicManager::getInstance();

} // namespace rmk

#endif