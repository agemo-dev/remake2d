#ifndef REMAKE2D_BODY_
#define REMAKE2D_BODY_

#include <remake2d/shape.hpp>
#include <remake2d/error.hpp>
#include <remake2d/signal.hpp>
#include <remake2d/tracker.hpp>
#include <remake2d/texture.hpp>
#include <remake2d/numeric.hpp>
#include <remake2d/concept.hpp>
#include <remake2d/config/forward.hpp>
#include <remake2d/box2d/include/box2d/box2d.h>

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <functional>
#include <unordered_map>

namespace rmk {

class PhysicBody : public Trackable<PhysicBody> {
public:
    using AnimMap   = std::map<std::string,  Animation>;

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
    AnimMap                 m_animations;
    std::string             m_focused_anim{""};
    std::string             m_tag;
    bool                    m_solid{true};
    bool                    m_needs_sync{true};
    u64                     m_id{0};
    Vec2d                   m_cached_velocity{0};
    f32                     m_current_angle{0.0f};

public:
    _PhysicSignal<PhysicBody*, PhysicBody*> onContact;
    _PhysicSignal<PhysicBody*, PhysicBody*> onContactEnd;
    _PhysicSignal<PhysicBody*, PhysicBody*> onContactStart;

public:
    PhysicBody(const Geometry&);
    PhysicBody(const PhysicBody&);
    PhysicBody& operator=(const PhysicBody&);
    PhysicBody(void)                         = default;
    PhysicBody(PhysicBody&&)                 noexcept;
    PhysicBody& operator=(PhysicBody&&)      noexcept;

public:
    void isSolid(bool)          noexcept;
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
	void             focusAnimation(std::string_view);
    Animation&       animation(std::string_view = "");
    const Animation& animation(std::string_view = "") const;
    void             linkAnimation(std::string_view, const Animation&);

protected:
    void         _sync(void);
    static u64   _nextId(void);
    virtual void _build(b2WorldId);
    void         _rebuildShape(void);
    void         _calculateVertices(void) noexcept;
    void         _initBody(b2WorldId, b2BodyType, f32, f32, f32);
    void         _detachFromWorld(void) noexcept;

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
    StaticBody(StaticBody&&)                 noexcept;
    StaticBody& operator=(StaticBody&&)      noexcept;

public:
    virtual ~StaticBody(void) override       = default;

protected:
    using PhysicBody::_build;

private:
    friend class Window;
    friend class TileMap;
    friend class PhysicManager;
};


class DynamicBody : public PhysicBody {
protected:
    f32  m_mass{1.0f};
    f32  m_density{1.0f};
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
    DynamicBody(DynamicBody&&)               noexcept;
    DynamicBody& operator=(DynamicBody&&)    noexcept;

public:
    virtual ~DynamicBody(void) override      = default;

public:
    void  mass(f32)              noexcept;
    void  density(f32)           noexcept;
    void  bounce(f32)            noexcept;
    void  bounceThreshold(f32)   noexcept;
    void  infiniteBounce(bool)   noexcept;
    void  friction(f32)          noexcept;
    void  gravity(bool)          noexcept;
    void  isBullet(bool)         noexcept;
    void  warp(const Area&)      noexcept;
    void  limit(const Area&)     noexcept;

public:
    f32   mass(void)             const noexcept;
    f32   density(void)          const noexcept;
    f32   bounce(void)           const noexcept;
    f32   bounceThreshold(void)  const noexcept;
    bool  infiniteBounce(void)   const noexcept;
    f32   friction(void)         const noexcept;
    bool  gravity(void)          const noexcept;
    bool  isBullet(void)         const noexcept;
    Area  warp(void)             const noexcept;
    Area  limit(void)            const noexcept;

public:
    void  jump(f32)              noexcept;
    void  push(const Vec2d&)     noexcept;
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

} // namespace rmk

#endif