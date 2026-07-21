#ifndef REMAKE2D_CROUTINE_
#define REMAKE2D_CROUTINE_

#include <remake2d/numeric.hpp>
#include <remake2d/error.hpp>
#include <remake2d/lock.hpp>

#include <condition_variable>
#include <functional>
#include <coroutine>
#include <algorithm>
#include <optional>
#include <concepts>
#include <utility>
#include <memory>
#include <thread>
#include <vector>
#include <queue>

#define rmk_pause()     co_yield rmk::_pause_t{}
#define rmk_await(coro) co_await (coro)

namespace rmk {

inline u64 _thread_id_counter{0};
inline u64 _croutine_id_counter{0};

constexpr u32 CROUTINE_PER_THREAD = 15;

namespace croutine {
    enum class priority : u8 { engine, heavy, user };
}

struct _pause_t {};

class CroutinePool;
class CroutineBase;

struct Task {

public:
    struct promise_type {
        void unhandled_exception(void);
        void return_void(void) 					  noexcept;
        Task get_return_object(void) 			  noexcept;
        std::suspend_always final_suspend(void)   noexcept;
        std::suspend_always initial_suspend(void) noexcept;
        std::suspend_always yield_value(_pause_t) noexcept;
    };

public:
    std::coroutine_handle<promise_type> handle;
    explicit Task(std::coroutine_handle<promise_type> h) noexcept;
    Task(Task&& o) noexcept;
    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;
    ~Task(void);
};

struct _CroutineEntry {
    std::coroutine_handle<> handle;
    CroutineBase*           owner{nullptr};
    u64*                    major_id{nullptr};
    bool*                   running{nullptr};
};

class CroutineBase {
public:
    std::coroutine_handle<> m_continuation{};
    croutine::priority      m_priority{croutine::priority::user};
    virtual void resume(void)        noexcept = 0;
    virtual void stop(void)          noexcept = 0;
    virtual void wait(void)          noexcept = 0;
    virtual bool isRunning(void)     noexcept = 0;
    virtual u64  ID(void)      const noexcept = 0;
    virtual u64  majorID(void) const noexcept = 0;

public:
    virtual ~CroutineBase(void) = default;
};

CroutineBase* self(void) noexcept;

class ThreadWorker {
private:
    std::jthread               m_thread;
    std::queue<_CroutineEntry> m_queue;
    std::mutex                 m_mtx;
    std::condition_variable    m_cv;
    bool   					   m_running{true};
    u64    			   		   m_id{_thread_id_counter += 1};
    std::atomic<u8>    		   m_count{0};
public:
    ThreadWorker(void);
    void submit(_CroutineEntry);
    bool idle(void);
    u32  count(void) const noexcept;
    u64  ID(void)    const noexcept;
    void stop(std::stop_token);
    ~ThreadWorker(void) = default;
    ThreadWorker(const ThreadWorker&) = delete;
    ThreadWorker& operator=(const ThreadWorker&) = delete;
private:
    void _loop(void);
};

class CroutinePool {
private:
    std::unique_ptr<ThreadWorker>              m_engine;
    std::unique_ptr<ThreadWorker>              m_heavy;
    std::vector<std::unique_ptr<ThreadWorker>> m_user;
    u32                m_max_user;
    mutable std::mutex m_user_mtx;
    CroutinePool(void);
    CroutinePool(const CroutinePool&) = delete;
    CroutinePool& operator=(const CroutinePool&) = delete;
public:
    static CroutinePool& getInstance(void);
    void submit(_CroutineEntry entry, croutine::priority p);
    void submitHandle(std::coroutine_handle<> h, CroutineBase* owner);
private:
    void _submitUser(_CroutineEntry entry);
};

inline CroutinePool& croutinePool = CroutinePool::getInstance();

template<typename Derived>
class CroutineImpl : public CroutineBase {
protected:
    std::optional<Task> m_task;
    bool   m_running{false};
    bool   m_stopped{false};
    u64    m_major_id{0};
    u64    m_coro_id{_croutine_id_counter += 1};

public:
    void stop(void) 		 noexcept override;
    void wait(void) 		 noexcept override;
    void resume(void) 		 noexcept override;
    bool isRunning(void)     noexcept override;
    u64  ID(void)      const noexcept override;
    u64  majorID(void) const noexcept override;

    void isHeavy(bool)  			  noexcept;
    void isEngine(bool) 			  noexcept;
    void priority(croutine::priority) noexcept;

    auto operator co_await(void) noexcept;

    ~CroutineImpl(void);

protected:
    _CroutineEntry _makeEntry(void) noexcept;
};

template<typename... Args>
class Croutine : public CroutineImpl<Croutine<Args...>> {
private:
    using FuncType = std::function<Task(Args...)>;
    FuncType m_function;

public:
    Croutine(void) = default;

    template<typename F>
    explicit Croutine(F&& function) : m_function(std::forward<F>(function)) {}

    Croutine(const Croutine&)            = delete;
    Croutine& operator=(const Croutine&) = delete;
    Croutine(Croutine&&)                 = delete;
    Croutine& operator=(Croutine&&)      = delete;

    void run(Args...);
    template<typename F> void load(F&&);
};

template<>
class Croutine<> : public CroutineImpl<Croutine<>> {
private:
    using FuncType = std::function<Task(void)>;
    FuncType m_function;

public:
    Croutine(void) = default;

    template<typename F>
    explicit Croutine(F&& function) : m_function(std::forward<F>(function)) {}

    Croutine(const Croutine&)            = delete;
    Croutine& operator=(const Croutine&) = delete;
    Croutine(Croutine&&)                 = delete;
    Croutine& operator=(Croutine&&)      = delete;

    void run(void);
    template<typename F> void load(F&&);
};

} // namespace rmk

#include <remake2d/template/croutine.tpp>

#endif