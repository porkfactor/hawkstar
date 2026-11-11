cmake_minimum_required(VERSION 3.25.0)

include(FetchContent)

FetchContent_Declare(sdl-mixer
    GIT_REPOSITORY "git@github.com:libsdl-org/SDL_mixer.git"
    GIT_TAG "release-3.2.0"
    GIT_SHALLOW TRUE
    GIT_SUBMODULES ""
    EXCLUDE_FROM_ALL
    SYSTEM
)

set(SDLMIXER_DEPS_SHARED FALSE)
set(SDLMIXER_BUILD_SHARED_LIBS FALSE)

FetchContent_MakeAvailable(sdl-mixer)
