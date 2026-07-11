#ifndef REMAKE2D_SIGNAL_
#define REMAKE2D_SIGNAL_

#include <remake2d/lock.hpp>
#include <remake2d/error.hpp>
#include <remake2d/croutine.hpp>

#if __has_include(<SDL2/SDL.h>)
    #include <SDL2/SDL.h>
#elif __has_include(<SDL.h>)
    #include <SDL.h>
#else
    #error "SDL not found."
#endif

#include <vector>
#include <mutex>
#include <atomic>
#include <tuple>
#include <utility>
#include <optional>
#include <algorithm>
#include <functional>


namespace rmk {

class TimerManager;
class EventManager;
class PhysicBody;
template<typename...> class Signal;
	
SDL_GameController* _getOpenController(i32) noexcept;

enum class EdgeMode : u8 {
    level,
    rising,
    falling,
    both
};


class SignalManager {
private:
    std::vector<std::function<void()>> m_slot_user;
    Croutine<>                         m_routine_user;
    std::atomic<bool>                  m_started_user{false};

    std::vector<std::function<void()>> m_all_dispatch;
    mutable std::mutex                 m_mtx;

private:
    SignalManager(void);
    Task _runUser(void);

    template<typename... Args>
    void _register(std::vector<std::function<void()>>&, Croutine<>&, std::atomic<bool>&, Signal<Args...>&, std::function<Task()>);

public:
    void dispatch(void);
    static SignalManager& getInstance(void);
    template<typename... Args> void registerUser(Signal<Args...>&);
    template<typename... Args> void registerDispatchOnly(Signal<Args...>&);

public:
    friend class DeltaThreadConnector;
};

inline SignalManager& signalManager = SignalManager::getInstance();

class SignalBase {
protected:
    virtual void _dispatch(void)     = 0;
    virtual void _refreshState(void) = 0;
    
private:
    friend class SignalManager;
    friend class DeltaThreadConnector;
    template<typename...> friend class Croutine;
};

template<typename... Args>
class Signal : public SignalBase {
public:
    using Slot      = std::function<void(Args...)>;
    using Condition = std::function<bool(void)>;

protected:
    std::vector<void (*)(Args...)> m_once_funcs;
    std::vector<void (*)(Args...)> m_connected_funcs;
    std::vector<void (*)(Args...)> m_prioritary_funcs;
    std::vector<Slot*>             m_once_slots;
    std::vector<Slot*>             m_connected_slots;
    std::vector<Slot*>             m_prioritary_slots;
    std::vector<Slot>              m_joined_slots;
    std::vector<Slot>              m_joined_once_slots;
    std::vector<Slot>              m_joined_prioritary_slots;
    i32                            m_count{0};

    Condition                            m_condition;
    std::function<std::tuple<Args...>()> m_default_args;
    std::atomic<bool>                    m_active{false};
    std::atomic<bool>                    m_needs_emit{false};
    std::tuple<Args...>                  m_pending_args;
    std::atomic<bool>                    m_previous_cond{false};
    EdgeMode                             m_edge_mode{EdgeMode::level};

public:
    Signal(void) = default;
	
public:
    Signal(const Signal&);
    Signal(Signal&&) noexcept;
    Signal& operator=(const Signal&);
    Signal& operator=(Signal&&) noexcept;

public:
    void connect(void (*)(Args...));
    void disconnect(void (*)(Args...));
    void connectOnce(void (*)(Args...));
    void connect(Slot&);
    void disconnect(Slot&);
    void connectOnce(Slot&);
    void connect(Slot&&)    = delete;
    void disconnect(Slot&&) = delete;

    void join(Slot&&);
    void joinOnce(Slot&&);
    void joinPriority(Slot&&);

    void connectPriority(void (*)(Args...));
    void disconnectPriority(void (*)(Args...));
    void connectPriority(Slot&);
    void disconnectPriority(Slot&);
    void connectPriority(Slot&&)    = delete;
    void disconnectPriority(Slot&&) = delete;

    void unablePriority(void (*)(Args...));
    void unablePriority(Slot&);
    void disablePriority(void (*)(Args...));
	
    void disablePriority(Slot&);
    void unablePriority(Slot&&)  = delete;
    void disablePriority(Slot&&) = delete;

    void emit(Args...);

    void bind(Condition, Args...);
    void bindRising(Condition, Args...);
    void bindFalling(Condition, Args...);
    void bindChange(Condition, Args...);

    void stop(void);
    void start(void);
    int count(void) const;
    void reserve(int size);

protected:
    void _dispatch(void) override;
    virtual void _evaluate(Args...);
    virtual void _refreshState(void) override;

private:
    friend class SignalManager;
    friend class DeltaThreadConnector;
    template<typename...> friend class Croutine;

public:
    virtual ~Signal(void) = default;
};

template<typename... Args>
class _EngineSignal : public Signal<Args...> {
protected:
    bool    m_registered{false};

protected:
    _EngineSignal(void) = default;
    using Signal<Args...>::Signal;
    using Signal<Args...>::operator=;
    
protected:
    void _evaluate(Args...)  override;
    void _refreshState(void) override;
    
public:
    virtual ~_EngineSignal(void) = default;
};

template<typename... Args>
class _TimerSignal : public _EngineSignal<Args...> {
private:
    _TimerSignal(void) = default;
    using _EngineSignal<Args...>::_EngineSignal;
    using _EngineSignal<Args...>::operator=;

    friend class Timer;
    friend class TimerManager;
    friend class SignalManager;
    friend class DeltaThreadConnector;
    template<typename...> friend class Croutine;
};

template<typename... Args>
class _EventSignal : public _EngineSignal<Args...> {
private:
    SDL_Scancode             m_scancode{SDL_SCANCODE_UNKNOWN};
    SDL_GameControllerButton m_button{SDL_CONTROLLER_BUTTON_INVALID};

private:
    _EventSignal(void) = default;
    using _EngineSignal<Args...>::_EngineSignal;
    using _EngineSignal<Args...>::operator=;

public:
    bool isActive(void) const noexcept;

private:
    void _setScancode(SDL_Scancode)           noexcept;
    void _setButton(SDL_GameControllerButton) noexcept;

    friend class EventManager;
    friend class SignalManager;
    friend class DeltaThreadConnector;
    template<typename...> friend class Croutine;
};

template<typename... Args>
class _PhysicSignal : public _EngineSignal<Args...> {
private:
    _PhysicSignal(void) = default;
    using _EngineSignal<Args...>::_EngineSignal;
    using _EngineSignal<Args...>::operator=;
    
private:
    friend class PhysicBody;
    friend class DynamicBody;
    friend class SignalManager;
    friend class PhysicManager;
    friend class DeltaThreadConnector;
    template<typename...> friend class Croutine;
};

} // namespace rmk

#include <remake2d/template/signal.tpp>

#endif