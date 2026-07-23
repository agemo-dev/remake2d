#ifndef REMAKE2D_SYSTEM_
#define REMAKE2D_SYSTEM_

#include <remake2d/clock.hpp>
#include <remake2d/vector.hpp>
#include <remake2d/numeric.hpp>
#include <remake2d/config/config.hpp>
#include <remake2d/config/script/config.hpp>

#include <span>
#include <string>
#include <stdlib.h>
#include <algorithm>

namespace rmk {

class Date;
class Clock;

class System {

private:
    struct Info {
        u32 ramMB(void);
        u8 cpuCount(void);
        u8 displayCount(void);
        u16 channelCount(void);
        Date  currentDay(void);
        Clock currentTime(void);
        Dim2d screenSize(u8 = 0);
        std::string platform(void);
    };

    struct Setup {
        void allocateChannels(u16);
        void backend(std::string_view);
        void audioMode(std::string_view);
        void scalingMode(std::string_view);
        void audioCategory(std::string_view);
        void mouseRelativeMode(std::string_view);
        void mobileOrientation(std::span<std::string_view>);
    };

    struct Toggle {
        void vsync(bool);
        void blockOnPause(bool);
        void gameController(bool);
        void hideHomeIndicator(bool);
        void relativeMouseMode(bool);
        void materialAcceleration(bool);
        void accelerometerAsJoystick(bool);
    };

public:
    Setup setup;
    Toggle toggle;
    Info info;

private:
    bool        m_is_init{false};
	u32			m_instance{0};
    u32         m_channel_count{0};
    SDL_bool    m_relative_mouse_mode{SDL_FALSE};

private:
    System(void)                        = default;
    System(const System &)              = delete;
    System &operator=(const System &)   = delete;

public:
    void init(void);
    bool isInit(void);
	void quit(void) noexcept;

public:
    static System &getInstance(void) noexcept;
    
private:
    void _init(void);

public:
    ~System(void);
    
    friend class Setup;
    friend class Toggle;
    friend class Info;
    friend class Window;
    friend class SFX;
    friend void config::solstat::initLuaSingleton(sol::table&) noexcept;
};

namespace orientation {
inline constexpr const char *normal     = "Portrait";
inline constexpr const char *reverse    = "PortraitUpSideDown";
inline constexpr const char *right      = "LandscapeRight";
inline constexpr const char *left       = "LandscapeLeft";
inline constexpr const char *portrait   = "Portrait PortraitUpSideDown";
inline constexpr const char *landscape  = "LandscapeRight LandscapeLeft";
inline constexpr const char *any        = "Portrait PortraitUpSideDown LandscapeRight LandscapeLeft";
}

namespace scaling {
inline constexpr const char *nearest        = "0";
inline constexpr const char *linear         = "1";
inline constexpr const char *anisotropic    = "2";
}

namespace backend {
inline constexpr const char *opengl     = "opengl";
inline constexpr const char *opengles   = "opengles2";
inline constexpr const char *direct3d   = "direct3D";
inline constexpr const char *vulkan     = "vulkan";
inline constexpr const char *metal      = "metal";
inline constexpr const char *software   = "software";
}

namespace audiomode {
inline constexpr const char *fast       = "fast";
inline constexpr const char *medium     = "medium";
inline constexpr const char *best       = "best";
}

namespace audiocategory {
inline constexpr const char *media  = "media";
inline constexpr const char *voice  = "voice";
inline constexpr const char *alarm  = "alarm";
}

namespace relmode {
inline constexpr const char *raw    = "0";
inline constexpr const char *warp   = "1";
}

enum class channel : u16 {
    min = 32,
    mid = 128,
    max = 256
};

inline System &system = System::getInstance();

} // namespace rmk

#endif