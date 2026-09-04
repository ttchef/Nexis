cmake_minimum_required(VERSION 3.15)
project(Nexis VERSION 1.0.0)

# Build options
option(NEXIS_BUILD_EDITOR "Build Nexis editor" ON)

set(CMAKE_C_STADARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

add_subdirectory(core)

if (NEXIS_BUILD_EDITOR)
    add_subdirectory(editor)
endif()
