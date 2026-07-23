#ifndef REMAKE2D_TEXTURE_
#define REMAKE2D_TEXTURE_

#include <remake2d/shape.hpp>
#include <remake2d/error.hpp>
#include <remake2d/color.hpp>
#include <remake2d/system.hpp>
#include <remake2d/concept.hpp>
#include <remake2d/numeric.hpp>
#include <remake2d/config/resource.hpp>

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <filesystem>

#if __has_include(<SDL2/SDL.h>)
    #include <SDL2/SDL.h>
	#include <SDL2/SDL_ttf.h>
	#include <SDL2/SDL_image.h>
#elif __has_include(<SDL.h>)
    #include <SDL.h>
	#include <SDL_ttf.h>
	#include <SDL_image.h>
#else
    #error "SDL not found."
#endif


namespace rmk {

class Window;


class TextureBase {
public:
    TextureBase(void)							= default;
    TextureBase(TextureBase&&)					= default;
    TextureBase(const TextureBase&)				= default;
    TextureBase& operator=(TextureBase&&)		= default;
    TextureBase& operator=(const TextureBase&)	= default;

protected:
    virtual void _calculateVertices(void)						  noexcept = 0;
    virtual SDL_Texture* _ownerTexture(SDL_Renderer*)		const noexcept = 0;
    virtual void _applyColor(SDL_Renderer*, const Color&)	const noexcept = 0;

public:
    virtual void rotate(f32)									noexcept = 0;
    virtual void move(const Vec2d&)								noexcept = 0;
    virtual void scale(const Fact2d&)							noexcept = 0;
    virtual void resize(const Dim2d&)							noexcept = 0;
    virtual void transform(const Vec2d&, f32, const Fact2d&)	noexcept = 0;

    virtual Vec2d center(void) 			const noexcept = 0;
    virtual Dim2d size(void)   			const noexcept = 0;
    virtual Dim2d realSize(void)		const noexcept = 0;
    virtual const Geometry& shape(void) const noexcept = 0;

    virtual void unclip(void) 						noexcept = 0;
    virtual void clip(const Vec2d&, const Dim2d&)	noexcept = 0;
    virtual bool hasIntersected(const Geometry&) 	const noexcept = 0;
    virtual bool hasIntersected(const TextureBase&) const noexcept;

