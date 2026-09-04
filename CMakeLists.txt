cmake_minimum_required(VERSION 3.15)
project(Nexis VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

# Raylib
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    # 6.0 Release
    GIT_TAG dbc56a87da87d973a9c5baa4e7438a9d20121d28
)

# ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    # Docking branch
    GIT_TAG fd13a1e8923a0a7077b404fc36fd063b25a0c0b5
)

# rlImGui
FetchContent_Declare(
    rlimgui
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
    GIT_TAG Raylib_6_0
)

# NativeFileDialog
FetchContent_Declare(
    nfd
    GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
    # v1.3
    GIT_TAG fc168e8605bfa51aaec22ab0c4e46b9de665a437
)

FetchContent_MakeAvailable(raylib imgui rlimgui nfd)

add_subdirectory(core)
add_subdirectory(editor)
