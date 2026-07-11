#ifndef REMAKE2D_CROUTINE_TPP_
#define REMAKE2D_CROUTINE_TPP_

namespace rmk {

template<typename Derived>
void CroutineImpl<Derived>::resume(void) noexcept {
    if (!m_task || m_task->handle.done() || m_stopped || m_running) return;
    m_running = true;
    croutinePool.submit(_makeEntry(), m_priority);
}

template<typename Derived>
void CroutineImpl<Derived>::stop(void) noexcept {
    m_stopped = true;
    m_running = false;
}

template<typename Derived>
void CroutineImpl<Derived>::wait(void) noexcept {
    while (m_running) std::this_thread::yield();
}

template<typename Derived>
bool CroutineImpl<Derived>::isRunning(void) noexcept { return m_running; }

template<typename Derived>
u64 CroutineImpl<Derived>::id(void) const noexcept { return m_coro_id; }

template<typename Derived>
u64 CroutineImpl<Derived>::majorID(void) const noexcept { return m_major_id; }

template<typename Derived>
void CroutineImpl<Derived>::priority(croutine::priority p) noexcept { m_priority = p; }

template<typename Derived>
void CroutineImpl<Derived>::isHeavy(bool v) noexcept {
    m_priority = v ? croutine::priority::heavy : croutine::priority::user;
}

template<typename Derived>
void CroutineImpl<Derived>::isEngine(bool v) noexcept {
    m_priority = v ? croutine::priority::engine : croutine::priority::user;
}

template<typename Derived>
auto CroutineImpl<Derived>::operator co_await(void) noexcept {
    struct Awaiter {
        Derived& coro;
        bool await_ready(void) noexcept { return !coro.isRunning(); }
        bool await_suspend(std::coroutine_handle<> h) noexcept {
            if (!coro.isRunning()) return false;
            coro.m_continuation = h;
            return true;
        }
        void await_resume(void) noexcept {}
    };
    return Awaiter{static_cast<Derived&>(*this)};
}

template<typename Derived>
CroutineImpl<Derived>::~CroutineImpl(void) { stop(); }

template<typename Derived>
_CroutineEntry CroutineImpl<Derived>::_makeEntry(void) noexcept {
    return { m_task->handle, this, &m_major_id, &m_running };
}


template<typename... Args>
template<typename F>
void Croutine<Args...>::load(F&& newFunc) {
    this->m_function = std::forward<F>(newFunc);
}

template<typename... Args>
void Croutine<Args...>::run(Args... args) {
    if (!m_function) throw CroutineError(error::croutine::any_function_linked);
    this->m_stopped = false;
    this->m_running = true;
    this->m_task.emplace(m_function(std::forward<Args>(args)...));
    croutinePool.submit(this->_makeEntry(), this->m_priority);
}


template<typename F>
void Croutine<>::load(F&& newFunc) {
    m_function = std::forward<F>(newFunc);
}

inline void Croutine<>::run(void) {
    if (!m_function) throw CroutineError(error::croutine::any_function_linked);
    m_stopped = false;
    m_running = true;
    m_task.emplace(m_function());
    croutinePool.submit(_makeEntry(), m_priority);
}

} // namespace rmk

#endif