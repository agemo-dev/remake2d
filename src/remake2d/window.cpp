#include <remake2d/error.hpp>
#include <remake2d/event.hpp>
#include <remake2d/window.hpp>
#include <remake2d/physic.hpp>
#include <remake2d/camera.hpp>
#include <remake2d/utility.hpp>

#include <map>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <filesystem>

#include <SDL2/SDL.h>

namespace rmk {

Window::Viewport::Viewport(const Area& z) : zone(z), camera({0, 0}, {(f32)z.w, (f32)z.h}) {}
Window::Viewport::Viewport(const Area& z, const Camera& cam) : zone(z), camera(cam) {}

Window::Window(void) : Window("RE:MAKE 2D") {}
Window::Window(std::string_view name, Vec2d pos, Dim2d size)
    : m_size(size), m_title(name), m_camera({0, 0}, size) {
    int x, y;

    rmk::system._init();

	switch(pos.x) {
	case -1:
		pos = { SDL_WINDOWPOS_CENTERED };
	case -2:
		pos = { SDL_WINDOWPOS_UNDEFINED };
	}

    m_window = SDL_CreateWindow(std::string(name).c_str(), pos.x, pos.y, size.w, size.h, SDL_WINDOW_SHOWN);
    if (!m_window) rmk_dynamicAssert(rmk::WindowError, (std::string(error::window::window_no_create) + " : " + SDL_GetError()));

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    if (!m_renderer) rmk_dynamicAssert(rmk::WindowError, (std::string(error::window::renderer_no_create) + " : " + SDL_GetError()));

    SDL_GetWindowPosition(m_window, &x, &y);
    m_pos = { (f32)x, (f32)y };

    m_window_id = (u32)SDL_GetWindowID(m_window);
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    _newCenter();
    xwindow._registerWindow(this);
}

Window::Window(Window&& other) noexcept
    : Trackable<Window>(std::move(other))
    , m_window_id(other.m_window_id)
    , m_window(other.m_window)
    , m_renderer(other.m_renderer)
    , m_is_resizable(other.m_is_resizable)
    , m_is_open(other.m_is_open)
    , m_pos(other.m_pos)
    , m_size(other.m_size)
    , m_center(other.m_center)
    , m_title(std::move(other.m_title))
    , m_camera(std::move(other.m_camera))
    , m_draw_layers(std::move(other.m_draw_layers))
    , m_fill_layers(std::move(other.m_fill_layers))
    , m_viewports(std::move(other.m_viewports))
    , m_active_viewport(std::move(other.m_active_viewport))
{
    other.m_window   = nullptr;
    other.m_renderer = nullptr;
    other.m_is_open  = false;
    other.m_window_id = 0;

    relocate();
}

Window& Window::operator=(Window&& other) noexcept {
    if (this != &other) {
        close();

        Trackable<Window>::operator=(std::move(other));

        m_window_id        = other.m_window_id;
        m_window            = other.m_window;
        m_renderer          = other.m_renderer;
        m_is_resizable       = other.m_is_resizable;
        m_is_open           = other.m_is_open;
        m_pos               = other.m_pos;
        m_size               = other.m_size;
        m_center            = other.m_center;
        m_title              = std::move(other.m_title);
        m_camera             = std::move(other.m_camera);
        m_draw_layers        = std::move(other.m_draw_layers);
        m_fill_layers        = std::move(other.m_fill_layers);
        m_viewports          = std::move(other.m_viewports);
        m_active_viewport    = std::move(other.m_active_viewport);

        other.m_window   = nullptr;
        other.m_renderer = nullptr;
        other.m_is_open  = false;
        other.m_window_id = 0;

        relocate();
    }
    return *this;
}

u32 Window::ID(void) const noexcept {
    return m_window_id;
}

Vec2d Window::pos(void) const noexcept {
    return m_pos;
}

Dim2d Window::size(void) const noexcept {
    return m_size;
}

Vec2d Window::center(void) const noexcept {
    return m_center;
}

SDL_Renderer* Window::renderer(void) const noexcept {
    return m_renderer;
}

Area Window::area(void) const noexcept {
	return Area(Vec2d(0), m_size);
}

void Window::move(Vec2d pos) noexcept {
    m_pos = pos;
    _newCenter();
    SDL_SetWindowPosition(m_window, (u32)pos.x, (u32)pos.y);
}

void Window::resize(Dim2d size) noexcept {
    resizable(true);
    m_size = size;
    m_camera.resize(size);
    _newCenter();
    SDL_SetWindowSize(m_window, (u32)size.w, (u32)size.h);
}

void Window::maxSize(Dim2d size) noexcept {
    SDL_SetWindowMinimumSize(m_window, (u32)size.w, (u32)size.h);
}

void Window::rename(std::string_view title) noexcept {
    m_title = title;
    SDL_SetWindowTitle(m_window, m_title.c_str());
}

void Window::icon(std::string_view path) {
    SDL_Surface* surface = IMG_Load(std::string(path).c_str());
    if(!surface) rmk_dynamicAssert(rmk::TextureError, (std::string(error::texture::texture_no_load) + " : " + IMG_GetError()));
    SDL_SetWindowIcon(m_window, surface);
    SDL_FreeSurface(surface);
}

void Window::blendMode(window::blendmode mode) noexcept {
	SDL_BlendMode bm;
	switch (mode) {
	    case window::blendmode::normal:
	        bm = SDL_BLENDMODE_BLEND;
	        break;
	    case window::blendmode::add:
	        bm = SDL_BLENDMODE_ADD;
	        break;
	    case window::blendmode::mod:
	        bm = SDL_BLENDMODE_MOD;
	        break;
	    case window::blendmode::mul:
	        bm = SDL_BLENDMODE_MUL;
	        break;
	    case window::blendmode::none:
	    default:
	        bm = SDL_BLENDMODE_NONE;
	        break;
	}
    SDL_SetRenderDrawBlendMode(m_renderer, bm);
}

void Window::resizable(bool stat) noexcept {
    m_is_resizable = stat;
    SDL_SetWindowResizable(m_window, stat ? SDL_TRUE : SDL_FALSE);
}

void Window::fullScreen(bool stat) noexcept {
    SDL_SetWindowFullscreen(m_window, stat ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

void Window::border(bool stat) noexcept {
    SDL_SetWindowBordered(m_window, stat ? SDL_TRUE : SDL_FALSE);
}

void Window::present(void) noexcept {
    for (auto layer : m_active_layers) _flushLayer(layer);

    m_used_layers.clear();
    m_active_layers.clear();

    m_draw_layers.clear();
    m_fill_layers.clear();
    SDL_RenderPresent(m_renderer);
}

Camera& Window::camera(void) noexcept {
    return m_camera;
}

const Camera& Window::camera(void) const noexcept {
    return m_camera;
}

void Window::draw(const Drawable& obj, u16 layer) noexcept {
    _testLayer(layer);
	xwindow._setLastDrawnWindow(this);
	_pushDraw(obj.__draw__(), layer);
}

void Window::fill(const Fillable& obj, u16 layer) noexcept {
    _testLayer(layer);
	xwindow._setLastDrawnWindow(this);
	_pushFill(obj.__fill__(), layer);
}

void Window::Viewport::draw(const Drawable& obj, u16 layer) noexcept {
	Window* win = m_window.locate();
	if (!win) return;
    win->_testLayer(layer);
	xwindow._setLastDrawnWindow(win);
	win->_pushDraw(obj.__draw__(), layer, this);
}

void Window::Viewport::fill(const Fillable& obj, u16 layer) noexcept {
	Window* win = m_window.locate();
	if (!win) return;
    win->_testLayer(layer);
	xwindow._setLastDrawnWindow(win);
	win->_pushFill(obj.__fill__(), layer, this);
}

void Window::_testLayer(i16 layer) const {
    if (m_used_layers.test(layer)) return;

    if (layer < (i16)layer::min || layer > (i16)layer::max) {
        rmk_dynamicAssert(rmk::SceneError, error::scene::layer_is_overlimits);
    }

    m_active_layers.pushAndSort(layer);
    m_used_layers.set(layer - (i16)layer::min);
}

void Window::_pushDraw(const std::vector<DrawPack>& pack, u16 layer, const Viewport* vp) noexcept {
    const Camera& cam = vp ? vp->camera : m_camera;
    Vec2d origin = cam.viewCenter();
    f32   zoom   = cam.zoom();
    Dim2d bounds = vp ? Dim2d{ (f32)vp->zone.w, (f32)vp->zone.h } : m_size;

    auto& dst = m_draw_layers[layer];

    for (const auto& dp : pack) {
        DrawPack segment;
        segment.color = dp.color;
        f32 minx = 0, miny = 0, maxx = 0, maxy = 0;
        bool first = true;

        auto flushSegment = [&](void) {
            if (!segment.points.empty() && !(maxx < 0 || maxy < 0 || minx > bounds.w || miny > bounds.h)) {
                dst.push_back(segment);
                dst.back().points.push_back(contour::breaker());
            }
            segment.points.clear();
            first = true;
        };

        for (auto p : dp.points) {
            if (contour::isBreak(p)) { flushSegment(); continue; }
            p = (p - origin) * zoom;
            segment.points.push_back(p);
            if (first) { minx = maxx = p.x; miny = maxy = p.y; first = false; }
            else {
                minx = std::min(minx, p.x); maxx = std::max(maxx, p.x);
                miny = std::min(miny, p.y); maxy = std::max(maxy, p.y);
            }
        }
        flushSegment();
    }
}

void Window::_pushFill(const std::vector<VertexBatch>& batches, u16 layer, const Viewport* vp) noexcept {
    const Camera& cam = vp ? vp->camera : m_camera;
    Vec2d origin = cam.viewCenter();
    f32   zoom   = cam.zoom();
    Dim2d bounds = vp ? Dim2d{ (f32)vp->zone.w, (f32)vp->zone.h } : m_size;

    auto& dst = m_fill_layers[layer];
    for (const auto& batch : batches) {
        if (batch.vertices.empty()) continue;

        VertexBatch transformed;
        transformed.texture = batch.texture;
        transformed.vertices = batch.vertices;

        f32 minx = 0, miny = 0, maxx = 0, maxy = 0;
        bool first = true;
        for (auto& v : transformed.vertices) {
            v.x = (v.x - origin.x) * zoom;
            v.y = (v.y - origin.y) * zoom;
            if (first) { minx = maxx = v.x; miny = maxy = v.y; first = false; }
            else {
                minx = std::min(minx, v.x); maxx = std::max(maxx, v.x);
                miny = std::min(miny, v.y); maxy = std::max(maxy, v.y);
            }
        }

        if (maxx < 0 || maxy < 0 || minx > bounds.w || miny > bounds.h) continue;
        dst.push_back(std::move(transformed));
    }
}

void Window::_flushLayer(u16 layer) const noexcept {
    auto& dr = m_draw_layers[layer];
    auto& fl = m_fill_layers[layer];

    if (!dr.empty()) {
        std::vector<SDL_FPoint> segment;
        for (const auto& dp : vec) {
            SDL_SetRenderDrawColor(m_renderer, dp.color.r, dp.color.g, dp.color.b, dp.color.a);
            segment.clear();
            for (const auto& p : dp.points) {
                if (contour::isBreak(p)) {
                    if (segment.size() >= 2) SDL_RenderDrawLinesF(m_renderer, segment.data(), (int)segment.size());
                    segment.clear();
                    continue;
                }
                segment.push_back(p);
            }
            if (segment.size() >= 2) SDL_RenderDrawLinesF(m_renderer, segment.data(), (int)segment.size());
        }
    }

    if (!fl.empty()) {
        std::vector<SDL_Vertex> sdlVerts;
        for (auto& batch : fl) {
            sdlVerts.clear();
            sdlVerts.reserve(batch.vertices.size());
            for (const auto& v : batch.vertices) sdlVerts.push_back((SDL_Vertex)v);
            SDL_RenderGeometry(m_renderer, batch.texture, sdlVerts.data(), (int)sdlVerts.size(), nullptr, 0);
        }
    }
}

void Window::screenshot(std::string_view path) noexcept {
    int w = 0, h = 0;
    SDL_GetRendererOutputSize(m_renderer, &w, &h);
    if (w <= 0 || h <= 0) return;

    std::filesystem::path p = data.root() + "/screenshot";
    p /= std::string(path) + ".png";

    if (p.has_parent_path()) {
        std::error_code ec;
        std::filesystem::create_directories(p.parent_path(), ec);
        if (ec) return;
    }

    SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA32);
    if (!surf) return;

    if (SDL_RenderReadPixels(m_renderer, nullptr, SDL_PIXELFORMAT_RGBA32, surf->pixels, surf->pitch) == 0) {
        IMG_SavePNG(surf, p.string().c_str());
    }

    SDL_FreeSurface(surf);
}

bool Window::resizable(void) const noexcept {
    return m_is_resizable;
}

std::string Window::title(void) noexcept {
    return m_title;
}

bool Window::isOpen(void) const noexcept {
    return m_is_open;
}

bool Window::isFocus(void) const noexcept {
    return (SDL_GetWindowFlags(m_window) & SDL_WINDOW_INPUT_FOCUS) != 0;
}

void Window::close(void) noexcept {
    if(m_window)   SDL_DestroyWindow(m_window);
    if(m_renderer) SDL_DestroyRenderer(m_renderer);
    m_window   = nullptr;
    m_renderer = nullptr;
    m_is_open  = false;
	xwindow._unregisterWindow(this);
}

Window::~Window(void) {
    close();
	rmk::system._quit();
}

void Window::connectViewport(Viewport& v) noexcept {
    v.m_window = this->tracker();
    auto tracker = v.tracker();
    for (auto& t : m_viewports) if (t == tracker) return;
    m_viewports.push_back(tracker);
}

void Window::disconnectViewport(Viewport& v) noexcept {
    auto tracker = v.tracker();
    auto it = std::find(m_viewports.begin(), m_viewports.end(), tracker);
    if (it != m_viewports.end()) m_viewports.erase(it);
    if (m_active_viewport == tracker) m_active_viewport = {};
    v.m_window = nil;
}

void Window::useViewport(Viewport& v) noexcept {
    m_active_viewport = v.tracker();
}

void Window::resetViewport(void) noexcept {
    m_active_viewport = {};
    SDL_RenderSetViewport(m_renderer, nullptr);
}

void Window::_newCenter(void) noexcept {
    m_center = { m_pos.x + (m_size.w / 2), m_pos.y + (m_size.h / 2) };
}

void Window::_applyViewport(const Viewport* vp) noexcept {
    SDL_Rect c{};
    SDL_RenderGetViewport(m_renderer, &c);
    m_viewport_stack.push({c.x, c.y, c.w, c.h});
    if (vp) {
        SDL_Rect v = vp->zone;
        SDL_RenderSetViewport(m_renderer, &v);
    }
}

void Window::_restoreViewport(void) noexcept {
    if (m_viewport_stack.empty()) return;
    Area previous = m_viewport_stack.top();
    m_viewport_stack.pop();

    if (previous.x == 0 && previous.y == 0 && previous.w == 0 && previous.h == 0) {
        SDL_RenderSetViewport(m_renderer, nullptr);
    } else {
        SDL_Rect p{ (int)previous.x, (int)previous.y, (int)previous.w, (int)previous.h };
        SDL_RenderSetViewport(m_renderer, &p);
    }
}

void Window::clear(Color color) noexcept {
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_renderer);
}

XWindow::XWindow(void) {
    event.onWindowClose.joinPriority([this] (u32 id) {
        for (auto& t : m_windows) {
            Window* w = t.locate();
            if (w && w->m_window_id == id) { w->close(); return; }
        }
    });
    event.onWindowMoved.joinPriority([this] (u32 id, Vec2d pos) {
        for (auto& t : m_windows) {
            Window* w = t.locate();
            if (w && w->m_window_id == id) {
                w->m_pos = pos;
                w->_newCenter();
                return;
            }
        }
    });
    event.onWindowResized.joinPriority([this] (u32 id, Dim2d size) {
        for (auto& t : m_windows) {
            Window* w = t.locate();
            if (w && w->m_window_id == id) {
                w->m_size = size;
                w->m_camera.resize(size);
                w->_newCenter();
                return;
            }
        }
    });
}

XWindow& XWindow::getInstance(void) noexcept {
    static XWindow instance;
    return instance;
 }

void XWindow::_registerWindow(Window* win) noexcept {
    for (auto& t : m_windows) if (t.locate() == win) return;
	auto tracker = win->tracker();
    m_windows.push_back(tracker);
}

void XWindow::_unregisterWindow(Window* win) noexcept {
    auto it = std::find_if(m_windows.begin(), m_windows.end(),
        [win](Tracker<Window>& t) { return t.locate() == win; });
    if (it != m_windows.end()) m_windows.erase(it);
    if (m_last_drawn_window.locate() == win) m_last_drawn_window = nil;
}

void XWindow::_setLastDrawnWindow(Window* win) noexcept {
    m_last_drawn_window = win->tracker();
}

const Tracker<Window>& XWindow::lastDrawnWindow(void) const noexcept {
    return m_last_drawn_window;
}

} // namespace rmk