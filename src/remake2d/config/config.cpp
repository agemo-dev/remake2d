#include <remake2d/config/config.hpp>
#include <remake2d/utility.hpp>
#include <remake2d/sound.hpp>

rmk_searchIncludeF2(<SDL2/SDL.h>,       <SDL.h>)
rmk_searchIncludeF2(<SDL2/SDL_ttf.h>,   <SDL_ttf.h>)
rmk_searchIncludeF2(<SDL2/SDL_image.h>, <SDL_image.h>)
rmk_searchIncludeF2(<SDL2/SDL_mixer.h>, <SDL_mixer.h>)

namespace rmk {
namespace config {

namespace system {
void initSDL(void) {
    SDL_SetMainReady();

    int sdlflags = SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS;
    if(SDL_Init(sdlflags) != 0) {
        rmk_dynamicAssert(rmk::SystemError, (std::string(error::system::sdl_init_fail) + " : " + SDL_GetError()));
    }

    int imgflags = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_WEBP;
    if((IMG_Init(imgflags) & imgflags) != imgflags) {
        rmk_dynamicAssert(rmk::SystemError, (std::string(error::system::sdl_image_init_fail) + " : " + IMG_GetError()));
    }

    int mixflags = MIX_INIT_OGG | MIX_INIT_MP3 | MIX_INIT_FLAC;
    if((Mix_Init(mixflags) & mixflags) != mixflags) {
        rmk_dynamicAssert(rmk::SystemError, (std::string(error::system::sdl_mixer_init_fail) + " : " + Mix_GetError()));
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        rmk_dynamicAssert(rmk::SystemError, (std::string(error::system::sdl_mixer_init_fail) + " : " + Mix_GetError()));
    }

    if(TTF_Init() != 0) {
        rmk_dynamicAssert(rmk::SystemError, (std::string(error::system::sdl_ttf_init_fail) + " : " + TTF_GetError()));
    }
}
} //namespace system

namespace sound {
void initQueue(void) noexcept {
    static bool isInit = false;

    if(isInit) return;
    for(u16 i = 0; i < rmk::system.info.channelCount(); i++) {
        rmk::SFX::m_free_channels.push(i);
        rmk::SFX::m_channel_owners[i] = nullptr;
    }
    isInit = true;
}
} //namespace sound

} //namespace config
} //namespace rmk