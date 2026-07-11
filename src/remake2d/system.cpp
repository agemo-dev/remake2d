#include <remake2d/system.hpp>
#include <remake2d/error.hpp>
#include <remake2d/sound.hpp>

#include <cmath>
#include <chrono>
#include <time.h>
#include <sstream>

#if __has_include(<SDL2/SDL.h>)
    #include <SDL2/SDL.h>
	#include <SDL2/SDL_ttf.h>
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL_mixer.h>
#elif __has_include(<SDL.h>)
    #include <SDL.h>
	#include <SDL_ttf.h>
	#include <SDL_image.h>
	#include <SDL_mixer.h>
#else
    #error "SDL not found."
#endif


namespace rmk {

void System::Setup::scalingMode(std::string_view mode) {
    #ifdef SDL_HINT_RENDER_SCALE_QUALITY
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, std::string(mode).c_str());
    #endif
}

void System::Setup::backend(std::string_view bcknd) {
    #ifdef SDL_HINT_RENDER_DRIVER
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, std::string(bcknd).c_str());
    #endif
}

void System::Setup::audioMode(std::string_view mode) {
    #ifdef SDL_AUDIO_RESAMPLING_MODE
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    SDL_SetHint(SDL_AUDIO_RESAMPLING_MODE, std::string(mode).c_str());
    #endif
}

void System::Setup::audioCategory(std::string_view category) {
    #ifdef SDL_AUDIO_CATEGORY
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    SDL_SetHint(SDL_AUDIO_CATEGORY, std::string(category).c_str());
    #endif
}

void System::Setup::mouseRelativeMode(std::string_view mode) {
    #ifdef SDL_HINT_MOUSE_RELATIVE_MODE_WARP
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, std::string(mode).c_str());
    #endif
}

void System::Setup::mobileOrientation(std::span<std::string_view> orients) {
    #ifdef SDL_HINT_ORIENTATION
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    std::string flags;
    for(const auto& e : orients) flags += std::string(e) + " ";
    SDL_SetHint(SDL_HINT_ORIENTATION, flags.c_str());
    #endif
}


void System::Setup::allocateChannels(u16 channels) {
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    system.m_channel_count = std::clamp(channels, (u16)channel::min, (u16)channel::max);
}

void System::Toggle::vsync(bool statut) {
    #ifdef SDL_HINT_RENDER_VSYNC
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    std::string s = std::to_string((int)statut);
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, s.c_str());
    #endif
}

void System::Toggle::blockOnPause(bool statut) {
    #ifdef SDL_HINT_ANDROID_BLOCK_ON_PAUSE
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    std::string s = std::to_string((int)statut);
    SDL_SetHint(SDL_HINT_ANDROID_BLOCK_ON_PAUSE, s.c_str());
    #endif
}

void System::Toggle::hideHomeIndicator(bool statut) {
    #ifdef SDL_HINT_IOS_HIDE_HOME_INDICATOR
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    std::string s = std::to_string((int)statut);
    SDL_SetHint(SDL_HINT_IOS_HIDE_HOME_INDICATOR, s.c_str());
    #endif
}

void System::Toggle::relativeMouseMode(bool statut) {
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    system.m_relative_mouse_mode = statut ? SDL_TRUE : SDL_FALSE;
}

void System::Toggle::materialAcceleration(bool statut) {
    #ifdef SDL_HINT_FRAMEBUFFER_ACCELERATION
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    std::string s = std::to_string((int)statut);
    SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, s.c_str());
    #endif
}

void System::Toggle::gameController(bool statut) {
    #if defined(SDL_INIT_JOYSTICK) && defined(SDL_INIT_GAMECONTROLLER) && defined(SDL_INIT_HAPTIC)
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    auto flags = SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC;
    if (statut) SDL_InitSubSystem(flags);
    else SDL_QuitSubSystem(flags);
    #endif
}

void System::Toggle::accelerometerAsJoystick(bool statut) {
    #ifdef SDL_HINT_ACCELEROMETER_AS_JOYSTICK
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    std::string s = std::to_string((int)statut);
    SDL_SetHint(SDL_HINT_ACCELEROMETER_AS_JOYSTICK, s.c_str());
    #endif
}

u32 System::Info::ramMB(void) {
    return SDL_GetSystemRAM();
}

u8 System::Info::cpuCount(void) {
    return SDL_GetCPUCount();
}

std::string System::Info::platform(void) {
    return std::string(SDL_GetPlatform());
}

u8 System::Info::displayCount(void) {
    return SDL_GetNumVideoDisplays();
}

u16 System::Info::channelCount(void) {
    return system.m_channel_count;
}

Dim2d System::Info::screenSize(u8 screen) {
    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(screen, &mode);
    return { (f32)mode.w, (f32)mode.h };
}

Date System::Info::currentDay(void) {
    auto now = std::chrono::system_clock::now();
    auto ymd = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(now)
    };
     return Date{
        (u8)(unsigned)ymd.day(),
        (u8)(unsigned)ymd.month(),
        (i32)(int)ymd.year()
     };
 }

 Clock System::Info::currentTime(void) {
    auto now  = std::chrono::system_clock::now();
    auto secs = std::chrono::floor<std::chrono::seconds>(now);
    std::time_t t = std::chrono::system_clock::to_time_t(secs);
    std::tm* tm   = std::localtime(&t);
    return Clock{
        (u8)tm->tm_hour,
        (u8)tm->tm_min,
        (u8)tm->tm_sec
     };
}

System &System::getInstance(void) noexcept {
    static System sys;
    return sys;
}

void _hookFunc(void) {
    if(!rmk::Music::m_current_music) return;
    auto* m = rmk::Music::m_current_music;
    if(m->m_loops_remaining < 0) {
        return;
    } else if(m->m_loops_remaining > 0) {
        m->m_loops_remaining -= 1;
    } else {
        m->stop();
        m->onFinish.emit();
        rmk::Music::m_current_music = nullptr;
    }
}

 void _subHook(int channel) {
    if(channel >= (int)system.info.channelCount()) return;
    rmk::SFX* sfx = rmk::SFX::m_channel_owners[channel];
    if(!sfx) return;
    if(sfx->m_loops_remaining < 0) {
        return;
    } else if(sfx->m_loops_remaining > 0) {
        sfx->m_loops_remaining -= 1;
    } else {
        rmk::SFX::m_free_channels.push(channel);
        rmk::SFX::m_channel_owners[channel] = nullptr;
        sfx->m_is_playing = false;
        sfx->onFinish.emit();
    }
}

void System::_init(void) {
    config::system::initSDL();
    
    toggle.vsync(true);
    SDL_SetRelativeMouseMode(m_relative_mouse_mode);
	
    m_channel_count = m_channel_count ? m_channel_count : (u16)channel::mid;
    Mix_AllocateChannels((int)m_channel_count);
    
    Mix_ChannelFinished(_subHook);
    Mix_HookMusicFinished(_hookFunc);
	
    m_is_init = true;
}

void System::init(void) {
    if(!m_is_init) _init();
}

bool System::isInit(void) {
    return m_is_init;
}

System::~System(void) {
    Mix_CloseAudio();
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}

} // namespace rmk