cmake_minimum_required(VERSION 3.25.0)

set(BUILD_SHARED_LIBS FALSE)

project(hawkstar VERSION 0.1.0)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/subprojects)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

