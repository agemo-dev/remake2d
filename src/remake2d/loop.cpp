#include <remake2d/loop.hpp>
#include <remake2d/time.hpp>
#include <remake2d/color.hpp>
#include <remake2d/event.hpp>
#include <remake2d/physic.hpp>
#include <remake2d/window.hpp>

#include <algorithm>
#include <functional>

namespace rmk {

void MainRenderLoop::update(void) noexcept {
    m_is_running = true;
    
    auto cond = m_condition;
    auto exec = m_execute ? m_execute : [](){};

	if(!cond) return;
	
    while (cond()) {
        event.poll();
        delta.update();
        physics.update();
        
        for (auto& win : xwindow.m_windows) win->clear();
        exec();
        for (auto& win : xwindow.m_windows) win->present();
    }

    m_is_running = false;
}

void MainRenderLoop::execute(Window& win, const std::function<void(void)>& body) noexcept {
    const auto func = [&win](void) { return win.isOpen(); };
    m_condition = func;
    m_execute   = body;
}

void MainRenderLoop::execute(const std::function<bool(void)>& condition, const std::function<void(void)>& body) noexcept {
    m_condition = condition;
    m_execute   = body;
}

bool MainRenderLoop::isRunning(void) const noexcept {
    return m_is_running;
}

MainRenderLoop& MainRenderLoop::getInstance(void) {
    static MainRenderLoop instance;
    return instance;
}

} // namespace rmk