#ifndef REMAKE2D_RESSOURCE_
#define REMAKE2D_RESSOURCE_

#include <remake2d/config/sdl.hpp>
#include <remake2d/config/forward.hpp>

namespace rmk {

inline void _resourceFreeSurface(SDL_Surface* s)  { sdl.freeSurface(s); }
inline void _resourceFreeChunk(Mix_Chunk* c)      { sdl.freeChunk(c);   }
inline void _resourceFreeMusic(Mix_Music* m)      { sdl.freeMusic(m);   }

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

using Surface = Resource<SDL_Surface, _resourceFreeSurface>;
using CHK     = Resource<Mix_Chunk,   _resourceFreeChunk>;
using MUS     = Resource<Mix_Music,   _resourceFreeMusic>;

} // namespace rmk

#endif

#include <remake2d/template/config/resource.tpp>