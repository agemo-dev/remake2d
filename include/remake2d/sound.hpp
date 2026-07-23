#ifndef REMAKE2D_SOUND_
#define REMAKE2D_SOUND_

#include <remake2d/signal.hpp>
#include <remake2d/system.hpp>
#include <remake2d/numeric.hpp>
#include <remake2d/config/config.hpp>
#include <remake2d/config/resource.hpp>

#include <array>
#include <memory>

#if __has_include(<SDL2/SDL_mixer.h>)
    #include <SDL2/SDL_mixer.h>
#elif __has_include(<SDL_mixer.h>)
    #include <SDL_mixer.h>
#else
    #error "SDL_mixer not found."
#endif


namespace rmk {

class Sound {

protected:
    u8      m_volume;
    i8      m_loops_remaining;
    bool    m_is_playing;

public:
    Signal<> onFinish;

protected:
    Sound(void) = default;

public:
    Sound(Sound&&)                  = default;
    Sound(const Sound&)             = default;
    Sound& operator=(Sound&&)       = default;
    Sound& operator=(const Sound&)  = default;
    
public:
    i8 playFor(void) const noexcept;
    u8 getVolume(void) const noexcept;

public:
    virtual void play(i8 = 0) = 0;
    virtual void stop(void)   = 0;
    virtual void pause(void)  = 0;
    virtual void resume(void) = 0;
    virtual void volume(u8) noexcept = 0;
    
public:
    virtual ~Sound(void) = default;
    
private:
    friend class System;
};

class Music : public Sound {
private:
    MUS 				 m_music;
    inline static Music *m_current_music;

public:
    Music(std::string_view, u8 = 64);
    Music(void)                     = delete;
    Music(Music&&)                  = default;
    Music(const Music&)             = default;
    Music& operator=(Music&&)       = default;
    Music& operator=(const Music&)  = default;
    
public:
    void play(i8 = 0)		 override;
    void stop(void)			 override;
    void pause(void)		 override;
    void resume(void)		 override;
    void volume(u8) noexcept override;

public:
    ~Music(void) override;
    
private:
    friend class System;
    friend void _hookFunc(void);
};

class SFX : public Sound {
private:
    CHK       m_sfx;
    u32       m_channel{0};

private:
    inline static std::queue<u32>                           m_free_channels;
    inline static std::array<SFX*, (usize)channel::max>     m_channel_owners;

public:
    SFX(std::string_view, u8 = 64);
    SFX(void)                   = delete;
    SFX(SFX&&)                  = default;
    SFX(const SFX&)             = default;
    SFX& operator=(SFX&&)       = default;
    SFX& operator=(const SFX&)  = default;

public:
    void stopAll(void);

public:
    void play(i8 = 0)        override;
    void stop(void) 		 override;
    void pause(void)	     override;
    void resume(void)        override;
    void volume(u8) noexcept override;

public:
    ~SFX(void) override;

private:
    friend class System;
    friend void _subHook(int);
    friend void config::sound::initQueue(void) noexcept;
};

enum class volume : u8 {
	min = 0,
	max = 128
};

} // namespace rmk
#endif