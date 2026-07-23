#include <remake2d/signal.hpp>

namespace rmk {

SignalManager::SignalManager(void) {
    m_routine_user.isEngine(true);
}

Task SignalManager::_runUser(void) {
    while (true) {
        {
            std::lock_guard lock(m_mtx);
            for (auto& eval : m_slot_user) if (eval) (*eval)();
        }
        rmk_pause();
    }
}

SignalManager& SignalManager::getInstance(void) {
    static SignalManager instance;
    return instance;
}

void SignalManager::dispatch(void) {
    std::lock_guard lock(m_mtx);
    for (auto& d : m_all_dispatch) if (d) (*d)();
}

void SignalManager::unregisterUser(usize idx) noexcept {
    std::lock_guard lock(m_mtx);
    if (idx < m_slot_user.size()) m_slot_user[idx].reset();
}

void SignalManager::unregisterDispatchOnly(usize idx) noexcept {
    std::lock_guard lock(m_mtx);
    if (idx < m_all_dispatch.size()) m_all_dispatch[idx].reset();
}

} //namespace rmk