#include <SDL3/SDL.h>

#include "application.hpp"

namespace hawkstar
{
    Application::Application(SDL_Window *w, SDL_Renderer *r) :
        window_(w),
        renderer_(r)
    {}

    /**
     * thought screed on rendering
     * 
     * We have a screen co-ordinate system with (0, 0) at the top-left. This is based on the
     * window size (use the window parameter as a reference).
     * 
     * We have a tile map with [0, 0] at the bottom left and a grid size of x tiles by y tiles. The
     * tile size is a multiple of pixels. 
     * 
     * We have a world with a [0, 0] at the bottom left and a grid size of <tile-width> *
     * <tile-columns> by <tile-height> * <tile-rows>. Conversion between world and tile co-ordinates
     * should always be trivial linear algebra.
     * 
     * The camera position is always based on world co-ordinates. The screen is typically rendered with
     * the camera co-ordinates [cx, cy] centred on [<screen-width> / 2, <screen-height> / 2]. 
     * 
     * We make exceptions to this rule for the purpose of smooth player movement; we may not want to 
     * rigidly bind the camera to the player for small movement. Some investigatgion later we see that
     * there are a few common models:
     *     a) strict player/camera binding (e.g. Magic Pockets)
     *     b) camera bound to an outer box, moves as the player pushes towards the edges (e.g. Gods)
     *     c) camera snaps to coarse grid (e.g. Rick Dangerous). This works better in games with 
     *        clearly zoned areas. 
     *     d) camera has a delayed pan (e.g. Killing Game Show). This works very well where vertical
     *        movement is limited, unclear how well this works where the player can move large
     *        vertical distances
     * In many cases, the treatment of the x axis is different to the y-axis. The x axis typically
     * has a fixed pan.
     */

    SDL_Point x;

    rectangle<std::size_t> Application::getScreenBounds(void) const
    {
        int w{};
        int h{};

        SDL_GetWindowSize(window_, &w, &h);

        return rectangle<std::size_t>(static_cast<std::size_t>(w), static_cast<std::size_t>(h));
    }

    rectangle<std::size_t> Application::getRenderBounds(void) const
    {
        int w{};
        int h{};

        SDL_GetRenderOutputSize(renderer_, &w, &h);

        return rectangle<std::size_t>(static_cast<std::size_t>(w), static_cast<std::size_t>(h));
    }

    static SDL_FRect translateRenderRect(rectangle<std::size_t> const &world, rectangle<std::size_t> const &render)
    {
        return SDL_FRect{
            static_cast<float>(world.x()),
            static_cast<float>(render.height() - (world.y() + world.height())),
            static_cast<float>(world.width()),
            static_cast<float>(world.height())
        };
    }

    void Application::draw(void)
    {
        SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
        SDL_RenderClear(renderer_);
        SDL_SetRenderScale(renderer_, 1.0f, 1.0f);

        {
            rectangle<std::size_t> screen = getRenderBounds();
            rectangle<std::size_t> tr{map_.getTileRegion(screen)};

            SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255);
            for (std::size_t x = 0; x < tr.width(); ++x)
            {
                for (std::size_t y = 0; y < tr.height(); ++y)
                {
                    auto id = map_.getId(x, y);

                    if (id != 0)
                    {
                        auto tm = map_.getWorldRegion(x ,y);
                        SDL_FRect frect{translateRenderRect(tm, screen)};
    
                        SDL_RenderFillRect(renderer_, &frect);
                    }
                }
            }
        }

        {
            char camera_text[64];

            SDL_snprintf(camera_text, sizeof(camera_text), "x: %.02f y: %.02f", camera_.x(), camera_.y());

            SDL_SetRenderScale(renderer_, 2.0f, 2.0f);
            SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
            SDL_RenderDebugText(renderer_, 0, 0, camera_text);
        }

        SDL_RenderPresent(renderer_);
    }
}
