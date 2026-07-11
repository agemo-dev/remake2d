#include <remake2d/config/concept.hpp>
#include <remake2d/utility.hpp>
#include <remake2d/texture.hpp>
#include <remake2d/window.hpp>
#include <remake2d/color.hpp>
#include <remake2d/time.hpp>

#include <memory>
#include <string>
#include <utility>
#include <algorithm>
#include <filesystem>

namespace rmk {

bool TextureBase::hasIntersected(const TextureBase& other) const noexcept {
    return hasIntersected(other.shape());
}

Sprite::Sprite(std::string_view path, const Rectangle& shape)
    : Texture<Rectangle>(path, shape) {}

GlyphAtlas::~GlyphAtlas(void) {
	for (auto& [renderer, data] : textures) {
		if (data.texture) SDL_DestroyTexture(data.texture);
	}
}

void FontManager::load(std::string_view tag, std::string_view path, u8 size) {
    std::string key(tag);
    if (m_fonts.count(key)) return;

    TTF_Font* f = TTF_OpenFont(std::string(path).c_str(), size);
    if (!f) {
        rmk_dynamicAssert(rmk::TextureError, (std::string(error::texture::font_no_load) + " : " + TTF_GetError()));
    }

    FontEntry entry;
    entry.font = f;
    m_fonts.emplace(key, std::move(entry));

    for (auto& win : xwindow.m_windows) {
        _buildAtlas(m_fonts.at(key), win->m_renderer);
    }
}

void FontManager::_buildAtlas(FontEntry& entry, SDL_Renderer* renderer) {
    const int firstChar = 32;
    const int lastChar  = 126;
    const int numChars  = lastChar - firstChar + 1;

    TTF_Font* font = entry.font;

    int maxWidth = 0;
    std::vector<SDL_Surface*> surfaces;
    surfaces.reserve(numChars);

    for (int c = firstChar; c <= lastChar; ++c) {
        char ch = static_cast<char>(c);
        SDL_Surface* surf = TTF_RenderGlyph_Blended(font, ch, color::white._data());
        if (!surf) continue;
        surfaces.push_back(surf);
        maxWidth = std::max(maxWidth, surf->w);
    }

    int atlasHeight = 0;
    for (auto* surf : surfaces) {
        atlasHeight = std::max(atlasHeight, surf->h);
    }

    int atlasWidth = maxWidth * numChars;

    SDL_Texture* atlasTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, atlasWidth, atlasHeight);
	
    if (!atlasTex) {
        for (auto* s : surfaces) SDL_FreeSurface(s);
        return;
    }
	
    SDL_SetTextureBlendMode(atlasTex, SDL_BLENDMODE_BLEND);
    SDL_Texture* oldTarget = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, atlasTex);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    int xOffset = 0;
    for (size_t i = 0; i < surfaces.size(); ++i) {
        SDL_Surface* surf = surfaces[i];
        SDL_Texture* tempTex = SDL_CreateTextureFromSurface(renderer, surf);
        if (tempTex) {
            SDL_Rect dest = {xOffset, 0, surf->w, surf->h};
            SDL_RenderCopy(renderer, tempTex, nullptr, &dest);
            SDL_DestroyTexture(tempTex);
        }
        char ch = static_cast<char>(firstChar + i);
        entry.atlas.glyphs[ch] = {xOffset, 0, surf->w, surf->h};
        xOffset += surf->w;
        SDL_FreeSurface(surf);
    }

    SDL_SetRenderTarget(renderer, oldTarget);

    GlyphAtlas::AtlasData ad;
    ad.texture = atlasTex;
    entry.atlas.textures[renderer] = ad;
    entry.atlas.glyph_height = atlasHeight;
    entry.atlas.baseline = TTF_FontAscent(font);
}

const GlyphAtlas* FontManager::atlas(std::string_view tag) const noexcept {
    auto it = m_fonts.find(std::string(tag));
    if (it != m_fonts.end()) return &it->second.atlas;
	
    return nullptr;
}

FontManager& FontManager::getInstance(void) {
    static FontManager instance;
    return instance;
}

FontManager::~FontManager(void) {
    for (auto& pair : m_fonts) {
        TTF_CloseFont(pair.second.font);
    }
    m_fonts.clear();
}


Text::Text(std::string_view font_name, Vec2d pos)
    : Texture<Rectangle>(Rectangle(pos, {0, 0})),
      m_font_name(font_name)
{
    m_anchor_pos = pos;
    auto it = font.m_fonts.find(m_font_name);
    if (it == font.m_fonts.end()) {
        rmk_dynamicAssert(rmk::TextureError, error::texture::font_unexist);
    }
    m_font = it->second.font;
}

void Text::anchorX(anchor::x axis) noexcept { m_anchor_x = axis; }
void Text::anchorY(anchor::y axis) noexcept { m_anchor_y = axis; }

std::string Text::text(void) const noexcept {
    return m_current_text;
}

