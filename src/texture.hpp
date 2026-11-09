#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include <memory>
#include <SDL3/SDL.h>

struct sdl_texture_deleter
{
    void operator()(struct SDL_Texture *ptr) const
    {
        SDL_DestroyTexture(ptr);
    }
};

using unique_sdl_texture = std::unique_ptr<SDL_Texture, sdl_texture_deleter>;

#endif
