cmake_minimum_required(VERSION 3.25.0)

include(FetchContent)

FetchContent_Declare(Freetype
    GIT_REPOSITORY "git@github.com:freetype/freetype.git"
    GIT_TAG "VER-2-13-3"
    GIT_SHALLOW TRUE
    GIT_SUBMODULES ""
    EXCLUDE_FROM_ALL
    SYSTEM
    OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(Freetype)

add_library(Freetype::Freetype ALIAS freetype)
