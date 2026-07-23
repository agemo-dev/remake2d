#include <remake2d/error.hpp>
#include <remake2d/event.hpp>
#include <remake2d/window.hpp>
#include <remake2d/physic.hpp>
#include <remake2d/camera.hpp>
#include <remake2d/tilemap.hpp>
#include <remake2d/tilegrid.hpp>
#include <remake2d/parallax.hpp>

#include <map>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <filesystem>

namespace rmk {

Window::Viewport::Viewport(const Area& z) : zone(z) {}
Window::Viewport::Viewport(const Area& z, Camera& cam) : zone(z), camera(&cam) {}

Window::Window(void) : Window("RE:MAKE 2D") {}
Window::Window(std::string_view name, Vec2d pos, Dim2d size) : m_title(name), m_size(size) {
    int x, y;
	
    rmk::system.init();
	
    m_window = SDL_CreateWindow(name.data(), pos.x, pos.y, size.w, size.h, SDL_WINDOW_SHOWN);
    if(!m_window) rmk_dynamicAssert(rmk::WindowError, (std::string(error::window::window_no_create) + " : " + SDL_GetError()));
	
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    if(!m_renderer) rmk_dynamicAssert(rmk::WindowError, (std::string(error::window::renderer_no_create) + " : " + SDL_GetError()));
    
    SDL_GetWindowPosition(m_window, &x, &y);
    m_pos = { (f32)x, (f32)y };
	
    m_window_id = (u32)SDL_GetWindowID(m_window);
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
    
    event.onWindowClose.joinPriority([this] (u32 id) {
        if(id == this->m_window_id) this->close();
    });
    event.onWindowMoved.joinPriority([this] (u32 id, Vec2d pos) { 
        if(id == this->m_window_id) {
            this->m_pos = pos;
            this->_newCenter();
        }
    });
    event.onWindowResized.joinPriority([this] (u32 id, Dim2d size) { 
        if(id == this->m_window_id) {
            this->m_size = size;
            this->_newCenter();
        } 
    });
	
    _newCenter();
    xwindow._registerWindow(this);
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

void Window::move(Vec2d pos) noexcept {
    m_pos = pos;
    _newCenter();
    SDL_SetWindowPosition(m_window, (u32)pos.x, (u32)pos.y);
}

void Window::resize(Dim2d size) noexcept {
    resizable(true);
    m_size = size;
    _newCenter();
    SDL_SetWindowSize(m_window, (u32)size.w, (u32)size.h);
}

void Window::maxSize(Dim2d size) noexcept {
    SDL_SetWindowMinimumSize(m_window, (u32)size.w, (u32)size.h);
}

void Window::rename(std::string_view title) noexcept {
    SDL_SetWindowTitle(m_window, title.data());
}

void Window::icon(std::string_view path) {
    SDL_Surface* surface = IMG_Load(std::string(path).c_str());
    if(!surface) rmk_dynamicAssert(rmk::TextureError, (std::string(error::texture::texture_no_load) + " : " + IMG_GetError()));
    SDL_SetWindowIcon(m_window, surface);
    SDL_FreeSurface(surface);
}

void Window::blendMode(window::blendmode mode) noexcept {
    SDL_SetRenderDrawBlendMode(m_renderer, (SDL_BlendMode)mode);
}

void Window::resizable(bool stat) noexcept {
    SDL_SetWindowResizable(m_window, stat ? SDL_TRUE : SDL_FALSE);
}

void Window::fullScreen(bool stat) noexcept {
    SDL_SetWindowFullscreen(m_window, stat ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

void Window::border(bool stat) noexcept {
    SDL_SetWindowBordered(m_window, stat ? SDL_TRUE : SDL_FALSE);
}

void Window::present(void) noexcept {
    SDL_RenderPresent(m_renderer);
}

bool Window::isOpen(void) const noexcept {
    return m_is_open;
}

bool Window::isFocus(void) const noexcept {
    return SDL_GetWindowFlags(m_window) & SDL_WINDOW_INPUT_FOCUS;
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
	rmk::system.quit();
}


void Window::addViewport(std::string_view name, Viewport v) noexcept {
    m_viewports[std::string(name)] = v;
}

void Window::linkCamera(std::string_view viewport, Camera& cam) noexcept {
    auto it = m_viewports.find(std::string(viewport));
    if (it != m_viewports.end()) it->second.camera = &cam;
}

void Window::unlinkCamera(std::string_view viewport) noexcept {
    auto it = m_viewports.find(std::string(viewport));
    if (it != m_viewports.end()) it->second.camera = nullptr;
}

void Window::removeViewport(std::string_view name) noexcept {
    m_viewports.erase(std::string(name));
    if (m_active_viewport == name) m_active_viewport = {};
}

void Window::useViewport(std::string_view name) noexcept {
    m_active_viewport = std::string(name);
}

void Window::resetViewport(void) noexcept {
    m_active_viewport = {};
    SDL_RenderSetViewport(m_renderer, nullptr);
}

void Window::_newCenter(void) noexcept {
    m_center = { m_pos.x + (m_size.w / 2), m_pos.y + (m_size.h / 2) };
}

const Window::Viewport* Window::_resolveViewport(std::string_view viewport) const noexcept {
    std::string key = viewport.empty() ? m_active_viewport : std::string(viewport);
    if (key.empty()) return nullptr;
    auto it = m_viewports.find(key);
    return (it != m_viewports.end()) ? &it->second : nullptr;
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
    SDL_Rect zero{};
    if (SDL_memcmp(&previous, &zero, sizeof(SDL_Rect)) == 0) {
        SDL_RenderSetViewport(m_renderer, nullptr);
    } else {
        SDL_Rect p = previous;
        SDL_RenderSetViewport(m_renderer, &p);
    }
}



void Window::clear(Color color, std::string_view viewport) noexcept {
    const Viewport* vp = _resolveViewport(viewport);
    _applyViewport(vp);
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_renderer);
    _restoreViewport();
}

void Window::draw(const TextureBase& tex, Color color, std::string_view viewport) noexcept {
    const Viewport* vp = _resolveViewport(viewport);
    _applyViewport(vp);

    tex._applyColor(m_renderer, color);

    SDL_Texture* t = tex._ownerTexture(m_renderer);
    if (t) {
        auto verts = tex.vertices();
        SDL_RenderGeometry(m_renderer, t, verts.data(), (int)verts.size(), nullptr, 0);
    }

    _restoreViewport();
}

void Window::draw(const Parallax& para, Color color, std::string_view viewport) noexcept {
    const Viewport* vp = _resolveViewport(viewport);
    _applyViewport(vp);
    para._draw(*this, color);
    _restoreViewport();
}

void Window::draw(const TileMap& tile, Color color, std::string_view viewport) noexcept {
    const Viewport* vp = _resolveViewport(viewport);
    _applyViewport(vp);
    if(vp && vp->camera) tile._draw(*this, color, *vp->camera);
    else tile._draw(*this, color);
    _restoreViewport();
}

void Window::draw(const Area& area, Color color, std::string_view viewport) noexcept {
    const Viewport* vp = _resolveViewport(viewport);
    _applyViewport(vp);
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = area;
    SDL_RenderDrawRect(m_renderer, &rect);
    _restoreViewport();
}

void Window::draw(const TileGrid& grid, Color color, std::string_view viewport) noexcept {
    const Viewport* vp = _resolveViewport(viewport);
    _applyViewport(vp);
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    grid._draw(*this);
    _restoreViewport();
}

void Window::fill(const Area& area, Color color, std::string_view viewport) noexcept {
    const Viewport* vp = _resolveViewport(viewport);
    _applyViewport(vp);
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = area;
    SDL_RenderFillRect(m_renderer, &rect);
    _restoreViewport();
}

void Window::draw(const Geometry& shape, Color color, std::string_view viewport) noexcept {
    const Viewport* vp = _resolveViewport(viewport);
    _applyViewport(vp);
    auto contour = shape._toContour();
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLinesF(m_renderer, contour.data(), (int)contour.size());
    _restoreViewport();
}

void Window::fill(const Geometry& shape, Color color, std::string_view viewport) noexcept {
    const Viewport* vp = _resolveViewport(viewport);
    _applyViewport(vp);
    auto verts = shape._toVertices();
    for (auto& v : verts) {
        v.color.r = color.r;
        v.color.g = color.g;
        v.color.b = color.b;
        v.color.a = color.a;
    }
    SDL_RenderGeometry(m_renderer, nullptr, verts.data(), (int)verts.size(), nullptr, 0);
    _restoreViewport();
}

void Window::draw(const PhysicBody& body, Color color, std::string_view viewport) noexcept {
    const Viewport* vp = _resolveViewport(viewport);
    _applyViewport(vp);

	auto& contour = body.m_cached_contour;
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLinesF(m_renderer, contour.data(), (int)contour.size());
    _restoreViewport();
}

void Window::fill(const PhysicBody& body, Color color, std::string_view viewport) noexcept {
    const Viewport* vp = _resolveViewport(viewport);
    _applyViewport(vp);
    auto verts = body.m_cached_vertices;
	
    for (auto& v : verts) {
        v.color.r = color.r;
        v.color.g = color.g;
        v.color.b = color.b;
        v.color.a = color.a;
    }
	
	SDL_RenderGeometry(m_renderer, nullptr, verts.data(), verts.size(), nullptr, 0);
	_restoreViewport();
}

void Window::_buildScreenVertices(const TextureBase& tex, const Camera& cam) noexcept {
    Vec2d origin = cam.center();
    f32   zoom   = cam.zoom();
    m_screen_vertices = tex.vertices();
    for (auto& v : m_screen_vertices) {
        v.position.x = (v.position.x - origin.x) * zoom;
        v.position.y = (v.position.y - origin.y) * zoom;
    }
}


XWindow& XWindow::getInstance(void) noexcept {
    static XWindow instance;
    return instance;
 }
 
void XWindow::_registerWindow(Window* win) noexcept {
	auto& vec = m_windows;
	auto it   = std::find(vec.begin(), vec.end(), win);
    if(it == vec.end()) vec.push_back(win);
}
	
void XWindow::_unregisterWindow(Window* win) noexcept {
	auto& vec = m_windows;
	auto it   = std::find(vec.begin(), vec.end(), win);
    if(it != vec.end()) vec.erase(it);
}

} // namespace rmk