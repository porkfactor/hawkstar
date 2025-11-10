cmake_minimum_required(VERSION 3.25.0)

add_executable(resource
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_compile_options(resource PRIVATE
    -Werror -Wall -Wextra
)

target_link_libraries(resource
    PRIVATE
        cxxopts
)