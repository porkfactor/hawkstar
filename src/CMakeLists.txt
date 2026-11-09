cmake_minimum_required(VERSION 3.25.0)

add_executable(hawkstar
    ${CMAKE_CURRENT_SOURCE_DIR}/application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/application.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdl_main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/point.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vector.hpp
)

set_target_properties(hawkstar PROPERTIES
    CXX_STANDARD 11
    C_STANDARD 11
)

target_compile_definitions(hawkstar PRIVATE
    SDL_MAIN_USE_CALLBACKS
)

target_compile_options(hawkstar PRIVATE
    -Werror
    -Wall
    -Wextra
    -Wno-unused-parameter
)

target_include_directories(hawkstar PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
)

target_link_libraries(hawkstar
    PRIVATE
        blessed
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        SDL3_mixer::SDL3_mixer
        SDL3::SDL3
)

