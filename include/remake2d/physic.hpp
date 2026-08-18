#ifndef REMAKE2D_PHYSIC_
#define REMAKE2D_PHYSIC_

#include <remake2d/body.hpp>

namespace rmk {

class PhysicManager {
private:
    b2WorldId                                    m_world{b2_nullWorldId};
    std::vector<Tracker<PhysicBody>>             m_bodies;
    std::vector<Tracker<PhysicBody>>             m_statics;
    std::vector<Tracker<PhysicBody>>             m_dynamics;
    std::unordered_map<u64, Tracker<PhysicBody>> m_body_map;
    f32                                          m_gravity{9.8f};
    f32                                          m_ptm{32.0f};
    Area                                         m_world_size{0, 0, 0, 0};
    bool                                         m_use_fixed_step{false};
    f32                                          m_fixed_step{1.0f / 60.0f};
    fmax                                         m_accumulator{0.0};
    std::vector<std::unique_ptr<StaticBody>>     m_boundary_walls;

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

    void useFixedStep(bool)                noexcept;
    bool useFixedStep(void)          const noexcept;

    void fixedStep(f32)                    noexcept;
    f32  fixedStep(void)             const noexcept;

    std::vector<Tracker<PhysicBody>>  bodies(void)    noexcept;
    std::vector<Tracker<PhysicBody>>  statics(void)   noexcept;
    std::vector<Tracker<PhysicBody>>  dynamics(void)  noexcept;

    void update(void);

private:
    PhysicManager(void);
    PhysicManager(const PhysicManager&)            = delete;
    PhysicManager& operator=(const PhysicManager&) = delete;

    void _rebuildBoundary(void);
    bool _isValidBody(PhysicBody*)    const;
    void _registerBody(PhysicBody*)   noexcept;
    void _unregisterBody(PhysicBody*) noexcept;

    void _stepAndDispatch(f32, i32) noexcept;

    static PhysicBody* _ownerOf(void*) noexcept;

public:
    ~PhysicManager(void);

private:
	friend class PhysicBody;
    friend class StaticBody;
    friend class DynamicBody;
};

inline PhysicManager& physics = PhysicManager::getInstance();

} // namespace rmk

#endif