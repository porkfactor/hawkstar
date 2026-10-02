cmake_minimum_required(VERSION 3.25.0)

set(CMAKE_POLICY_DEFAULT_CMP077 NEW)

set(BUILD_SHARED_LIBS FALSE)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

project(hawkstar VERSION 0.1.0)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/subprojects)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tools)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

