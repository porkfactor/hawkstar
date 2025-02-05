cmake_minimum_required(VERSION 3.25.0)

include(FetchContent)

FetchContent_Declare(sdl-image
    GIT_REPOSITORY "git@github.com:libsdl-org/SDL_image.git"
    GIT_TAG "release-3.2.0"
    GIT_SHALLOW TRUE
    GIT_SUBMODULES ""
    EXCLUDE_FROM_ALL
    SYSTEM
)

FetchContent_MakeAvailable(sdl-image)
