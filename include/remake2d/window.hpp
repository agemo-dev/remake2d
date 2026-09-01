#ifndef REMAKE2D_WINDOW_
#define REMAKE2D_WINDOW_

#include <remake2d/draw.hpp>
#include <remake2d/layer.hpp>
#include <remake2d/system.hpp>
#include <remake2d/camera.hpp>
#include <remake2d/tracker.hpp>
#include <remake2d/utility.hpp>
#include <remake2d/concept.hpp>
#include <remake2d/all/types.hpp>
#include <remake2d/private/nil.hpp>
#include <remake2d/private/ivector.hpp>
#include <remake2d/config/forward.hpp>

#include <algorithm>
#include <string>
#include <vector>
#include <stack>
#include <map>

namespace rmk {

namespace window {

namespace pos {
inline constexpr Vec2d centered  = { -1 };
inline constexpr Vec2d undefined = { -2 };
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
    none,
    normal,
    add,
    mod,
    mul
};

} //namespace window

class Window : public Trackable<Window> {
public:
    class Viewport : public Trackable<Viewport> {
    public:
        Area    zone;
        Camera  camera;

    public:
        Viewport(void) = default;

    public:
        Viewport(const Area&);
        Viewport(const Area&, const Camera&);

    public:
        void draw(const Drawable&, u16 = 0) noexcept;
        void fill(const Fillable&, u16 = 0) noexcept;

    private:
        Tracker<Window> m_window{nil};

    private:
        friend class Window;
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
    Camera                           m_camera;

private:
    std::bitset<(usize)layer::count>   m_used_layers;
    IVector<i16, (usize)layer::count>  m_active_layers;

private:
    std::array<std::vector<DrawPack>,    (usize)layer::count>  m_draw_layers;
    std::array<std::vector<VertexBatch>, (usize)layer::count>  m_fill_layers;

private:
    std::vector<Tracker<Viewport>>           m_viewports;
    Tracker<Viewport>                        m_active_viewport{};
    std::stack<Area>                         m_viewport_stack{};

public:
    Window(void);
    Window(Window&&)                    noexcept;
    Window(const Window&)               = delete;
    Window& operator=(Window&&)         noexcept;
    Window& operator=(const Window&)    = delete;
    Window(std::string_view, Vec2d = window::pos::undefined, Dim2d = window::size::hd);

public:
    u32 ID(void)       const noexcept;
    Vec2d pos(void)    const noexcept;
    Dim2d size(void)   const noexcept;
    Vec2d center(void) const noexcept;
    SDL_Renderer* renderer(void) const noexcept;

public:
    void move(Vec2d)              noexcept;
    void border(bool)             noexcept;
    void resize(Dim2d)            noexcept;
    void maxSize(Dim2d)           noexcept;
    void resizable(bool)          noexcept;
    void fullScreen(bool)         noexcept;
    void rename(std::string_view) noexcept;

public:
    void resetViewport(void)               noexcept;
    void useViewport(Viewport&)            noexcept;
    void connectViewport(Viewport&)        noexcept;
    void disconnectViewport(Viewport&)     noexcept;

public:
    void icon(std::string_view);
    void close(void)                  noexcept;
    void present(void)                noexcept;
    void screenshot(std::string_view) noexcept;

public:
    std::string title(void)           noexcept;
    Camera& camera(void)              noexcept;
    const Camera& camera(void)  const noexcept;
    Area area(void)             const noexcept;
    bool isOpen(void)           const noexcept;
    bool isFocus(void)          const noexcept;
    bool isResizable(void)      const noexcept;
    void blendMode(window::blendmode) noexcept;

    void clear(Color = rmk::color::black)  noexcept;
    void draw(const Drawable&, u16 = 0)      noexcept;
    void fill(const Fillable&, u16 = 0)      noexcept;

private:
    void _newCenter(void)                noexcept;
    void _restoreViewport(void)          noexcept;
    void _applyViewport(const Viewport*) noexcept;
    void _flushLayer(u16)                noexcept;
    void _pushDraw(const std::vector<DrawPack>&, u16, const Viewport* = nullptr)    noexcept;
    void _pushFill(const std::vector<VertexBatch>&, u16, const Viewport* = nullptr) noexcept;

public:
    ~Window(void);

private:
    friend class Text;
    friend class XWindow;
    friend class FontManager;
    template<IsShape> friend class Texture;
};



class XWindow {
private:
    std::vector<Tracker<Window>>  m_windows;
    Tracker<Window>               m_last_drawn_window{};

private:
    XWindow(void);
    XWindow(XWindow&&)                 = default;
    XWindow(const XWindow&)            = delete;
    XWindow& operator=(XWindow&&)      = default;
    XWindow& operator=(const XWindow&) = delete;

private:
    void _registerWindow(Window*)     noexcept;
    void _unregisterWindow(Window*)   noexcept;
    void _setLastDrawnWindow(Window*) noexcept;

public:
    static XWindow& getInstance(void) noexcept;

public:
    const Tracker<Window>& lastDrawnWindow(void) const noexcept;

private:
    friend class Text;
    friend class Window;
    friend class FontManager;
    friend class MainRenderLoop;
    template<IsShape> friend class Texture;
};

inline XWindow& xwindow = XWindow::getInstance();

} //namespace rmk

#endif
