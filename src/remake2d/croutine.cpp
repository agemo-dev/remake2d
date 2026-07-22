#include <remake2d/croutine.hpp>

namespace rmk {

thread_local std::vector<CroutineBase*> _self_stack;

CroutineBase* self(void) noexcept {
    return _self_stack.empty() ? nullptr : _self_stack.back();
}

Task Task::promise_type::get_return_object(void) noexcept {
    return Task{std::coroutine_handle<Task::promise_type>::from_promise(*this)};
}
std::suspend_always Task::promise_type::initial_suspend(void) noexcept { return {}; }
std::suspend_always Task::promise_type::final_suspend(void)   noexcept { return {}; }
void Task::promise_type::return_void(void) noexcept {}
void Task::promise_type::unhandled_exception(void) { std::terminate(); }
std::suspend_always Task::promise_type::yield_value(_pause_t) noexcept { return {}; }

Task::Task(std::coroutine_handle<Task::promise_type> h) noexcept : handle(h) {}
Task::Task(Task&& o) noexcept : handle(std::exchange(o.handle, {})) {}
Task::~Task(void) { if (handle) handle.destroy(); }

ThreadWorker::ThreadWorker(void) {
    m_thread = std::jthread([this](std::stop_token token) {
        _loop(token);
    });
}

ThreadWorker::~ThreadWorker(void) {
    stop();
}

void ThreadWorker::submit(_CroutineEntry entry) {
    m_queue.push(entry);
    m_count.fetch_add(1);
    m_cv.notify_one();
}

void ThreadWorker::stop(void) {
    {
        std::lock_guard lock(m_mtx);
        m_running = false;
    }
    m_cv.notify_all();
    if (m_thread.joinable()) m_thread.join();
}

bool ThreadWorker::idle(void) {
    return m_queue.empty();
}

u32 ThreadWorker::count(void) const noexcept {
    return m_count.load();
}

u64 ThreadWorker::ID(void) const noexcept {
    return m_id;
}

void ThreadWorker::_loop(std::stop_token token) {
    while (true) {
        _CroutineEntry entry{};
        {
            std::unique_lock lock(m_mtx);
            const bool ready = m_cv.wait(lock, token, [this] {
                return !m_queue.empty() || !m_running;
            });
            if (!ready || !m_running || m_queue.empty()) return;
            entry = m_queue.front();
            m_queue.pop();
        }

        if (entry.handle && !entry.handle.done()) {
            if (entry.major_id) *entry.major_id = m_id;
            _self_stack.push_back(entry.owner);
            entry.handle.resume();
            _self_stack.pop_back();

            if (entry.handle.done()) {
                if (entry.running) *entry.running = false;
                if (entry.owner && entry.owner->m_continuation) {
                    croutinePool.submitHandle(
                        entry.owner->m_continuation,
                        entry.owner
                    );
                    entry.owner->m_continuation = {};
                }
            } else {
                if (entry.running) *entry.running = false;
            }
        }

        m_count.fetch_sub(1);
    }
}

CroutinePool::CroutinePool(void) {
    m_engine   = std::make_unique<ThreadWorker>();
    m_heavy    = std::make_unique<ThreadWorker>();
    m_max_user = std::max(1u, std::thread::hardware_concurrency());

    u32 initial = std::max(1u, m_max_user / 4u);
    for (u32 i = 0; i < initial; i++)
        m_user.emplace_back(std::make_unique<ThreadWorker>());
}

CroutinePool& CroutinePool::getInstance(void) {
    static CroutinePool instance;
    return instance;
}

void CroutinePool::submit(_CroutineEntry entry, croutine::priority p) {
    switch (p) {
        case croutine::priority::engine: m_engine->submit(entry); break;
        case croutine::priority::heavy:  m_heavy->submit(entry);  break;
        case croutine::priority::user:   _submitUser(entry);      break;
    }
}

void CroutinePool::submitHandle(std::coroutine_handle<> h, CroutineBase* owner) {
    _CroutineEntry entry{ h, owner, nullptr, nullptr };
    submit(entry, owner->m_priority);
}

void CroutinePool::_submitUser(_CroutineEntry entry) {
    std::lock_guard lock(m_user_mtx);

    ThreadWorker* best = nullptr;
    for (auto& w : m_user) {
        if (w->count() < CROUTINE_PER_THREAD) {
            if (!best || w->count() < best->count())
                best = w.get();
        }
    }

    if (!best && m_user.size() < m_max_user) {
        m_user.emplace_back(std::make_unique<ThreadWorker>());
        best = m_user.back().get();
    }

    if (!best) {
        best = m_user[0].get();
        for (auto& w : m_user)
            if (w->count() < best->count())
                best = w.get();
    }

    best->submit(entry);
}

} // namespace remake2d