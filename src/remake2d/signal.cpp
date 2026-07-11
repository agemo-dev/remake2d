#include <remake2d/signal.hpp>

namespace rmk {

SignalManager::SignalManager(void) {
    m_routine_user.isEngine(true);
}

Task SignalManager::_runUser(void) {
    while (true) {
        for (auto& eval : m_slot_user) eval();
        rmk_pause();
    }
}

SignalManager& SignalManager::getInstance(void) {
    static SignalManager instance;
    return instance;
}

void SignalManager::dispatch(void) {
    for (auto& d : m_all_dispatch) d();
}

} //namespace rmk