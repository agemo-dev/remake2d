#ifndef REMAKE2D_PARALLAX_
#define REMAKE2D_PARALLAX_

#include <remake2d/numeric.hpp>
#include <remake2d/texture.hpp>
#include <remake2d/vector.hpp>
#include <remake2d/camera.hpp>
#include <vector>


namespace rmk {


class Parallax {

public:
    using SpriteList   = std::span<Sprite>;
    using QuotientList = std::span<u8>;

private:
    struct Layer {
        Sprite sprite_a;
        Sprite sprite_b;
        f32    speed;
    };

private:
    mutable std::vector<Layer>  m_layers;
    std::vector<u8>     		m_speed_quotients;
    Vec2d               		m_velocity{0.0f, 0.0f};
    Dim2d               		m_size;
    Vec2d               		m_center;
    SpriteList          		m_sprite_list;
    u32                 		m_parse{0};

public:
    Parallax(void)                       = delete;
    Parallax(const Parallax&)            = default;
    Parallax& operator=(const Parallax&) = default;
    Parallax(Parallax&&)                 = default;
    Parallax& operator=(Parallax&&)      = default;

    Parallax(const Vec2d&, const Dim2d&, const SpriteList&, const QuotientList&);

public:
    void move(const Vec2d&)     noexcept;
    void resize(const Dim2d&)   noexcept;
    void velocity(const Vec2d&) noexcept;

    Dim2d size(void)     const noexcept;
    Vec2d center(void)   const noexcept;
    Vec2d velocity(void) const noexcept;

private:
    void _tile(Layer&) 						  const noexcept;
    void _draw(class Window&, Color) 		  const noexcept;
    void _moveAndResize(const Vec2d&, const Dim2d&) noexcept;

private:
    friend class Window;
};

} // namespace rmk
#endif