#ifndef REMAKE2D_ESIGNAL_TPP_
#define REMAKE2D_ESIGNAL_TPP_

namespace rmk {

template<typename... Args>
void _EngineSignal<Args...>::_evaluate(Args... args) {
    this->m_pending_args = std::make_tuple(args...);
    this->m_needs_emit.store(true);
}

template<typename... Args>
void _EngineSignal<Args...>::_refreshState(void) {
    bool ready = this->m_count > 0;
    this->m_active.store(ready);
    if (ready && !m_registered) {
        this->m_dispatch_index = signalManager.registerDispatchOnly(*this);
        this->m_registered_dispatch = true;
        m_registered = true;
    }
}

template<typename... Args>
void _EventSignal<Args...>::_setScancode(i32 sc) noexcept {
    m_scancodes.push(sc);
}

template<typename... Args>
void _EventSignal<Args...>::_setScancode(std::initializer_list<i32> list) noexcept {
    for (auto sc : list) m_scancodes.push(sc);
}

template<typename... Args>
void _EventSignal<Args...>::_setButton(i32 b) noexcept {
    m_button = b;
}

template<typename... Args>
bool _EventSignal<Args...>::isActive(void) const noexcept {
    if (!m_scancodes.empty()) {
        for (auto& sc : m_scancodes) if (sdl.keyPressed(sc)) return true;
    }

    if (m_button != -1) {
        for (int i = 0; i < sdl.numJoysticks(); i++) {
            i32 id = sdl.joystickInstanceId(i);
            SDL_GameController* ctrl = _getOpenController(id);
            if (ctrl && sdl.controllerButtonPressed(ctrl, m_button))
                return true;
        }
    }

    return false;
}

} // namespace rmk
#endif