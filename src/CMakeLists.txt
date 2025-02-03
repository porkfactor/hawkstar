cmake_minimum_required(VERSION 3.25.0)

add_executable(hawkstar
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
)

target_link_libraries(hawkstar
    PRIVATE
        SDL3::SDL3
)

