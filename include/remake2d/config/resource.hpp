#ifndef REMAKE2D_RESSOURCE_
#define REMAKE2D_RESSOURCE_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

namespace rmk {

template<typename T, void(*F)(T*)> struct Resource {
public:
    T* data{nullptr};

private:
    u32* m_refcount{nullptr};

public:
    Resource(T*);
    Resource(void);
    Resource(const Resource&);
    Resource(Resource&&) noexcept;

    Resource& operator=(const Resource&);
    Resource& operator=(Resource&&) noexcept;

    ~Resource(void);

private:
    void release(void);
};

using Surface = Resource<SDL_Surface, SDL_FreeSurface>;
using CHK     = Resource<Mix_Chunk,     Mix_FreeChunk>;
using MUS     = Resource<Mix_Music,     Mix_FreeMusic>;

} // namespace rmk

#endif

#include <remake2d/template/config/resource.tpp>