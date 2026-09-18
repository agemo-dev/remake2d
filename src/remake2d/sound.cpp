#include <remake2d/sound.hpp>
#include <remake2d/private/nil.hpp>
#include <remake2d/config/config.hpp>

#include <SDL2/SDL_mixer.h>

#include <string>
#include <stdlib.h>
#include <algorithm>
#include <filesystem>

namespace rmk {

u8 Sound::volume(void) const noexcept {
    return m_volume;
}

i8 Sound::playFor(void) const noexcept {
    return m_loops_remaining;
}

Music::Music(std::string_view path, u8 vol) {
    std::string temp(path);
	m_music.data = Mix_LoadMUS(temp.c_str());

    if(!m_music.data) rmk_dynamicAssert(rmk::SoundError, (std::string(error::sound::sound_no_load) + " : " + Mix_GetError()));
    m_is_playing = false;
    volume(vol);
}

void Music::volume(u8 volume) noexcept {
    m_volume = std::max(volume, (u8)volume::max);
    if(m_is_playing) Mix_VolumeMusic(m_volume);
}

void Music::play(i8 loop) {
    if(m_current_music && m_current_music != tracker()) m_current_music->stop();
    m_current_music = tracker();
    Mix_HaltMusic();
    Mix_PlayMusic(m_music.data, loop < -1 ? -1 : loop);
    Mix_VolumeMusic(m_volume);
    m_loops_remaining = loop == 0 ? 0 : loop - 1;
    m_is_playing = true;
}

void Music::stop(void) {
    if(!m_is_playing) return;
    Mix_HaltMusic();
    m_loops_remaining = 0;
    m_is_playing = false;
}

void Music::pause(void) {
    if(m_is_playing) Mix_PauseMusic();
}

void Music::resume(void) {
    if(m_is_playing) Mix_ResumeMusic();
}

Music::~Music(void) {
    stop();
}

SFX::SFX(std::string_view path, u8 vol) {
    m_is_playing = false;
    std::string temp(path);
    config::sound::initQueue();
	m_sfx.data = Mix_LoadWAV(temp.c_str());

    if(!m_sfx.data) rmk_dynamicAssert(rmk::SoundError, (std::string(error::sound::sound_no_load) + " : " + Mix_GetError()));
    volume(vol);
}

void SFX::volume(u8 volume) noexcept {
    m_volume = volume > (u8)volume::max ? (u8)volume::max : volume;
    if(m_is_playing) Mix_Volume(m_channel, m_volume);
}

void SFX::play(i8 loop) {
    if(m_free_channels.empty()) return;
    m_channel = m_free_channels.front();
    m_free_channels.pop();
    m_loops_remaining = loop == 0 ? 0 : loop - 1;
    m_channel_owners[m_channel] = tracker();
    Mix_Volume(m_channel, m_volume);
    Mix_PlayChannel(m_channel, m_sfx.data, loop < -1 ? -1 : loop);
    m_is_playing = true;
}

void SFX::stop(void) {
    if (m_is_playing) {
        Mix_HaltChannel(m_channel);
        m_channel_owners[m_channel] = nil;
        m_free_channels.push(m_channel);
        m_is_playing = false;
    }
}

void SFX::stopAll(void) {
    for (u32 i = 0; i < (u32)m_channel_owners.size(); i++) {
        if (m_channel_owners[i] != tracker()) continue;
        Mix_HaltChannel(i);
        m_channel_owners[i] = nil;
        m_free_channels.push(i);
    }
    m_is_playing = false;
}

void SFX::pause(void) {
    if(m_is_playing) Mix_Pause(m_channel);
}

void SFX::resume(void) {
    if(m_is_playing) Mix_Resume(m_channel);
}

SFX::~SFX(void) {
    stopAll();
}

} //namespace rmk