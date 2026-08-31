#ifndef REMAKE2D_SDL_
#define REMAKE2D_SDL_

#include <remake2d/color.hpp>
#include <remake2d/numeric.hpp>
#include <remake2d/config/forward.hpp>

#include <string>

namespace rmk {

class SDLWrapper {
private:
    SDLWrapper(void)                       = default;
    SDLWrapper(SDLWrapper&&)                    = delete;
    SDLWrapper(const SDLWrapper&)               = delete;
    SDLWrapper& operator=(SDLWrapper&&)         = delete;
    SDLWrapper& operator=(const SDLWrapper&)    = delete;

public:
    static SDLWrapper& getInstance(void) noexcept;

public:
    SDL_Texture* createTexture(SDL_Renderer*, u32, i32, i32, i32)              const noexcept;
    SDL_Texture* createTextureFromSurface(SDL_Renderer*, SDL_Surface*)         const noexcept;
    void         destroyTexture(SDL_Texture*)                                  const noexcept;

    void setTextureBlendMode(SDL_Texture*, i32)                                const noexcept;
    void setTextureColorMod(SDL_Texture*, u8, u8, u8)                          const noexcept;
    void setTextureAlphaMod(SDL_Texture*, u8)                                  const noexcept;

    SDL_Texture* getRenderTarget(SDL_Renderer*)                                const noexcept;
    void         setRenderTarget(SDL_Renderer*, SDL_Texture*)                  const noexcept;
    void         setRenderDrawColor(SDL_Renderer*, u8, u8, u8, u8)             const noexcept;
    void         renderClear(SDL_Renderer*)                                    const noexcept;
    void         renderCopy(SDL_Renderer*, SDL_Texture*, const SDL_Rect*, const SDL_Rect*) const noexcept;

    std::string getError(void) const noexcept;
    void setHint(std::string_view, std::string_view) const noexcept;

    void freeSurface(SDL_Surface*) const noexcept;
    void freeChunk(Mix_Chunk*)     const noexcept;
    void freeMusic(Mix_Music*)     const noexcept;

    i32 surfaceWidth(SDL_Surface*)  const noexcept;
    i32 surfaceHeight(SDL_Surface*) const noexcept;

    SDL_Surface* load(std::string_view)      const noexcept;
    std::string  getImageError(void)         const noexcept;

    TTF_Font*    openFont(std::string_view, i32)                                    const noexcept;
    void         closeFont(TTF_Font*)                                               const noexcept;
    std::string  getFontError(void)                                                 const noexcept;
    SDL_Surface* renderGlyphBlended(TTF_Font*, char, Color)                         const noexcept;
    SDL_Surface* renderUTF8BlendedWrapped(TTF_Font*, std::string_view, Color, u32)  const noexcept;
    i32          fontAscent(TTF_Font*)                                              const noexcept;

    bool  keyPressed(i32)                     const noexcept;
    i32   numJoysticks(void)                  const noexcept;
    i32   joystickInstanceId(i32)             const noexcept;
    bool  controllerButtonPressed(SDL_GameController*, i32) const noexcept;
};

inline SDLWrapper& sdl = SDLWrapper::getInstance();

} // namespace rmk

#endif
