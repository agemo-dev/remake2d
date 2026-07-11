#include <remake2d/parallax.hpp>
#include <remake2d/window.hpp>
#include <remake2d/time.hpp>

#include <vector>
#include <utility>
#include <algorithm>

namespace rmk {

Parallax::Parallax(const Vec2d& center, const Dim2d& size, const SpriteList& sprites, const QuotientList& quotients) {

    m_size = size;
    m_center = center;
    
    m_speed_quotients = std::vector<u8>(quotients.begin(), quotients.end());
    u32 sprite_count  = sprites.size();

    if (m_speed_quotients.size() > sprite_count && sprite_count > 1) {
        u32 n = sprite_count;
        std::vector<u8> temp;
        temp.reserve(n);
        for (u32 i = 0; i < n; i++) {
            u32 idx = i * (m_speed_quotients.size() - 1) / (n - 1);
            temp.emplace_back(m_speed_quotients[idx]);
        }
        m_speed_quotients = std::move(temp);
    }
	
	if (m_speed_quotients.empty()) m_speed_quotients.push_back(0);
    m_parse = sprite_count / m_speed_quotients.size();
    m_sprite_list = sprites;
    
    _moveAndResize(center, size);
}

void Parallax::velocity(const Vec2d& v) noexcept {
    m_velocity = v;
}

Vec2d Parallax::velocity(void) const noexcept {
    return m_velocity;
}

void Parallax::move(const Vec2d& center) noexcept {
    _moveAndResize(center, m_size);
}

void Parallax::resize(const Dim2d& size) noexcept {
    _moveAndResize(m_center, size);
}

Vec2d Parallax::center(void) const noexcept {
    return m_center;
}

Dim2d Parallax::size(void) const noexcept {
    return m_size;
}

void Parallax::_tile(Layer& layer) const noexcept {
    Dim2d size = layer.sprite_a.size();

    
    if (layer.sprite_a.center().x < -size.w) {
        layer.sprite_a.move({
            layer.sprite_b.center().x + size.w,
            layer.sprite_a.center().y
        });
    }

    
    if (layer.sprite_b.center().x < -size.w) {
        layer.sprite_b.move({
            layer.sprite_a.center().x + size.w,
            layer.sprite_b.center().y
        });
    }
}

void Parallax::_draw(Window& win, Color color) const noexcept {

    Vec2d vel = m_velocity;

    for (auto& layer : m_layers) {
        
        Vec2d delta_ = {
            vel.x * layer.speed * (f32)delta.tick(),
            vel.y * layer.speed * (f32)delta.tick()
        };

        layer.sprite_a.move({
            layer.sprite_a.center().x + delta_.x,
            layer.sprite_a.center().y + delta_.y
        });
        layer.sprite_b.move({
            layer.sprite_b.center().x + delta_.x,
            layer.sprite_b.center().y + delta_.y
        });

        
        _tile(layer);

        
        win.draw(layer.sprite_a, color);
        win.draw(layer.sprite_b, color);
    }
}

void Parallax::_moveAndResize(const Vec2d& center, const Dim2d& size) noexcept {
    m_center = center;
    m_size = size;
    m_layers.clear();

    u32 group = 0;
    u32 count = 0;
    for (auto& sprite : m_sprite_list) {
        if (count > 0 && count % m_parse == 0) group++;

        f32 speed = 1.0f;
        for (u32 i = 0; i <= group && i < m_speed_quotients.size(); i++)
            speed *= (1.0f - m_speed_quotients[i] / 100.0f);

        Sprite a = sprite;
        a.move(center);
        a.resize(size);

        Sprite b = a;
        b.move({center.x + (f32)size.w, center.y});

        m_layers.emplace_back(Layer{std::move(a), std::move(b), speed});
        count++;
    }
}

} //namespace rmk