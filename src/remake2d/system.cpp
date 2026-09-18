#include <remake2d/system.hpp>
#include <remake2d/utility.hpp>
#include <remake2d/error.hpp>
#include <remake2d/sound.hpp>
#include <remake2d/config/sdl.hpp>

#include <cmath>
#include <mutex>
#include <chrono>
#include <time.h>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>


namespace rmk {

bool System::Setup::scalingMode(std::string_view mode) {
    #ifdef SDL_HINT_RENDER_SCALE_QUALITY
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    return sdl.setHint(SDL_HINT_RENDER_SCALE_QUALITY, mode);
    #else
    (void) mode;
    return false;
    #endif
}

bool System::Setup::backend(std::string_view bcknd) {
    #ifdef SDL_HINT_RENDER_DRIVER
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    return sdl.setHint(SDL_HINT_RENDER_DRIVER, bcknd);
    #else
    (void) bcknd;
    return false;
    #endif
}

bool System::Setup::audioMode(std::string_view mode) {
    #ifdef SDL_AUDIO_RESAMPLING_MODE
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    return sdl.setHint(SDL_AUDIO_RESAMPLING_MODE, mode);
    #else
	(void) mode;
    return false;
    #endif
}

bool System::Setup::audioCategory(std::string_view category) {
    #ifdef SDL_AUDIO_CATEGORY
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    return sdl.setHint(SDL_AUDIO_CATEGORY, category);
    #else
	(void) category;
    return false;
    #endif
}

bool System::Setup::mouseRelativeMode(std::string_view mode) {
    #ifdef SDL_HINT_MOUSE_RELATIVE_MODE_WARP
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    return sdl.setHint(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, mode);
    #else
	(void) mode;
    return false;
    #endif
}

bool System::Setup::mobileOrientation(std::span<std::string_view> orients) {
    #ifdef SDL_HINT_ORIENTATION
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    std::string flags;
    for(const auto& e : orients) flags += std::string(e) + " ";
    return sdl.setHint(SDL_HINT_ORIENTATION, flags);
    #else
	(void) orients;
    return false;
    #endif
}


void System::Setup::allocateChannels(u16 channels) {
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    system.m_channel_count = std::clamp(channels, (u16)channel::min, (u16)channel::max);
}

bool System::Toggle::vsync(bool statut) {
    #ifdef SDL_HINT_RENDER_VSYNC
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    std::string s = std::to_string((int)statut);
    return sdl.setHint(SDL_HINT_RENDER_VSYNC, s);
    #else
	(void) statut;
    return false;
    #endif
}

bool System::Toggle::blockOnPause(bool statut) {
    #ifdef SDL_HINT_ANDROID_BLOCK_ON_PAUSE
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    std::string s = std::to_string((int)statut);
    return sdl.setHint(SDL_HINT_ANDROID_BLOCK_ON_PAUSE, s);
    #else
	(void) statut;
    return false;
    #endif
}

bool System::Toggle::hideHomeIndicator(bool statut) {
    #ifdef SDL_HINT_IOS_HIDE_HOME_INDICATOR
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    std::string s = std::to_string((int)statut);
    return sdl.setHint(SDL_HINT_IOS_HIDE_HOME_INDICATOR, s);
    #else
	(void) statut;
    return false;
    #endif
}

void System::Toggle::relativeMouseMode(bool statut) {
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    system.m_relative_mouse_mode = statut;
}

bool System::Toggle::materialAcceleration(bool statut) {
    #ifdef SDL_HINT_FRAMEBUFFER_ACCELERATION
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    std::string s = std::to_string((int)statut);
    return sdl.setHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, s);
    #else
	(void) statut;
    return false;
    #endif
}

void System::Toggle::gameController(bool statut) {
    #if defined(SDL_INIT_JOYSTICK) && defined(SDL_INIT_GAMECONTROLLER) && defined(SDL_INIT_HAPTIC)
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    auto flags = SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC;
    if (statut) SDL_InitSubSystem(flags);
    else SDL_QuitSubSystem(flags);
    #else
    (void) statut;
    #endif
}

bool System::Toggle::accelerometerAsJoystick(bool statut) {
    #ifdef SDL_HINT_ACCELEROMETER_AS_JOYSTICK
    if(system.m_is_init) rmk_dynamicAssert(rmk::SystemError, error::system::bad_call);
    std::string s = std::to_string((int)statut);
    return sdl.setHint(SDL_HINT_ACCELEROMETER_AS_JOYSTICK, s);
    #else
	(void) statut;
    return false;
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

void System::init(void) {
	if(m_is_init) return;
    std::mutex mut;
    std::lock_guard<std::mutex> lock(mut);

    config::system::initSDL();

    toggle.vsync(true);
    SDL_SetRelativeMouseMode(m_relative_mouse_mode ? SDL_TRUE : SDL_FALSE);

    m_channel_count = m_channel_count ? m_channel_count : (u16)channel::mid;
    Mix_AllocateChannels((int)m_channel_count);

    m_is_init = true;
}

void System::_init(void) {
	m_instance.fetch_add(1);
    init();
}

bool System::isInit(void) {
    return m_is_init;
}

void System::_quit(void) {
	m_instance.fetch_sub(1);
	quit();
}

void System::quit(void) {
	if (!m_is_init || m_instance) return;

	std::mutex mut;
    std::lock_guard<std::mutex> lock(mut);

    Mix_CloseAudio();
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();

	croutinePool.stopAll();
	m_is_init = false;
}

} // namespace rmk