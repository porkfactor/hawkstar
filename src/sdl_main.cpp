#include <new>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "application.hpp"

#include <blessed/intrusive_ptr.hpp>
#include <blessed/string_view.hpp>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **opaque, int argc, char *argv[])
{
    /* Create the window */
    if (!SDL_CreateWindowAndRenderer("HawkStar", 1024, 768, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    void *ptr = SDL_calloc(1, sizeof(hawkstar::Application));
    hawkstar::Application *application = new (ptr) hawkstar::Application(window, renderer);

    (*opaque) = application->downcast();

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *opaque, SDL_Event *event)
{
    if (event->type == SDL_EVENT_KEY_DOWN ||
        event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *opaque)
{
    hawkstar::Application *self = hawkstar::Application::upcast(opaque);

    self->draw();

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *opaque, SDL_AppResult result)
{
    hawkstar::Application *self = hawkstar::Application::upcast(opaque);

    delete self;
}
