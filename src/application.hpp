#ifndef HAWKSTAR_APPLICATION_HPP_
#define HAWKSTAR_APPLICATION_HPP_

#include <blessed/span.hpp>

#include "camera.hpp"
#include "rectangle.hpp"
#include "point.hpp"
#include "stage_map.hpp"
#include "entity.hpp"

#include "sdl.hpp"

namespace hawkstar
{
    class Application
    {
    public:
        Application(struct SDL_Window *, struct SDL_Renderer *);

        void draw(void);

        inline void *downcast() noexcept
        {
            return static_cast<void *>(this);
        }

        inline void const *downcast() const noexcept
        {
            return static_cast<void const *>(this);
        }

        static inline Application *upcast(void *v)
        {
            return static_cast<Application *>(v);
        }

        inline struct SDL_Window *window() const noexcept
        {
            return window_.get();
        }

        inline struct SDL_Renderer *renderer() const noexcept
        {
            return renderer_.get();
        }

        rectangle<double> getScreenBounds(void) const;

        /**
         * @brief get a world rectangle based on the current camera position
         */
        rectangle<double> getRenderBounds(void) const;

        void periodic(void);
        void onKeyDown(SDL_KeyboardEvent const &);
        void onKeyUp(SDL_KeyboardEvent const &);
        bool running(void) const noexcept;

    private:
        sdl::unique_window window_;
        sdl::unique_renderer renderer_;

        blessed::span<bool const> keyState_;

        StageMap map_;
        Camera camera_;
        Entity player_;
    };
}

#endif
