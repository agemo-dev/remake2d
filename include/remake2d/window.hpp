#ifndef REMAKE2D_WINDOW_
#define REMAKE2D_WINDOW_

#include <remake2d/vector.hpp>
#include <remake2d/system.hpp>
#include <remake2d/camera.hpp>
#include <remake2d/utility.hpp>
#include <remake2d/numeric.hpp>
#include <remake2d/concept.hpp>
#include <remake2d/config/forward.hpp>

#include <algorithm>
#include <string>
#include <vector>
#include <stack>
#include <map>

#if __has_include(<SDL2/SDL.h>)
    #include <SDL2/SDL.h>
#elif __has_include(<SDL.h>)
    #include <SDL.h>
#else
    #error "SDL not found."
#endif


namespace rmk {

namespace window {

namespace pos {
inline constexpr Vec2d centered  = { SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED  };
inline constexpr Vec2d undefined = { SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED };
} //namespace pos

namespace size {
inline constexpr Dim2d svga   = { 800,  600  };
inline constexpr Dim2d vga    = { 640,  480  };
inline constexpr Dim2d hd     = { 1280, 720  };
inline constexpr Dim2d fullhd = { 1920, 1080 };
inline constexpr Dim2d qhd    = { 2560, 1440 };
inline constexpr Dim2d uhd    = { 3840, 2160 };
} //namespace size

enum class blendmode : u8 {
	none		= (u8)SDL_BLENDMODE_NONE,
	normal		= (u8)SDL_BLENDMODE_BLEND,
	add			= (u8)SDL_BLENDMODE_ADD
};

} //namespace window


class Window {
public:
    struct Viewport {
	public:
        Area       	zone;     
        Camera*     camera{nullptr};

	public:
		Viewport(void) = default;

	public:
		Viewport(const Area&);
		Viewport(const Area&, Camera&);
    };

private:
    u32                              m_window_id;
    SDL_Window*                      m_window{nullptr};
    SDL_Renderer*                    m_renderer{nullptr};
    bool                             m_is_resizable{true};
    bool                             m_is_open{true};
    Vec2d                            m_pos;
    Dim2d                            m_size;
    Vec2d                            m_center;
    std::string                      m_title{};
    std::string                      m_active_viewport{};
    std::stack<Area>                 m_viewport_stack{}; 
    std::vector<SDL_Vertex>          m_screen_vertices{};
    std::map<std::string, Viewport>  m_viewports;
    
public:
    Window(void);
    Window(Window&&)                    = default;
    Window(const Window&)               = delete;
    Window& operator=(Window&&)         = default;
    Window& operator=(const Window&)    = delete;
    Window(std::string_view, Vec2d = window::pos::undefined, Dim2d = window::size::hd);
    
public:
    u32 ID(void)       const noexcept;
    Vec2d pos(void)    const noexcept;
    Dim2d size(void)   const noexcept;
    Vec2d center(void) const noexcept;

public:
    void move(Vec2d)			  noexcept;
    void border(bool)			  noexcept;
    void resize(Dim2d)		   	  noexcept;
    void maxSize(Dim2d) 		  noexcept;
    void resizable(bool)		  noexcept;
    void fullScreen(bool)         noexcept;
    void rename(std::string_view) noexcept;

public:
    void resetViewport(void)				   		noexcept;
    void useViewport(std::string_view)		   		noexcept;
    void unlinkCamera(std::string_view) 	   		noexcept;
    void removeViewport(std::string_view)      		noexcept;
    void linkCamera(std::string_view, Camera&) 		noexcept;
    void addViewport(std::string_view, Viewport)	noexcept;

public:
    void icon(std::string_view);
    void close(void)                  noexcept;
    void present(void)                noexcept;
    std::string title(void)           noexcept;
    bool isOpen(void)           const noexcept;
    bool isFocus(void)          const noexcept;
    bool isResizable(void)      const noexcept;
    void blendMode(window::blendmode) noexcept;

    void clear(Color = rmk::color::black, std::string_view = "") 					  noexcept;
    void draw(const TileMap&,     Color = rmk::color::white, std::string_view = "") noexcept;
    void draw(const Parallax&,    Color = rmk::color::white, std::string_view = "") noexcept;
    void draw(const TextureBase&, Color = rmk::color::white, std::string_view = "") noexcept;
    void draw(const Area&,        Color = rmk::color::white, std::string_view = "") noexcept;
    void draw(const TileGrid&,    Color = rmk::color::white, std::string_view = "") noexcept;
    void draw(const Geometry&,    Color = rmk::color::white, std::string_view = "") noexcept;
    void draw(const PhysicBody&,  Color = rmk::color::white, std::string_view = "") noexcept;
    void fill(const Area&,        Color = rmk::color::white, std::string_view = "") noexcept;
    void fill(const Geometry&,    Color = rmk::color::white, std::string_view = "") noexcept;
    void fill(const PhysicBody&,  Color = rmk::color::white, std::string_view = "") noexcept;

private:
    void _newCenter(void)				 noexcept;
    void _restoreViewport(void)          noexcept;
    void _applyViewport(const Viewport*) noexcept;
    const Viewport* _resolveViewport(std::string_view) const noexcept;
    void _buildScreenVertices(const TextureBase&, const Camera&) noexcept;

public:
    ~Window(void);
	
private:
    friend class Text;
    friend class TileMap;
    friend class TileGrid;
    friend class Parallax;
    friend class FontManager;
    template<IsShape> friend class Texture;
};



class XWindow {
private:
    std::vector<Window*> m_windows;
    
private:
    XWindow(void)					   = default;
    XWindow(XWindow&&)				   = default;
    XWindow(const XWindow&) 		   = delete;
    XWindow& operator=(XWindow&&)      = default;
    XWindow& operator=(const XWindow&) = delete;
    
private:
    void _registerWindow(Window*)   noexcept;
    void _unregisterWindow(Window*) noexcept;

public:
    static XWindow& getInstance(void) noexcept;

private:
    friend class Text;
    friend class Window;
    friend class TileMap;
    friend class Parallax;
    friend class FontManager;
	friend class MainRenderLoop;
    template<IsShape> friend class Texture;
};

inline XWindow& xwindow = XWindow::getInstance();

} //namespace rmk
#endif