cmake_minimum_required(VERSION 3.25.0)

add_executable(hawkstar
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
)

target_compile_options(hawkstar PRIVATE
    -Werror
    -Wall
    -Wextra
    -Wno-unused-parameter
)

target_link_libraries(hawkstar
    PRIVATE
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        SDL3_mixer::SDL3_mixer
        SDL3::SDL3
)

