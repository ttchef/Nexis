cmake_minimum_required(VERSION 4.0)
project(Nexis VERSION 1.0.0 LANGUAGES CXX)

include(FetchContent)

# Raylib
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    # 6.0 Release
    GIT_TAG dbc56a87da87d973a9c5baa4e7438a9d20121d28
)

# Clay
FetchContent_Declare(
    clay
    GIT_REPOSITORY https://github.com/nicbarker/clay.git
    # 0.14 Release
    GIT_TAG b25a31c1a152915cd7dd6796e6592273e5a10aac
)
set(CLAY_INCLUDE_ALL_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib clay)

set(SRC_FILES
    src/main.cpp
    src/ui.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_link_libraries(${PROJECT_NAME} raylib)
# System include to suppress warnings
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${clay_SOURCE_DIR})
# Normal include
target_include_directories(${PROJECT_NAME} PRIVATE src)
