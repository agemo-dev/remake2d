#ifndef REMAKE2D_SIGNAL_TPP_
#define REMAKE2D_SIGNAL_TPP_

namespace rmk {

template<typename... Args>
void SignalManager::_register(std::vector<std::function<void()>>& slot,
                               Croutine<>& routine,
                               std::atomic<bool>& started,
                               Signal<Args...>& sig,
                               std::function<Task()> run_fn) {
    slot.emplace_back([&sig]() { 
        std::apply([&sig](auto&&... args) { 
            sig._evaluate(std::forward<decltype(args)>(args)...); 
        }, sig.m_default_args()); 
    });
    m_all_dispatch.emplace_back([&sig]() { sig._dispatch(); });
    if (!started.load()) {
        routine.load(run_fn);
        routine.run();
        started.store(true);
    }
}

template<typename... Args>
void SignalManager::registerUser(Signal<Args...>& sig) {
    _register(m_slot_user, m_routine_user, m_started_user, sig,
              [this]() -> Task { return _runUser(); });
}

template<typename... Args>
void SignalManager::registerDispatchOnly(Signal<Args...>& sig) {
    m_all_dispatch.emplace_back([&sig]() { sig._dispatch(); });
}

template<typename... Args>
Signal<Args...>::Signal(const Signal& other)
    : m_once_funcs(other.m_once_funcs)
    , m_connected_funcs(other.m_connected_funcs)
    , m_prioritary_funcs(other.m_prioritary_funcs)
    , m_once_slots(other.m_once_slots)
    , m_connected_slots(other.m_connected_slots)
    , m_prioritary_slots(other.m_prioritary_slots)
    , m_joined_slots(other.m_joined_slots)
    , m_joined_once_slots(other.m_joined_once_slots)
    , m_joined_prioritary_slots(other.m_joined_prioritary_slots)
    , m_count(other.m_count)
    , m_active(other.m_active.load())
    , m_needs_emit(other.m_needs_emit.load())
    , m_previous_cond(other.m_previous_cond.load())
    , m_edge_mode(other.m_edge_mode) {
    
    if (other.m_condition) {
        m_condition = other.m_condition;
    }
    if (other.m_default_args) {
        m_default_args = other.m_default_args;
    }
}

template<typename... Args>
Signal<Args...>::Signal(Signal&& other) noexcept
    : m_once_funcs(std::move(other.m_once_funcs))
    , m_connected_funcs(std::move(other.m_connected_funcs))
    , m_prioritary_funcs(std::move(other.m_prioritary_funcs))
    , m_once_slots(std::move(other.m_once_slots))
    , m_connected_slots(std::move(other.m_connected_slots))
    , m_prioritary_slots(std::move(other.m_prioritary_slots))
    , m_joined_slots(std::move(other.m_joined_slots))
    , m_joined_once_slots(std::move(other.m_joined_once_slots))
    , m_joined_prioritary_slots(std::move(other.m_joined_prioritary_slots))
    , m_count(other.m_count)
    , m_condition(std::move(other.m_condition))
    , m_default_args(std::move(other.m_default_args))
    , m_active(other.m_active.load())
    , m_needs_emit(other.m_needs_emit.load())
    , m_pending_args(std::move(other.m_pending_args))
    , m_previous_cond(other.m_previous_cond.load())
    , m_edge_mode(other.m_edge_mode) {}

template<typename... Args>
Signal<Args...>& Signal<Args...>::operator=(const Signal& other) {
    if (this != &other) {
        m_once_funcs = other.m_once_funcs;
        m_connected_funcs = other.m_connected_funcs;
        m_prioritary_funcs = other.m_prioritary_funcs;
        m_once_slots = other.m_once_slots;
        m_connected_slots = other.m_connected_slots;
        m_prioritary_slots = other.m_prioritary_slots;
        m_joined_slots = other.m_joined_slots;
        m_joined_once_slots = other.m_joined_once_slots;
        m_joined_prioritary_slots = other.m_joined_prioritary_slots;
        m_count = other.m_count;
        m_condition = other.m_condition;
        m_default_args = other.m_default_args;
        m_active = other.m_active.load();
        m_needs_emit = other.m_needs_emit.load();
        m_previous_cond = other.m_previous_cond.load();
        m_edge_mode = other.m_edge_mode;
    }
    return *this;
}

template<typename... Args>
Signal<Args...>& Signal<Args...>::operator=(Signal&& other) noexcept {
    if (this != &other) {
        m_once_funcs = std::move(other.m_once_funcs);
        m_connected_funcs = std::move(other.m_connected_funcs);
        m_prioritary_funcs = std::move(other.m_prioritary_funcs);
        m_once_slots = std::move(other.m_once_slots);
        m_connected_slots = std::move(other.m_connected_slots);
        m_prioritary_slots = std::move(other.m_prioritary_slots);
        m_joined_slots = std::move(other.m_joined_slots);
        m_joined_once_slots = std::move(other.m_joined_once_slots);
        m_joined_prioritary_slots = std::move(other.m_joined_prioritary_slots);
        m_count = other.m_count;
        m_condition = std::move(other.m_condition);
        m_default_args = std::move(other.m_default_args);
        m_active = other.m_active.load();
        m_needs_emit = other.m_needs_emit.load();
        m_pending_args = std::move(other.m_pending_args);
        m_previous_cond = other.m_previous_cond.load();
        m_edge_mode = other.m_edge_mode;
    }
    return *this;
}

template<typename... Args>
void Signal<Args...>::_refreshState(void) {
    bool ready = m_condition != nullptr && m_count > 0;
    m_active.store(ready);
}

template<typename... Args>
void Signal<Args...>::connect(void (*func)(Args...)) {
    m_connected_funcs.emplace_back(func);
    m_count++;
    _refreshState();
}

template<typename... Args>
void Signal<Args...>::disconnect(void (*func)(Args...)) {
    auto it = std::find(m_connected_funcs.begin(), m_connected_funcs.end(), func);
    if (it != m_connected_funcs.end()) {
        m_connected_funcs.erase(it);
        m_count--;
        _refreshState();
    } else rmk_dynamicAssert(rmk::SignalError, error::signal::function_unexist);
}

template<typename... Args>
void Signal<Args...>::connectOnce(void (*func)(Args...)) {
    m_once_funcs.emplace_back(func);
    m_count++;
    _refreshState();
}

template<typename... Args>
void Signal<Args...>::connect(Slot& slot) {
    m_connected_slots.emplace_back(&slot);
    m_count++;
    _refreshState();
}

template<typename... Args>
void Signal<Args...>::disconnect(Slot& slot) {
    auto it = std::find(m_connected_slots.begin(), m_connected_slots.end(), &slot);
    if (it != m_connected_slots.end()) {
        m_connected_slots.erase(it);
        m_count--;
        _refreshState();
    } else rmk_dynamicAssert(rmk::SignalError, error::signal::slot_unexist);
}

template<typename... Args>
void Signal<Args...>::connectOnce(Slot& slot) {
    m_once_slots.emplace_back(&slot);
    m_count++;
    _refreshState();
}

template<typename... Args>
void Signal<Args...>::join(Slot&& slot) {
    m_joined_slots.emplace_back(std::move(slot));
    m_count++;
    _refreshState();
}

template<typename... Args>
void Signal<Args...>::joinOnce(Slot&& slot) {
    if (!slot) rmk_dynamicAssert(rmk::SignalError, error::signal::invalid_slot);
    m_joined_once_slots.emplace_back(std::move(slot));
    m_count++;
    _refreshState();
}

template<typename... Args>
void Signal<Args...>::joinPriority(Slot&& slot) {
    m_joined_prioritary_slots.emplace_back(std::move(slot));
    m_count++;
    _refreshState();
}

template<typename... Args>
void Signal<Args...>::connectPriority(void (*func)(Args...)) {
    m_prioritary_funcs.emplace_back(func);
    m_count++;
    _refreshState();
}

template<typename... Args>
void Signal<Args...>::disconnectPriority(void (*func)(Args...)) {
    auto it = std::find(m_prioritary_funcs.begin(), m_prioritary_funcs.end(), func);
    if (it != m_prioritary_funcs.end()) {
        m_prioritary_funcs.erase(it);
        m_count--;
        _refreshState();
    } else rmk_dynamicAssert(rmk::SignalError, error::signal::function_unexist);
}

template<typename... Args>
void Signal<Args...>::connectPriority(Slot& slot) {
    m_prioritary_slots.emplace_back(&slot);
    m_count++;
    _refreshState();
}

template<typename... Args>
void Signal<Args...>::disconnectPriority(Slot& slot) {
    auto it = std::find(m_prioritary_slots.begin(), m_prioritary_slots.end(), &slot);
    if (it != m_prioritary_slots.end()) {
        m_prioritary_slots.erase(it);
        m_count--;
        _refreshState();
    } else rmk_dynamicAssert(rmk::SignalError, error::signal::slot_unexist);
}

template<typename... Args>
void Signal<Args...>::unablePriority(void (*func)(Args...)) {
    auto it = std::find(m_connected_funcs.begin(), m_connected_funcs.end(), func);
    if (it != m_connected_funcs.end()) {
        m_prioritary_funcs.emplace_back(*it);
        m_connected_funcs.erase(it);
    } else rmk_dynamicAssert(rmk::SignalError, error::signal::function_unexist);
}

template<typename... Args>
void Signal<Args...>::unablePriority(Slot& slot) {
    auto it = std::find(m_connected_slots.begin(), m_connected_slots.end(), &slot);
    if (it != m_connected_slots.end()) {
        m_prioritary_slots.emplace_back(*it);
        m_connected_slots.erase(it);
    } else rmk_dynamicAssert(rmk::SignalError, error::signal::slot_unexist);
}

template<typename... Args>
void Signal<Args...>::disablePriority(void (*func)(Args...)) {
    auto it = std::find(m_prioritary_funcs.begin(), m_prioritary_funcs.end(), func);
    if (it != m_prioritary_funcs.end()) {
        m_connected_funcs.emplace_back(*it);
        m_prioritary_funcs.erase(it);
    } else rmk_dynamicAssert(rmk::SignalError, error::signal::function_unexist);
}

template<typename... Args>
void Signal<Args...>::disablePriority(Slot& slot) {
    auto it = std::find(m_prioritary_slots.begin(), m_prioritary_slots.end(), &slot);
    if (it != m_prioritary_slots.end()) {
        m_connected_slots.emplace_back(*it);
        m_prioritary_slots.erase(it);
    } else rmk_dynamicAssert(rmk::SignalError, error::signal::slot_unexist);
}

template<typename... Args>
void Signal<Args...>::emit(Args... args) {
    std::vector<void (*)(Args...)> prior_funcs, onc_funcs, conn_funcs;
    std::vector<Slot*>             prior_slots, onc_slots, conn_slots;
    std::vector<Slot>              prior_snapshot, onc_snapshot, joined_snapshot;
    
	prior_funcs     = m_prioritary_funcs;
	onc_funcs       = std::move(m_once_funcs);
	conn_funcs      = m_connected_funcs;
	prior_slots     = m_prioritary_slots;
	onc_slots       = std::move(m_once_slots);
	conn_slots      = m_connected_slots;
	prior_snapshot  = m_joined_prioritary_slots;
	onc_snapshot    = std::move(m_joined_once_slots);
	joined_snapshot = m_joined_slots;
    
    for (auto* f : prior_slots)         (*f)(args...);
    for (auto& f : prior_snapshot)      f(args...);
    for (auto  f : prior_funcs)         (*f)(args...);
    for (auto  f : onc_slots)           (*f)(args...);
    for (auto  f : onc_snapshot)        f(args...);
    for (auto  f : onc_funcs)           (*f)(args...);
    for (auto* f : conn_slots)          (*f)(args...);
    for (auto& f : joined_snapshot)     f(args...);
    for (auto  f : conn_funcs)          (*f)(args...);

	m_count -= onc_funcs.size() + onc_slots.size() + onc_snapshot.size();
}

template<typename... Args>
void Signal<Args...>::bind(Condition cond, Args... defaultArgs) {
    m_condition    = std::move(cond);
    m_default_args = [defaultArgs...]() {
        return std::make_tuple(defaultArgs...);
    };
    m_edge_mode = EdgeMode::level;
    m_previous_cond.store(false);
    _refreshState();
    signalManager.registerUser(*this);
}


template<typename... Args>
void Signal<Args...>::bindRising(Condition cond, Args... defaultArgs) {
    m_condition    = std::move(cond);
    m_default_args = [defaultArgs...]() {
        return std::make_tuple(defaultArgs...);
    };
    m_edge_mode = EdgeMode::rising;
    m_previous_cond.store(false);
    _refreshState();
    signalManager.registerUser(*this);
    
}

template<typename... Args>
void Signal<Args...>::bindFalling(Condition cond, Args... defaultArgs) {
    m_condition    = std::move(cond);
    m_default_args = [defaultArgs...]() {
        return std::make_tuple(defaultArgs...);
    };
    m_edge_mode = EdgeMode::falling;
    m_previous_cond.store(false);
    _refreshState();
    signalManager.registerUser(*this);
}

template<typename... Args>
void Signal<Args...>::bindChange(Condition cond, Args... defaultArgs) {
    m_condition    = std::move(cond);
    m_default_args = [defaultArgs...]() {
        return std::make_tuple(defaultArgs...);
    };
    m_edge_mode = EdgeMode::both;
    m_previous_cond.store(false);
    _refreshState();
    signalManager.registerUser(*this);
}

template<typename... Args>
void Signal<Args...>::start(void) {
    m_active.store(true);
}

template<typename... Args>
void Signal<Args...>::stop(void) {
    m_active.store(false);
}

template<typename... Args>
int Signal<Args...>::count(void) const {
    return m_count;
}

template<typename... Args>
void Signal<Args...>::reserve(int size) {
    m_connected_funcs.reserve(size);
    m_prioritary_funcs.reserve(size);
    m_connected_slots.reserve(size);
    m_prioritary_slots.reserve(size);
    m_joined_slots.reserve(size);
    m_once_slots.reserve(size);
}

template<typename... Args>
void Signal<Args...>::_evaluate(Args... args) {
    
    if (!m_active.load()) return;
              
    bool cond = m_condition && m_condition();
    bool prev = m_previous_cond.load();
    bool should_emit = false;

    switch (m_edge_mode) {
        case EdgeMode::level:   should_emit = cond; break;
        case EdgeMode::rising:  should_emit = cond && !prev; break;
        case EdgeMode::falling: should_emit = !cond && prev; break;
        case EdgeMode::both:    should_emit = cond != prev; break;
    }
    if (should_emit) {
        m_pending_args = std::make_tuple(args...);
        m_needs_emit.store(true);
    }
    m_previous_cond.store(cond);
}

template<typename... Args>
void Signal<Args...>::_dispatch(void) {
    if (!m_needs_emit.load()) return;
    m_needs_emit.store(false);
    std::apply([this](Args... a) { emit(a...); }, m_pending_args);
}

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
        signalManager.registerDispatchOnly(*this);
        m_registered = true;
    }
}

template<typename... Args>
void _EventSignal<Args...>::_setScancode(SDL_Scancode sc) noexcept {
    m_scancode = sc;
}

template<typename... Args>
void _EventSignal<Args...>::_setButton(SDL_GameControllerButton b) noexcept {
    m_button = b;
}

template<typename... Args>
bool _EventSignal<Args...>::isActive(void) const noexcept {
    if (m_scancode != SDL_SCANCODE_UNKNOWN)
        return SDL_GetKeyboardState(nullptr)[m_scancode];
    if (m_button != SDL_CONTROLLER_BUTTON_INVALID) {
        for (int i = 0; i < SDL_NumJoysticks(); i++) {
            SDL_JoystickID id = SDL_JoystickGetDeviceInstanceID(i);
            SDL_GameController* ctrl = _getOpenController(id);
            if (ctrl && SDL_GameControllerGetButton(ctrl, m_button))
                return true;
        }
    }
    return false;
}

} // namespace rmk

#endif