#ifndef REMAKE2D_ESIGNAL_
#define REMAKE2D_ESIGNAL_

namespace rmk {

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
    IVector<i32, 2> m_scancodes;
    i32             m_button{-1};

private:
    _EventSignal(void) = default;
    using _EngineSignal<Args...>::_EngineSignal;
    using _EngineSignal<Args...>::operator=;

public:
    bool isActive(void) const noexcept;

private:
    void _setButton(i32)   noexcept;
    void _setScancode(i32) noexcept;
    void _setScancode(std::initializer_list<i32>) noexcept;

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

#include <remake2d/template/private/esignal.tpp>

#endif