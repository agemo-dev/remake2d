#ifndef REMAKE2D_WINDOW_
#define REMAKE2D_WINDOW_

#include <remake2d/area.hpp>
#include <remake2d/system.hpp>
#include <remake2d/camera.hpp>
#include <remake2d/tracker.hpp>
#include <remake2d/utility.hpp>
#include <remake2d/concept.hpp>
#include <remake2d/all/types.hpp>
#include <remake2d/private/nil.hpp>
#include <remake2d/private/draw.hpp>
#include <remake2d/private/layer.hpp>
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
private:
    using UsedLayers   = std::bitset<(usize)layer::count>;
    using ActiveLayers = IVector<i16, (usize)layer::count>;
    using DrawLayers   = std::array<std::vector<DrawPack>,    (usize)layer::count>;
    using FillLayers   = std::array<std::vector<VertexBatch>, (usize)layer::count>;

public:
    class Viewport : public Trackable<Viewport> {
    private:
        UsedLayers   m_used_layers;
        ActiveLayers m_active_layers;

    private:
        DrawLayers m_draw_layers;
        FillLayers m_fill_layers;

    private:
        Area            m_zone{0, 0, 0, 0};
        Camera          m_camera{};
        UnsafeTracker<Window> m_window{nil};

    public:
        explicit Viewport(const Area&);
        Viewport(void)                       = default;
        Viewport(Viewport&&)                 = default;
        Viewport(const Viewport&)            = default;
        Viewport& operator=(Viewport&&)      = default;
        Viewport& operator=(const Viewport&) = default;

    public:
        Camera& camera(void)             noexcept;
        void area(const Area&)           noexcept;
        Area area(void)            const noexcept;
        const Camera& camera(void) const noexcept;

    public:
        void clear(Color = color::black)    noexcept;
        void draw(const Drawable&, i16 = 0) noexcept;
        void fill(const Fillable&, i16 = 0) noexcept;

    private:
        void _present(SDL_Renderer*) noexcept;

    private:
        friend class Window;
    };

private:
    u32                              m_window_id;
    SDL_Window*                      m_window{nullptr};
    SDL_Renderer*                    m_renderer{nullptr};
    bool                             m_is_resizable{true};
    bool                             m_is_open{true};
    Vec2d                            m_pos{0.0f};
    Dim2d                            m_size{0.0f};
    Vec2d                            m_center{0.0f};
    std::string                      m_title{};
    Camera                           m_camera;

private:
    UsedLayers   m_used_layers;
    ActiveLayers m_active_layers;

private:
    DrawLayers m_draw_layers;
    FillLayers m_fill_layers;

private:
    std::vector<UnsafeTracker<Viewport>>    m_viewports;

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
    void connectViewport(Viewport&)     noexcept;
    void disconnectViewport(Viewport&)  noexcept;

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
    bool resizable(void)        const noexcept;
    void blendMode(window::blendmode) noexcept;

    void clear(Color = rmk::color::black)    noexcept;
    void draw(const Drawable&, i16 = 0)      noexcept;
    void fill(const Fillable&, i16 = 0)      noexcept;

private:
    void _newCenter(void) noexcept;

private:
    static void _applyViewport(SDL_Renderer*, std::stack<Area>&, const Area*) noexcept;
    static void _restoreViewport(SDL_Renderer*, std::stack<Area>&)            noexcept;

private:
    static void _flushLayer(SDL_Renderer*, i16, DrawLayers&, FillLayers&) noexcept; // draw a layer
    static usize _normalise(i16)                                    noexcept; // transform layer (i16) to -> usize
    static void _testLayer(i16, UsedLayers&, ActiveLayers&);

private: // place VertexBatch / DrawPack in buffer
    static void _pushFill(const std::vector<VertexBatch>&, i16, const Camera&, Dim2d, FillLayers&)  noexcept;
    static void _pushDraw(const std::vector<DrawPack>&,    i16, const Camera&, Dim2d, DrawLayers&)  noexcept;

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