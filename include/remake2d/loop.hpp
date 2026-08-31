#ifndef REMAKE2D_LOOP_
#define REMAKE2D_LOOP_

#include <remake2d/private/update.hpp>

#include <span>
#include <vector>
#include <functional>

namespace rmk {

class MainRenderLoop {
private:
    bool                            m_is_running{false};
    std::function<void(void)>       m_execute{nullptr};
    std::function<bool(void)>       m_condition{nullptr};
    std::vector<Tracker<Updatable>> m_updatables;

private:
    MainRenderLoop(void)                                = default;
    MainRenderLoop(const MainRenderLoop&)               = delete;
    MainRenderLoop& operator=(const MainRenderLoop&)    = delete;

public:
    void update(void) noexcept;
    void execute(class Window&, const std::function<void(void)>&)                      noexcept;
    void execute(const std::function<bool(void)>&, const std::function<void(void)>&) noexcept;

    bool isRunning(void) const noexcept;

    static MainRenderLoop& getInstance(void);
};

inline MainRenderLoop& loop = MainRenderLoop::getInstance();

} // namespace rmk
#endif