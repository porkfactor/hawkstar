#ifndef HAWKSTAR_APPLICATION_HPP_
#define HAWKSTAR_APPLICATION_HPP_

#include "camera.hpp"
#include "rectangle.hpp"
#include "point.hpp"
#include "stage_map.hpp"

struct SDL_Renderer;
struct SDL_Window;

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
            return window_;
        }

        inline struct SDL_Renderer *renderer() const noexcept
        {
            return renderer_;
        }

        rectangle<std::size_t> getScreenBounds(void) const;
        rectangle<std::size_t> getRenderBounds(void) const;

    private:
        struct SDL_Window *window_;
        struct SDL_Renderer *renderer_;

        Camera camera_;
        StageMap map_;
    };
}

#endif
