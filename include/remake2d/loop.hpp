#ifndef REMAKE2D_LOOP_
#define REMAKE2D_LOOP_

#include <span>
#include <vector>
#include <functional>

namespace rmk {

class MainRenderLoop {
private:
    std::function<void(void)>      m_execute;
    std::function<bool(void)>      m_condition;
    bool                           m_is_running{false};

private:
    MainRenderLoop(void) 								= default;
    MainRenderLoop(const MainRenderLoop&) 				= delete;
    MainRenderLoop& operator=(const MainRenderLoop&)	= delete;

public:
    void update(void) noexcept;
    void execute(class Window&, const std::function<void(void)>&) 				     noexcept;
    void execute(const std::function<bool(void)>&, const std::function<void(void)>&) noexcept;

    bool isRunning(void) const noexcept;

    static MainRenderLoop& getInstance(void);
};

inline MainRenderLoop& loop = MainRenderLoop::getInstance();

} // namespace rmk
#endif