#ifndef SDL_HPP_
#define SDL_HPP_

#include <memory>

#include <SDL3/SDL.h>

namespace sdl
{
    namespace detail
    {
        struct window_deleter
        {
            void operator()(SDL_Window *p) const
            {
                SDL_DestroyWindow(p);
            }
        };

        struct renderer_deleter
        {
            void operator()(SDL_Renderer *p) const
            {
                SDL_DestroyRenderer(p);
            }
        };
    }

    using unique_renderer = std::unique_ptr<SDL_Renderer, detail::renderer_deleter>;
    using unique_window = std::unique_ptr<SDL_Window, detail::window_deleter>;
}

#endif
