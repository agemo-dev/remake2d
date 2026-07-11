#ifndef REMAKE2D_TIME_
#define REMAKE2D_TIME_

#include <remake2d/config/forward.hpp>
#include <remake2d/signal.hpp>
#include <remake2d/numeric.hpp>

#include <map>
#include <atomic>
#include <chrono>
#include <string>
#include <algorithm>
#include <shared_mutex>
#include <condition_variable>


namespace rmk {

namespace time {

using Nanosecond  = std::chrono::duration<fmax, std::nano>;
using Microsecond = std::chrono::duration<fmax, std::micro>;
using Millisecond = std::chrono::duration<fmax, std::milli>;
using Second      = std::chrono::duration<fmax, std::ratio<1>>;
using Minute      = std::chrono::duration<fmax, std::ratio<60>>;
using Hour        = std::chrono::duration<fmax, std::ratio<3600>>;
using Day         = std::chrono::duration<fmax, std::ratio<86400>>;
using Week        = std::chrono::duration<fmax, std::ratio<604800>>;
using Month       = std::chrono::duration<fmax, std::ratio<2629800>>;   // 30.44 days
using Year        = std::chrono::duration<fmax, std::ratio<31557600>>;  // 365.25 days

void delay(time::Second) noexcept;
} // namespace time


class BaseClock {
protected:
  using t_point    = std::chrono::time_point<std::chrono::steady_clock>;
  using t_duration = std::chrono::steady_clock::duration;

  t_point            m_start_point{};
  t_point            m_end_point{};

public:
  template <typename D = time::Second> fmax count(void) const noexcept;

  virtual ~BaseClock(void) = default;
};


class Chronometer : public BaseClock {
private:
    bool        m_resumed{false};
    bool        m_stopped{false};
    bool        m_started{false};
    bool        m_paused{false};
	t_point     m_pause_point{};
	t_point     m_resume_point{};
	fmax        m_pause_time{0.0};
	
private:
    void _reset(void)			   noexcept;
    void _stop(void)			   noexcept;
    void _resume(void)			   noexcept;
    void _calculatePauseTime(void) noexcept;

public:
  Chronometer(void)                             = default;
  Chronometer(Chronometer&&)                    = default;
  Chronometer(const Chronometer&)               = default;
  Chronometer& operator=(Chronometer&&)         = default;
  Chronometer& operator=(const Chronometer&)    = default;

public:
    void stop(void)		noexcept;
    void start(void)	noexcept;
    void pause(void)	noexcept;
    void reset(void)	noexcept;
    void resume(void)	noexcept;

public:
	template <typename D = time::Second> 
	fmax elapsedTime(void) noexcept;
};


struct DeltaThreadConnector {
public:
	static DeltaThreadConnector& getInstance(void);

private:
    std::condition_variable_any cv;
    std::shared_mutex    mtx;
    std::atomic<bool>    count_ended{true};
    std::atomic<bool>    on_count{false};

private:
    DeltaThreadConnector(void) = default;
    DeltaThreadConnector(const DeltaThreadConnector&) 			 = delete;
    DeltaThreadConnector& operator=(const DeltaThreadConnector&) = delete;

private:
    void resumeCroutines(void);

private:
	friend class DeltaTime;
	friend class TimerManager;
};

inline auto& dlink = DeltaThreadConnector::getInstance();


class DeltaTime : public BaseClock {
private:
    fmax              m_max_fps{120};
    std::atomic<fmax> m_frame_time{0.0};

private:
	DeltaTime(void);
	DeltaTime(const DeltaTime&) 			= delete;
	DeltaTime& operator=(const DeltaTime&)	= delete;

public:
	fmax FPS(void) 	  const noexcept;
	fmax tick(void)   const noexcept;
	fmax maxFPS(void) const noexcept;

public:
    void update(void);
    void maxFPS(fmax) noexcept;
	static DeltaTime& getInstance(void);
};

inline DeltaTime& delta = DeltaTime::getInstance();


class Timer {
private:
	std::atomic<fmax>  m_limit{0.0};
	std::atomic<bool>  m_active{false};
	std::atomic<bool>  m_repeat{false};
	std::atomic<bool>  m_elapsed{false};
	std::atomic<fmax>  m_current_time{0.0};

public:
	_TimerSignal<> onTimeout;

public:
	Timer(void);
	Timer(fmax);
	Timer(time::Second);
	Timer(Timer&&);
	Timer(const Timer&);
	Timer& operator=(Timer&&);
	Timer& operator=(const Timer&);

public:
	void limit(fmax)  		  noexcept;
	void limit(time::Second)  noexcept;

public:
	void start(void)  noexcept;
	void stop(void)   noexcept;
	void pause(void)  noexcept;
	void resume(void) noexcept;
	void repeat(bool) noexcept;

public:
	fmax limit(void)       const noexcept;
	bool isActive(void)    const noexcept;
	bool isElapsed(void)   const noexcept;
	fmax elapsedTime(void) const noexcept;

public:
	~Timer(void);

private:
	friend class TimerManager;
};

class TimerManager {
private:
	std::atomic<bool>		m_routine_started{false};
	std::atomic<bool>		m_running{true};
	Croutine<>				m_routine;
    std::vector<Timer*>		m_timers;

private:
	TimerManager(void);
	TimerManager(const TimerManager&) 				= delete;
	TimerManager& operator=(const TimerManager&)	= delete;

private:
	void _registerTimer(Timer*)   noexcept;
	void _unregisterTimer(Timer*) noexcept;
	rmk::Task _timerCounter(void) noexcept;

public:
	static TimerManager& getInstance(void) noexcept;

public:
	~TimerManager(void);
	
private:
	friend class Timer;
	friend struct DeltaThreadConnector;
};

inline TimerManager& timer = TimerManager::getInstance();

enum class fps : u16 {
    min = 30,
    max = 1440
};

} // namespace rmk

#include <remake2d/template/time.tpp>

#endif