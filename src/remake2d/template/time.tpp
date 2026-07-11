#ifndef REMAKE2D_TIME_TPP_
#define REMAKE2D_TIME_TPP_

#include <SDL2/SDL.h>

namespace rmk {

template <typename D> fmax BaseClock::count(void) const noexcept {
    auto duration = std::chrono::duration_cast<D>(
		m_end_point - m_start_point
	);
    return duration.count();
}

template <typename D> fmax Chronometer::elapsedTime(void) noexcept {
    if (m_paused && !m_resumed)  _resume();
    if (m_started && !m_stopped) _stop();
	
    auto duration = std::chrono::duration_cast<D>(
		m_end_point - m_start_point
	);
    fmax c = static_cast<fmax>(duration.count());
	
    fmax pause_count = m_pause_time / 1000.0;
	
    c -= pause_count;
    return c;
}

} //namespace rmk

#endif