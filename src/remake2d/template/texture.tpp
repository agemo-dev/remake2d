#ifndef REMAKE2D_TEXTURE_TPP_
#define REMAKE2D_TEXTURE_TPP_

#include <remake2d/window.hpp>

namespace rmk {

template<IsShape S>
Texture<S>::Texture(std::string_view path, const S& shape)
    : m_shape(shape) {
    m_surface = std::make_shared<_Surface>();
    m_surface->data = IMG_Load(std::string(path).c_str());
    if (!m_surface->data) rmk_dynamicAssert(rmk::TextureError, (std::string(error::texture::surface_no_load) + " : " + IMG_GetError()));
    m_shape.m_is_changed = true;
    m_real_size = { (f32)m_surface->data->w, (f32)m_surface->data->h };
    for(auto& e : xwindow.m_windows) {
        TextureData td;
        td.texture = SDL_CreateTextureFromSurface(e->m_renderer, m_surface->data);
        if (!td.texture) rmk_dynamicAssert(rmk::TextureError, (std::string(error::texture::texture_no_load) + " : " + SDL_GetError()));
        m_textures[e->m_renderer] = td;
    }
    m_use_clip = false;
	m_shape.m_is_changed = true;
	_calculateVertices();
}

template<IsShape S>
Texture<S>::Texture(const Texture<S>& other) {
    this->_copy(other);
}

template<IsShape S>
Texture<S>& Texture<S>::operator=(const Texture<S>& other) {
    if (this != &other) this->_copy(other);
    return *this;
}

template<IsShape S>
void Texture<S>::move(const Vec2d& center) noexcept { 
	m_shape.move(center);
	_calculateVertices();
}

template<IsShape S>
void Texture<S>::rotate(f32 angle) noexcept {
	m_shape.rotate(angle);
	_calculateVertices();
}

template<IsShape S>
void Texture<S>::scale(const Fact2d& scaling) noexcept {
	m_shape.scale(scaling);
	_calculateVertices();
}

template<IsShape S>
void Texture<S>::resize(const Dim2d& size) noexcept {
	m_shape.resize(size);
	_calculateVertices();
}

template<IsShape S>
void Texture<S>::transform(const Vec2d& center, f32 angle, const Fact2d& scaling) noexcept {
	m_shape.transform(center, angle, scaling);
	_calculateVertices();
}


template<IsShape S>
const Geometry& Texture<S>::shape(void) const noexcept { 
	return m_shape;
}

template<IsShape S>
std::vector<SDL_Vertex> Texture<S>::vertices(void) const noexcept {
    return m_vertices;
}

template<IsShape S>
Vec2d Texture<S>::center(void) const noexcept { 
	return m_shape.center();
}

template<IsShape S>
Dim2d Texture<S>::size(void)   const noexcept { 
	return m_shape.size();
}

template<IsShape S>
Dim2d Texture<S>::realSize(void) const noexcept { 
	return m_real_size;
}

template<IsShape S>
void Texture<S>::_applyColor(SDL_Renderer* renderer, const Color& color) const noexcept {
    auto it = m_textures.find(renderer);
    if (it == m_textures.end()) return;
    if (it->second.current_color == color) return;
    SDL_SetTextureColorMod(it->second.texture, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(it->second.texture, color.a);
    it->second.current_color = color;
}

template<IsShape S>
void Texture<S>::clip(const Vec2d& clip_pos, const Dim2d& clip_size) noexcept {
    m_clip_pos = clip_pos;
    m_clip_size = clip_size;
    
    if (clip_size.w > 0 && clip_size.h > 0) {
        m_srcrect = {(int)clip_pos.x, (int)clip_pos.y, (int)clip_size.w, (int)clip_size.h};
        m_use_clip = true;
    } else {
        m_use_clip = false;
    }

	m_shape.m_is_changed = true;
    _calculateVertices();
}

template<IsShape S>
void Texture<S>::unclip(void) noexcept {
    m_clip_pos = {0, 0};
    m_clip_size = {0, 0};
    m_use_clip = false;

	m_shape.m_is_changed = true;
    _calculateVertices();
}

template<IsShape S>
bool Texture<S>::hasIntersected(const Geometry& other) const noexcept {
    return m_shape.hasIntersected(other);
}

template<IsShape S>
void Texture<S>::_copy(const Texture<S>& other) noexcept {
    if (this == &other) return;
    this->m_surface    = other.m_surface;
    this->m_shape      = other.m_shape;
    this->m_clip_pos   = other.m_clip_pos;
    this->m_clip_size  = other.m_clip_size;
    this->m_real_size  = other.m_real_size;
    this->m_use_clip   = other.m_use_clip;
    this->m_srcrect    = other.m_srcrect;
    for (auto& [renderer, data] : other.m_textures) {
        TextureData td;
        td.texture = SDL_CreateTextureFromSurface(renderer, this->m_surface->data);
        td.current_color = rmk::color::white;
        this->m_textures[renderer] = td;
    }
}

template<IsShape S>
void Texture<S>::_calculateVertices(void) noexcept {
    if (!m_shape.m_is_changed) return;
    m_shape._triangulate();

    auto verts = m_shape._toVertices();

    const auto* pts = m_shape.pointsPos();
    f32 minX = pts[0].x, maxX = pts[0].x;
    f32 minY = pts[0].y, maxY = pts[0].y;

    for (size_t i = 1; i <= m_shape.m_n; i++) {
        minX = std::min(minX, pts[i].x);
        maxX = std::max(maxX, pts[i].x);
        minY = std::min(minY, pts[i].y);
        maxY = std::max(maxY, pts[i].y);
    }

    f32 w = maxX - minX;
    f32 h = maxY - minY;
	
    if (w == 0.0f || h == 0.0f) {
		m_vertices.clear();
		return;
	}

    f32 uMin = 0.0f, uRange = m_real_size.w;
    f32 vMin = 0.0f, vRange = m_real_size.h;

    if (m_use_clip) {
        uMin   = m_clip_pos.x;
        vMin   = m_clip_pos.y;
        uRange = m_clip_size.w;
        vRange = m_clip_size.h;
    }

    for (auto& v : verts) {
        f32 u = (v.position.x - minX) / w;
        f32 t = (v.position.y - minY) / h;
        v.tex_coord.x = (uMin + u * uRange) / m_real_size.w;
        v.tex_coord.y = (vMin + t * vRange) / m_real_size.h;
    }

    m_vertices = std::move(verts);
}


template<IsShape S>
SDL_Texture* Texture<S>::_ownerTexture(SDL_Renderer* renderer) const noexcept {
    auto it = m_textures.find(renderer);
    if (it != m_textures.end()) return it->second.texture;
	return nullptr;
}


template<IsShape S>
Texture<S>::~Texture(void) {
    for (auto& [renderer, data] : m_textures) {
        if (data.texture) SDL_DestroyTexture(data.texture);
    }
    m_textures.clear();
}

template<IsShape S>
const SDL_Rect* Texture<S>::getClipRect(void) const noexcept {
    return m_use_clip ? &m_srcrect : nullptr;
}

} //namespace rmk
#endif