#include <remake2d/config/config.hpp>
#include <remake2d/all/everything.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

namespace rmk {

void _hookMusicFinished(void) {
    if(!rmk::Music::m_current_music) return;

    auto* mus = rmk::Music::m_current_music.locate<Music>();

    if(!mus) return;
    if(mus->m_loops_remaining < 0) {
        return;
    } else if(mus->m_loops_remaining > 0) {
        mus->m_loops_remaining -= 1;
		mus->onRepeat.emit();
    } else {
        mus->stop();
        mus->onFinish.emit();
        rmk::Music::m_current_music = nil;
    }
}

 void _channelFinished(int channel) {
    if(channel >= (int)system.info.channelCount()) return;

    rmk::SFX* sfx = rmk::SFX::m_channel_owners[channel].locate<SFX>();

    if(!sfx) return;
    if(sfx->m_loops_remaining < 0) {
        return;
    } else if(sfx->m_loops_remaining > 0) {
        sfx->m_loops_remaining -= 1;
		sfx->onRepeat.emit();
    } else {
        rmk::SFX::m_free_channels.push(channel);
        rmk::SFX::m_channel_owners[channel] = nil;
        sfx->m_is_playing = false;
        sfx->onFinish.emit();
    }
 }


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

    Mix_ChannelFinished(_channelFinished);
    Mix_HookMusicFinished(_hookMusicFinished);
}

} //namespace system


namespace loop {

void init (void) noexcept {
    rmk::loop.add(event.tracker());
    rmk::loop.add(delta.tracker());
    rmk::loop.add(physics.tracker());
    rmk::loop.add(parallax.tracker());
    rmk::loop.add(animation.tracker());
}

} // namespace loop


namespace sound {
void initQueue(void) noexcept {
    static bool isInit = false;

    if(isInit) return;
    for(u16 i = 0; i < rmk::system.info.channelCount(); i++) {
        rmk::SFX::m_free_channels.push(i);
        rmk::SFX::m_channel_owners[i] = nil;
    }
    isInit = true;
}
} //namespace sound

} //namespace config
} //namespace rmk