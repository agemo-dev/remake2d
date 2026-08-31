#include <remake2d/sdl.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

namespace rmk {

SDLWrapper& SDLWrapper::getInstance(void) noexcept {
    static SDLWrapper instance;
    return instance;
}

SDL_Texture* SDLWrapper::createTexture(SDL_Renderer* renderer, u32 format, i32 access, i32 w, i32 h) const noexcept {
    return SDL_CreateTexture(renderer, format, (SDL_TextureAccess)access, w, h);
}

SDL_Texture* SDLWrapper::createTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface) const noexcept {
    return SDL_CreateTextureFromSurface(renderer, surface);
}

void SDLWrapper::destroyTexture(SDL_Texture* texture) const noexcept {
    SDL_DestroyTexture(texture);
}

void SDLWrapper::setTextureBlendMode(SDL_Texture* texture, i32 mode) const noexcept {
    SDL_SetTextureBlendMode(texture, (SDL_BlendMode)mode);
}

void SDLWrapper::setTextureColorMod(SDL_Texture* texture, u8 r, u8 g, u8 b) const noexcept {
    SDL_SetTextureColorMod(texture, r, g, b);
}

void SDLWrapper::setTextureAlphaMod(SDL_Texture* texture, u8 a) const noexcept {
    SDL_SetTextureAlphaMod(texture, a);
}

SDL_Texture* SDLWrapper::getRenderTarget(SDL_Renderer* renderer) const noexcept {
    return SDL_GetRenderTarget(renderer);
}

void SDLWrapper::setRenderTarget(SDL_Renderer* renderer, SDL_Texture* texture) const noexcept {
    SDL_SetRenderTarget(renderer, texture);
}

void SDLWrapper::setRenderDrawColor(SDL_Renderer* renderer, u8 r, u8 g, u8 b, u8 a) const noexcept {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void SDLWrapper::renderClear(SDL_Renderer* renderer) const noexcept {
    SDL_RenderClear(renderer);
}

void SDLWrapper::renderCopy(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* src, const SDL_Rect* dst) const noexcept {
    SDL_RenderCopy(renderer, texture, src, dst);
}

bool setHint(std::string_view name, std::string_view val) const noexcept {
    return SDL_SetHint(std::string(name).c_str(), std::string(val).c_str());
}

std::string SDLWrapper::getError(void) const noexcept {
    return SDL_GetError();
}

void SDLWrapper::freeSurface(SDL_Surface* surface) const noexcept {
    SDL_FreeSurface(surface);
}

void SDLWrapper::freeChunk(Mix_Chunk* chunk) const noexcept {
    Mix_FreeChunk(chunk);
}

void SDLWrapper::freeMusic(Mix_Music* music) const noexcept {
    Mix_FreeMusic(music);
}

i32 SDLWrapper::surfaceWidth(SDL_Surface* surface) const noexcept {
    return surface->w;
}

i32 SDLWrapper::surfaceHeight(SDL_Surface* surface) const noexcept {
    return surface->h;
}

SDL_Surface* SDLWrapper::load(std::string_view path) const noexcept {
    return IMG_Load(path.data());
}

std::string SDLWrapper::getImageError(void) const noexcept {
    return IMG_GetError();
}

TTF_Font* SDLWrapper::openFont(std::string_view path, i32 size) const noexcept {
    return TTF_OpenFont(std::string(path).c_str(), size);
}

void SDLWrapper::closeFont(TTF_Font* font) const noexcept {
    TTF_CloseFont(font);
}

std::string SDLWrapper::getFontError(void) const noexcept {
    return TTF_GetError();
}

SDL_Surface* SDLWrapper::renderGlyphBlended(TTF_Font* font, char ch, Color color) const noexcept {
    return TTF_RenderGlyph_Blended(font, ch, color._data());
}

SDL_Surface* SDLWrapper::renderUTF8BlendedWrapped(TTF_Font* font, std::string_view text, Color color, u32 wrapLength) const noexcept {
    return TTF_RenderUTF8_Blended_Wrapped(font, std::string(text).c_str(), color._data(), wrapLength);
}

i32 SDLWrapper::fontAscent(TTF_Font* font) const noexcept {
    return TTF_FontAscent(font);
}

bool SDLWrapper::keyPressed(i32 scancode) const noexcept {
    return SDL_GetKeyboardState(nullptr)[scancode] != 0;
}

i32 SDLWrapper::numJoysticks(void) const noexcept {
    return SDL_NumJoysticks();
}

i32 SDLWrapper::joystickInstanceId(i32 index) const noexcept {
    return SDL_JoystickGetDeviceInstanceID(index);
}

bool SDLWrapper::controllerButtonPressed(SDL_GameController* ctrl, i32 button) const noexcept {
    return SDL_GameControllerGetButton(ctrl, (SDL_GameControllerButton)button) != 0;
}

} // namespace rmk
