#ifndef REMAKE2D_LOCK_
#define REMAKE2D_LOCK_

#include <mutex>
#include <shared_mutex>


#define rmk_lockedPause(lock) \
do { \
        lock.unlock(); \
        co_yield rmk::_pause_t{}; \
        lock.relock(); \
} while(0)


namespace rmk {

class PauseGuard {
private:
    std::mutex& m_mtx;
    bool        m_locked{false};

public:
    explicit PauseGuard(std::mutex& mtx) : m_mtx(mtx) {
        m_mtx.lock();
        m_locked = true;
    }

    void unlock(void) noexcept {
        if (m_locked) {
            m_mtx.unlock();
            m_locked = false;
        }
    }

    void relock(void) noexcept {
        if (!m_locked) {
            m_mtx.lock();
            m_locked = true;
        }
    }

    ~PauseGuard(void) {
        if (m_locked) m_mtx.unlock();
    }

    PauseGuard(const PauseGuard&)            = delete;
    PauseGuard& operator=(const PauseGuard&) = delete;
};

class SharedPause {
private:
    std::shared_mutex& m_mtx;
    bool               m_locked{false};

public:
    explicit SharedPause(std::shared_mutex& mtx) : m_mtx(mtx) {
        m_mtx.lock_shared();
        m_locked = true;
    }

    void unlock(void) noexcept {
        if (m_locked) {
            m_mtx.unlock_shared();
            m_locked = false;
        }
    }

    void relock(void) noexcept {
        if (!m_locked) {
            m_mtx.lock_shared();
            m_locked = true;
        }
    }

    ~SharedPause(void) {
        if (m_locked) m_mtx.unlock_shared();
    }

    SharedPause(const SharedPause&)            = delete;
    SharedPause& operator=(const SharedPause&) = delete;
};

} // namespace rmk
#endif