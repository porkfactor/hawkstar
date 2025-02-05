cmake_minimum_required(VERSION 3.25.0)

include(FetchContent)

FetchContent_Declare(sdl-ttf
    GIT_REPOSITORY "git@github.com:libsdl-org/SDL_ttf.git"
    GIT_TAG "main"
    GIT_SHALLOW TRUE
    GIT_SUBMODULES ""
    EXCLUDE_FROM_ALL
    SYSTEM
)

FetchContent_MakeAvailable(sdl-ttf)