    virtual const SDL_Rect* getClipRect(void) 		const noexcept = 0;
    virtual std::vector<SDL_Vertex> vertices(void)	const noexcept = 0;
    
public:
    virtual ~TextureBase(void) = default;
    
private:
    friend class Window;
};


template<IsShape S> class Texture : public TextureBase {

protected:
    struct TextureData {
        SDL_Texture* texture{nullptr};
        Color        current_color{rmk::color::white};
    };

protected:
    S                                       		m_shape;
    SDL_Rect                                		m_srcrect{0,0,0,0};
    bool                                    		m_use_clip{false};
    Vec2d                                   		m_clip_pos{0, 0};
    Dim2d                                   		m_clip_size{0, 0};
    Dim2d                                   		m_real_size{0, 0};
    std::vector<SDL_Vertex>                 		m_vertices;
    Surface              							m_surface;
    mutable std::map<SDL_Renderer*, TextureData>    m_textures;

protected:
    Texture(const S& shape) : m_shape(shape) {}

public:
    Texture(const Texture&);
    Texture& operator=(const Texture&);
    Texture(std::string_view, const S&);

public:
    Texture(Texture&&)				= default;
    Texture& operator=(Texture&&)	= default;

protected:
    void _copy(const Texture<S>&) 						  noexcept;
    void _calculateVertices(void) 						  noexcept override;
    SDL_Texture* _ownerTexture(SDL_Renderer*)		const noexcept override;
    void _applyColor(SDL_Renderer*, const Color&)	const noexcept override;

public:
    void rotate(f32) 								 noexcept override;
    void move(const Vec2d&) 						 noexcept override;
    void scale(const Fact2d&)  						 noexcept override;
    void resize(const Dim2d&) 						 noexcept override;
    void transform(const Vec2d&, f32, const Fact2d&) noexcept override;

    Dim2d size(void) 			const noexcept override;
    Vec2d center(void) 			const noexcept override;
    Dim2d realSize(void) 		const noexcept override;
    const Geometry& shape(void) const noexcept override;

    void unclip(void) 					  noexcept override;
    void clip(const Vec2d&, const Dim2d&) noexcept override;
	
    bool hasIntersected(const Geometry&) 	const noexcept override;
    const SDL_Rect* getClipRect(void) 		const noexcept override;
    std::vector<SDL_Vertex> vertices(void)  const noexcept override;

public:
    virtual ~Texture(void);

    friend class Window;
};


template<IsShape S>
using Image = Texture<S>;


class Sprite : public Image<Rectangle> {
public:
    Sprite(std::string_view, const Rectangle&);
};


struct GlyphAtlas {
public:
    struct AtlasData {
        SDL_Texture* texture{nullptr};
    };

public:
    std::map<SDL_Renderer*, AtlasData>	textures;
    std::map<char, SDL_Rect>  			glyphs;
    i32                       			glyph_height{0};
    i32                       			baseline{0};

public:
    GlyphAtlas(void) 							= default;
    GlyphAtlas(const GlyphAtlas&) 				= delete;
    GlyphAtlas& operator=(const GlyphAtlas&)	= delete;
    GlyphAtlas(GlyphAtlas&&) 					= default;
    GlyphAtlas& operator=(GlyphAtlas&&) 		= default;

public:
    ~GlyphAtlas(void);
};


class FontManager {
private:
    struct FontEntry {
        TTF_Font*   font{nullptr};
        GlyphAtlas  atlas;
    };

private:
    std::map<std::string, FontEntry>    m_fonts;

private:
    FontManager(void) 							= default;
    FontManager(const FontManager&) 			= delete;
    FontManager& operator=(const FontManager&)	= delete;

public:
    void load(std::string_view, std::string_view, u8);
    const GlyphAtlas* atlas(std::string_view) const noexcept;

public:
    static FontManager& getInstance(void);

private:
    void _buildAtlas(FontEntry& entry, SDL_Renderer* renderer);

public:
    ~FontManager(void);
	
private:
    friend class Text;
};


inline FontManager& font = FontManager::getInstance();


namespace anchor {
    enum class x : u8 { left, center, right };
    enum class y : u8 { top, middle, bottom };
}


enum fmt : u8 {
	nl,
	tab,
	endl,
	flush
};

class Text : public Texture<Rectangle> {
    
private:
    TTF_Font*               m_font;
    std::string             m_font_name;
    std::string             m_current_text{""};
    Vec2d                   m_anchor_pos;
    anchor::x               m_anchor_x{anchor::x::left};
    anchor::y               m_anchor_y{anchor::y::top};
    u16                     m_max_lengh{0};
	bool					m_erase{false};

public:
    Text(const Text&);
    Text& operator=(const Text&);
    Text(std::string_view, Vec2d);


public:
    Text(Text&&)			= default;
    Text& operator=(Text&&)	= default;

public:
    void anchorX(anchor::x) noexcept;
    void anchorY(anchor::y) noexcept;

public:
    std::string text(void)   const noexcept;

    void clear(void);
    void append(fmt);
    void write(std::string_view);
    void append(std::string_view);

public:
	void maxLengh(u16) 		  noexcept;
	u16  maxLengh(void) const noexcept;
	
private:
    void _textCopy(const Text&);
    void _updateWidth(const GlyphAtlas&) noexcept;

public:
    virtual ~Text(void) = default;

    friend class Window;
};

class Animation : public Sprite {
private:
    i8    	m_current_clip{0};
    u8    	m_loops_remaining{0};
    u8    	m_spacing{0};
    u8    	m_total_clips{0};
    bool  	m_is_playing{false};
    bool  	m_is_paused{false};
    fmax  	m_timer{0.0};
    fmax  	m_clip_duration{0.0};
    Vec2d 	m_start_pos{0, 0};
    Dim2d	m_clip_size{0, 0};

public:
    Animation(const Animation&);
    Animation& operator=(const Animation&);
    Animation(std::string_view, const Rectangle&, u8, Dim2d, Vec2d = {0, 0}, u8 = 0);

public:
    Animation(Animation&&) 				= default;
    Animation& operator=(Animation&&) 	= default;

private:
    void _advance(void);
    void _animationCopy(const Animation&) noexcept;

public:
    void play(i8 = 0, u8 = 12)	noexcept;
    void pause(void)  			noexcept;
    void resume(void) 			noexcept;
    void stop(void)   			noexcept;

public:
    ~Animation(void) override;

private:
    friend class Window;
    friend class AnimationManager;
};


class AnimationManager {
private:
    std::vector<Animation*> m_animations;

private:
    AnimationManager(void) = default;
    AnimationManager(const AnimationManager&) = delete;
    AnimationManager& operator=(const AnimationManager&) = delete;

public:
    static AnimationManager& getInstance(void) noexcept;
    void update(void);

private:
    void _registerAnimation(Animation*)   noexcept;
    void _unregisterAnimation(Animation*) noexcept;

private:
    friend class Animation;
};

inline AnimationManager& animation = AnimationManager::getInstance();

}//namespace rmk

#include <remake2d/template/texture.tpp>
#endif