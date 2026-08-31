#ifndef REMAKE2D_PARALLAX_
#define REMAKE2D_PARALLAX_

#include <remake2d/draw.hpp>
#include <remake2d/tracker.hpp>
#include <remake2d/numeric.hpp>
#include <remake2d/private/nil.hpp>
#include <remake2d/private/update.hpp>
#include <remake2d/config/forward.hpp>
#include <vector>


namespace rmk {


class Parallax : public Fillable, public Trackable<Parallax> {
private:
    struct Layer {
        Sprite sprite_a;
        Sprite sprite_b;
        f32    speed;
    };

private:
    mutable std::vector<Layer>  m_layers;
    std::vector<u8>             m_speed_quotients;
    Vec2d                       m_velocity{0.0f, 0.0f};
    Dim2d                       m_size{0.0f};
    Vec2d                       m_center{0.0f};
    u32                         m_parse{0};
    Tracker<Camera>             m_sync_cam{nil};
    std::vector<Sprite>         m_sprite_list;

public:
    Parallax(void)                       = delete;
    Parallax(Parallax&&)                 = default;
    Parallax(const Parallax&)            = default;
    Parallax& operator=(Parallax&&)      = default;
    Parallax& operator=(const Parallax&) = default;

    Parallax(const Vec2d&, const Dim2d&, const std::vector<Sprite>&, const std::vector<u8>&);

public:
    void update(void)              noexcept;
    void move(const Vec2d&)        noexcept;
    void resize(const Dim2d&)      noexcept;
    void velocity(const Vec2d&)    noexcept;
    void linkCamera(const Camera&) noexcept;

    Dim2d size(void)     const noexcept;
    Vec2d center(void)   const noexcept;
    Vec2d velocity(void) const noexcept;

private:
    void _tile(Layer&)                        const noexcept;
    void _moveAndResize(const Vec2d&, const Dim2d&) noexcept;

private:
    void fill(const Fillable&) const noexcept override;

public:
    ~Parallax(void);

private:
    friend class Window;
};

class ParallaxManager : public Updatable {
private:
    std::vector<Tracker<Parallax>> m_parallaxs;

private:
    ParallaxManager(void)                                = default;
    ParallaxManager(const ParallaxManager&)              = delete;
    ParallaxManager& operator=(const ParallaxManager&)   = delete;

private:
    void _registerParallax(const Parallax*)   noexcept;
    void _unregisterParallax(const Parallax*) noexcept;

public:
    void update(void) override;

public:
    static ParallaxManager& getInstance(void) noexcept;

public:
    ~ParallaxManager(void) = default;

private:
    friend class Parallax;
};

inline ParallaxManager& parallax = ParallaxManager::getInstance();

} // namespace rmk
#endif