void Text::write(std::string_view text) {
    std::string txt(std::string(text) == "" ? " " : text);
    if(txt == m_current_text) return;

    m_current_text  = txt;

    m_surface = std::make_shared<_Surface>();
    m_surface->data = TTF_RenderUTF8_Blended_Wrapped(m_font, txt.c_str(), color::white._data(), (int)m_max_lengh);
    if (!m_surface->data) {
        rmk_dynamicAssert(rmk::TextureError, (std::string(error::texture::texture_no_load) + " : " + TTF_GetError()));
    }

    float x = m_anchor_pos.x;
    float y = m_anchor_pos.y;
	
    switch (m_anchor_x) {
        case anchor::x::left:   x += m_surface->data->w / 2.0f; break;
        case anchor::x::center: break;
        case anchor::x::right:  x -= m_surface->data->w / 2.0f; break;
    }
    switch (m_anchor_y) {
        case anchor::y::top:    y += m_surface->data->h / 2.0f; break;
        case anchor::y::middle: break;
        case anchor::y::bottom: y -= m_surface->data->h / 2.0f; break;
    }

    m_shape.move({x, y});
    m_shape.resize({(f32)m_surface->data->w, (f32)m_surface->data->h});

    for (auto& [renderer, data] : m_textures) {
        if (data.texture) SDL_DestroyTexture(data.texture);
    }
    m_textures.clear();

    for (auto& win : xwindow.m_windows) {
        SDL_Texture* tex = SDL_CreateTextureFromSurface(win->m_renderer, m_surface->data);
        if (tex) {
            TextureData td;
            td.texture = tex;
            m_textures[win->m_renderer] = td;
        }
    }
    m_use_clip = false;
	m_erase    = false;
}

void Text::append(fmt txt) {
	switch (txt) {

		case fmt::nl :
			append("\n");
			break;
		
		case fmt::tab :
			append("    ");
			break;
			
		case fmt::endl :
			m_erase = true;
			break;

		case fmt::flush :
			clear();
			break;
		 
	}
}

void Text::append(std::string_view text) {
    if (!m_erase) write(m_current_text + std::string(text));
	else 		  write(text);
}

void Text::clear(void) {
    write("");
}

void Text::maxLengh(u16 l) noexcept {
	m_max_lengh = l;
}

u16 Text::maxLengh(void) const noexcept {
	return m_max_lengh;
}

Text::Text(const Text& other) : Texture<Rectangle>(other) {
    this->_textCopy(other);
}

Text& Text::operator=(const Text& other) {
    if (this != &other) {
        Texture<Rectangle>::operator=(other);
        this->_textCopy(other);
    }
    return *this;
}

void Text::_textCopy(const Text& other) {
    m_font_name		= other.m_font_name;
    m_current_text	= other.m_current_text;
    m_anchor_pos	= other.m_anchor_pos;
    m_anchor_x		= other.m_anchor_x;
    m_anchor_y		= other.m_anchor_y;
    m_font			= other.m_font;
}

void Text::_updateWidth(const GlyphAtlas& atlas) noexcept {
    (void)atlas;
}


Animation::Animation(std::string_view path, const Rectangle& shape, u8 total_clips, Dim2d clip_size, Vec2d start_pos, u8 spacing)
    : Sprite(path, shape) {
    this->m_clip_size = clip_size;
    this->m_start_pos = start_pos;
    this->m_total_clips = total_clips;
    this->m_spacing = spacing;
}

void Animation::play(i8 loop, u8 fps) {
    if (!m_is_playing) {
        m_timer				= 0.0;
        m_is_playing		= true;
        m_is_paused			= false;
        m_clip_duration		= 1.0 / fps;
        m_loops_remaining	= (loop < 0) ? -2 : loop;
    }

    if (m_is_paused) return;

    m_timer += delta.tick();

    if (m_timer >= m_clip_duration) {
        m_timer 	 = 0.0;
        i8 next 	 = m_current_clip + 1;
        i8 remaining = m_loops_remaining;

        if (next >= m_total_clips) {
            next = (remaining == -2 || remaining > 0) ? 0 : m_total_clips - 1;
            if (remaining > 0) m_loops_remaining--;
            else if (remaining == 0) {
                m_is_playing = false;
                return;
            }
        }
        m_current_clip = next;
    }

    f32 x = m_start_pos.x + m_current_clip * (m_clip_size.w + m_spacing);
    clip({x, m_start_pos.y}, m_clip_size);
}

void Animation::pause(void) noexcept {
    m_is_paused = true;
}

void Animation::resume(void) noexcept {
    m_is_paused = false;
}

void Animation::stop(void) noexcept {
    m_is_playing   = false;
    m_is_paused    = false;
    m_current_clip = 0;
    m_timer 	   = 0.0;
    clip(m_start_pos, m_clip_size);
}

Animation::Animation(const Animation& other) : Sprite(other) {
    this->_animationCopy(other);
}

Animation& Animation::operator=(const Animation& other) {
    if (this != &other) {
        Sprite::operator=(other);
        this->_animationCopy(other);
    }
    return *this;
}

void Animation::_animationCopy(const Animation& other) noexcept {
    m_spacing 			= other.m_spacing;
    m_total_clips 		= other.m_total_clips;
    m_clip_size 		= other.m_clip_size;
    m_start_pos 		= other.m_start_pos;
    m_current_clip 		= other.m_current_clip;
    m_loops_remaining	= other.m_loops_remaining;
    m_is_playing 		= other.m_is_playing;
    m_is_paused 		= other.m_is_paused;
    m_timer 			= other.m_timer;
    m_clip_duration 	= other.m_clip_duration;
}

} // namespace rmk