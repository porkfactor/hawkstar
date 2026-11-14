#include <cassert>

#include <SDL3/SDL.h>

#include "application.hpp"

namespace hawkstar
{
    Application::Application(SDL_Window *w, SDL_Renderer *r) :
        window_(w),
        renderer_(r)
    {
        int keyCount{};
        bool const *keyState = SDL_GetKeyboardState(&keyCount);

        keyState_ = blessed::span<bool const>(keyState, static_cast<std::size_t>(keyCount));
    }

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

    rectangle<double> Application::getScreenBounds(void) const
    {
        int w{};
        int h{};

        SDL_GetWindowSize(window(), &w, &h);

        return rectangle<double>(static_cast<double>(w), static_cast<double>(h));
    }

    rectangle<double> Application::getRenderBounds(void) const
    {
        int w{};
        int h{};

        SDL_GetRenderOutputSize(renderer(), &w, &h);

        double dw = static_cast<double>(w);
        double dh = static_cast<double>(h);

        return rectangle<double>(
            camera_.x() - (dw / 2.0),
            camera_.y() - (dh / 2.0),
            dw,
            dh
        );
    }

    static SDL_FRect translateRenderRect(rectangle<double> const &world, rectangle<double> const &render)
    {
        return SDL_FRect{
            static_cast<float>(world.x() - render.x()),
            static_cast<float>((render.y() + render.height()) - (world.y() + world.height())),
            static_cast<float>(world.width()),
            static_cast<float>(world.height())
        };
    }

    void Application::periodic(void)
    {
        player_.periodic();

        camera_.move(player_.position());
    }

    /**
     * one problem to resolve - if we assume the world co-ordinate system is
     * double-precision then we need to deal with the 'snap' from the tile
     * system. 
     * 
     * e.g. say the camera is at (100.5, 100.5) we need the tile region 
     * surrounding that (-923.5, -667.5, 1024, 768), but ... that has to snap
     * to a whole-number position because tiles are tiles, so we get tiles at
     * (-924, -668, 1024, 768) which expands to (-928, -672, 1040, 784).
     * 
     * question: does the renderer deal with clipping? that will be handy if it
     * does.
     * 
     * The top-left tile (-928, -672) needs to be clipped to (-923.5, -667.5, 
     * 14.5, 15.5) - so delta rect is (-1.5, -0.5, -1.5, -0.5), or do we just
     * use a translation point (-1.5, -0.5) - in general we're setting a
     * clipping point at one of the four vertices
     * 
     */

    void Application::draw(void)
    {
        SDL_SetRenderDrawColor(renderer(), 0, 0, 0, 255);
        SDL_RenderClear(renderer());
        SDL_SetRenderScale(renderer(), 1.0f, 1.0f);

        {
            rectangle<double> screen = getRenderBounds();
            rectangle<std::size_t> tr{map_.getTileRegion(rectangle<std::size_t>{screen})};
            // vector<double> delta = screen.origin() - point<double>(tr.origin());

            {
                auto wr = map_.getWorldRegion(tr);
                assert((wr.width() >= screen.width()) && (wr.height() >= screen.height()));
            }

            SDL_SetRenderDrawColor(renderer(), 255, 0, 0, 255);
            for (std::size_t x = 0; x < tr.width(); ++x)
            {
                for (std::size_t y = 0; y < tr.height(); ++y)
                {
                    auto id = map_.getId(x, y);

                    if (id != 0)
                    {
                        /**
                         * need to translate the pixel position to a world
                         * position. 
                         */
                        auto tm = rectangle<double>(map_.getWorldRegion(x ,y));

                        SDL_FRect frect{translateRenderRect(tm, screen)};
    
                        SDL_RenderFillRect(renderer(), &frect);
                    }
                }
            }
        }

        {
            char camera_text[64];

            SDL_snprintf(camera_text, sizeof(camera_text), "x: %.02f y: %.02f", camera_.x(), camera_.y());

            SDL_SetRenderScale(renderer(), 2.0f, 2.0f);
            SDL_SetRenderDrawColor(renderer(), 255, 255, 255, 255);
            SDL_RenderDebugText(renderer(), 0, 0, camera_text);
        }

        SDL_RenderPresent(renderer());
    }

    void Application::onKeyDown(SDL_KeyboardEvent const &e)
    {
        switch (e.scancode)
        {
        case SDL_SCANCODE_W:
            player_.actionOn(PLAYER_ACTION_UP);
            break;
        case SDL_SCANCODE_D:
            player_.actionOn(PLAYER_ACTION_RIGHT);
            break;
        case SDL_SCANCODE_A:
            player_.actionOn(PLAYER_ACTION_LEFT);
            break;
        case SDL_SCANCODE_S:
            player_.actionOn(PLAYER_ACTION_DOWN);
            break;
        default:
            break;
        }
    }

    void Application::onKeyUp(SDL_KeyboardEvent const &e)
    {
        switch (e.scancode)
        {
        case SDL_SCANCODE_W:
            player_.actionOff(PLAYER_ACTION_UP);
            break;
        case SDL_SCANCODE_D:
            player_.actionOff(PLAYER_ACTION_RIGHT);
            break;
        case SDL_SCANCODE_A:
            player_.actionOff(PLAYER_ACTION_LEFT);
            break;
        case SDL_SCANCODE_S:
            player_.actionOff(PLAYER_ACTION_DOWN);
            break;
        default:
            break;
        }
    }

    bool Application::running(void) const noexcept
    {
        return true;
    }
}
