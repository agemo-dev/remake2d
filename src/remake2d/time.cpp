#include <atomic>
#include <remake2d/time.hpp>
#include <remake2d/error.hpp>
#include <remake2d/system.hpp>
#include <remake2d/utility.hpp>
#include <remake2d/croutine.hpp>

#include <map>
#include <mutex>
#include <time.h>
#include <string>
#include <stdio.h>
#include <signal.h>
#include <algorithm>


namespace rmk {

constexpr usize RESERVED_TIMER = 4;

Task _timerCounter(void) noexcept {
    std::mutex mut;

    while (timer.m_running.load(std::memory_order_relaxed)) {
        dlink.on_count.store(true, std::memory_order_release);
        for (auto& t : timer.m_timers) {

            if (!t->m_active.load(std::memory_order_acquire) || t->m_elapsed.load(std::memory_order_relaxed)) continue;
            t->m_current_time.fetch_add(delta.tick(), std::memory_order_relaxed);

            if (t->m_current_time.load(std::memory_order_relaxed) >= t->m_limit.load(std::memory_order_relaxed)) {
                t->m_elapsed.store(true, std::memory_order_release);
                t->m_active.store(false, std::memory_order_relaxed);

                if(t->m_repeat.load(std::memory_order_relaxed)) t->start();
                std::lock_guard<std::mutex> lock(mut);
                t->_evaluate();
            }
        }
        dlink.on_count.store(false, std::memory_order_release);
        rmk_pause();
    }
}

namespace time {
void delay(time::Second delay) noexcept {
    std::this_thread::sleep_for(delay);
}
} //namespace time

void Chronometer::_calculatePauseTime(void) noexcept {
    auto temp = m_resume_point - m_pause_point;
    fmax time = static_cast<fmax>(
        std::chrono::duration_cast<time::Millisecond>(temp).count()
    );
    m_pause_time = m_pause_time + time;
}

void Chronometer::_reset(void) noexcept {
    m_start_point = t_point{};
    m_end_point   = t_point{};
    m_pause_time  = 0.0;
    m_resumed = false;
    m_stopped = false;
    m_started = false;
    m_paused  = false;
}

void Chronometer::_stop(void) noexcept {
    m_stopped = true;
    m_end_point = std::chrono::steady_clock::now();
}

void Chronometer::_resume(void) noexcept {
    m_resumed = true;
    m_resume_point = std::chrono::steady_clock::now();
    _calculatePauseTime();
}

void Chronometer::start(void) noexcept {
    _reset();
    m_started = true;
    m_start_point = std::chrono::steady_clock::now();
}

void Chronometer::pause(void) noexcept {
    if (!m_started || m_paused) return;
    m_paused = true;
    m_resumed = false;
    m_pause_point = std::chrono::steady_clock::now();
}

void Chronometer::resume(void) noexcept {
    if (!m_paused) return;
    m_paused= false;
    _resume();
}

void Chronometer::stop(void) noexcept {
    if (!m_started) return;
    _stop();
}

void Chronometer::reset(void) noexcept {
    _reset();
}


DeltaThreadConnector& DeltaThreadConnector::getInstance(void) {
    static DeltaThreadConnector instance;
    return instance;
}

void DeltaThreadConnector::resumeCroutines(void) {
    timer.m_routine.resume();
    signalManager.m_routine_user.resume();
}

DeltaTime::DeltaTime(void) {
    m_start_point = std::chrono::steady_clock::now();
}

fmax DeltaTime::tick(void) const noexcept {
    return m_frame_time.load(std::memory_order_relaxed);
}

fmax DeltaTime::FPS(void) const noexcept {
    fmax t = m_frame_time.load(std::memory_order_relaxed);
    if(!t) return 0.0;
    return 1.0 / t;
}

fmax DeltaTime::maxFPS(void) const noexcept {
    return m_max_fps;
}

void DeltaTime::maxFPS(fmax limit) noexcept {
    m_max_fps = limit != nil ? std::clamp(limit, (fmax)fps::min, (fmax)fps::max) : -1;
}

void DeltaTime::update(void) {
    signalManager.dispatch();

    std::unique_lock<std::shared_mutex> lock(dlink.mtx);
    if (dlink.on_count.load(std::memory_order_acquire)) dlink.cv.wait(lock, []() {
        return dlink.count_ended.load(std::memory_order_acquire);
    });

    m_end_point = std::chrono::steady_clock::now();
    auto wtimer = count<time::Microsecond>();
    m_frame_time.store(wtimer / 1'000'000.0L, std::memory_order_relaxed);
    m_start_point = std::chrono::steady_clock::now();

    if (m_max_fps < 0) {
        std::this_thread::sleep_until(
            m_start_point + time::Millisecond(umax((1.0 / m_max_fps) * 1'000.0))
        );
    }

    dlink.resumeCroutines();
}

DeltaTime& DeltaTime::getInstance(void) {
    static DeltaTime instance;
    return instance;
}

Timer::Timer(void) {
    rmk::system._init();
    timer._registerTimer(this);
    if(timer.m_routine_started.load(std::memory_order_acquire)) return;
    timer.m_routine.run();
    timer.m_routine_started.store(true, std::memory_order_release);
}

Timer::Timer(fmax l) : m_limit(l < 0 ? 0 : l) {
    rmk::system._init();
    timer._registerTimer(this);
    if(timer.m_routine_started.load(std::memory_order_acquire)) return;
    timer.m_routine.run();
    timer.m_routine_started.store(true, std::memory_order_release);
}

Timer::Timer(time::Second l) : Timer(l.count()) {}

Timer::Timer(Timer&& other) noexcept
    : Trackable(std::move(other))
    , m_limit(other.m_limit.load(std::memory_order_relaxed))
    , m_active(other.m_active.load(std::memory_order_relaxed))
    , m_elapsed(other.m_elapsed.load(std::memory_order_relaxed))
    , m_current_time(other.m_current_time.load(std::memory_order_relaxed))
    , onTimeout(other.onTimeout)
{
    timer._registerTimer(this);
}

Timer::Timer(const Timer& other)
    : Trackable(other)
    , m_limit(other.m_limit.load(std::memory_order_relaxed))
    , m_active(other.m_active.load(std::memory_order_relaxed))
    , m_elapsed(other.m_elapsed.load(std::memory_order_relaxed))
    , m_current_time(other.m_current_time.load(std::memory_order_relaxed))
    , onTimeout(other.onTimeout)
{
    timer._registerTimer(this);
}

Timer& Timer::operator=(Timer&& other) noexcept {
    if (this != &other) {
        Trackable::operator=(std::move(other));
        m_limit.store(other.m_limit.load(std::memory_order_relaxed), std::memory_order_relaxed);
        m_active.store(other.m_active.load(std::memory_order_relaxed), std::memory_order_relaxed);
        m_elapsed.store(other.m_elapsed.load(std::memory_order_relaxed), std::memory_order_relaxed);
        m_current_time.store(other.m_current_time.load(std::memory_order_relaxed), std::memory_order_relaxed);
    }
    timer._registerTimer(this);
    return *this;
}

Timer& Timer::operator=(const Timer& other) {
    if (this != &other) {
        m_limit.store(other.m_limit.load(std::memory_order_relaxed), std::memory_order_relaxed);
        m_active.store(other.m_active.load(std::memory_order_relaxed), std::memory_order_relaxed);
        m_elapsed.store(other.m_elapsed.load(std::memory_order_relaxed), std::memory_order_relaxed);
        m_current_time.store(other.m_current_time.load(std::memory_order_relaxed), std::memory_order_relaxed);
    }
    timer._registerTimer(this);
    return *this;
}

fmax Timer::limit(void) const noexcept {
    return m_limit.load(std::memory_order_relaxed);
}

void Timer::limit(fmax l) noexcept {
    m_limit.store(l < 0 ? 0 : l, std::memory_order_relaxed);
}

void Timer::limit(time::Second l) noexcept {
    fmax c = l.count();
    m_limit.store(c < 0 ? 0 : c, std::memory_order_relaxed);
}

void Timer::start(void) noexcept {
    m_current_time.store(0.0, std::memory_order_relaxed);
    m_elapsed.store(false, std::memory_order_relaxed);
    m_active.store(true, std::memory_order_release);
}

void Timer::stop(void) noexcept {
    m_current_time.store(0.0, std::memory_order_relaxed);
    m_active.store(false, std::memory_order_relaxed);
}

void Timer::pause(void) noexcept {
    m_active.store(false, std::memory_order_relaxed);
}

void Timer::resume(void) noexcept {
    m_active.store(true, std::memory_order_relaxed);
}

void Timer::repeat(bool stat) noexcept {
    m_repeat.store(stat, std::memory_order_relaxed);
}

bool Timer::active(void) const noexcept {
    return m_active.load(std::memory_order_relaxed);
}

bool Timer::elapsed(void) const noexcept {
    return m_elapsed.load(std::memory_order_acquire);
}

fmax Timer::elapsedTime(void) const noexcept {
    return m_current_time.load(std::memory_order_relaxed);
}

void Timer::_evaluate (void) noexcept {
    onTimeout._evaluate();
}

Timer::~Timer(void) {
    timer._unregisterTimer(this);
    rmk::system._quit();
}

TimerManager::TimerManager(void) {
    m_timers.reserve(RESERVED_TIMER);
    m_routine.load(_timerCounter);
}

void TimerManager::_registerTimer(Timer* t) noexcept {
    auto& vec = m_timers;
    auto it   = std::find(vec.begin(), vec.end(), t->tracker());
    if(it == m_timers.end()) m_timers.push_back(t->tracker());
}

void TimerManager::_unregisterTimer(Timer* t) noexcept {
    auto& vec = m_timers;
    auto it   = std::find(vec.begin(), vec.end(), t->tracker());
    if(it != vec.end()) vec.erase(it);
}

TimerManager& TimerManager::getInstance(void) noexcept {
    static TimerManager instance;
    return instance;
}

TimerManager::~TimerManager(void) {
    m_running.store(false, std::memory_order_relaxed);
    m_routine.stop();
}

} // namespace rmk