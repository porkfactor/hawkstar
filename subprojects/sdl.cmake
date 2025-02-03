cmake_minimum_required(VERSION 3.25.0)

include(FetchContent)

set(SDL_SHARED FALSE)
set(SDL_STATIC TRUE)
set(SDL_TEST_LIBRARY FALSE)
set(SDL_TESTS FALSE)
set(SDL_DISABLE_INSTALL TRUE)
set(SDL_DISABLE_INSTALL_DOCS TRUE)
set(SDL_INSTALL_TESTS FALSE)

FetchContent_Declare(SDL
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
    GIT_TAG "release-3.2.0"
    GIT_SHALLOW TRUE
    GIT_SUBMODULES ""
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(SDL)

