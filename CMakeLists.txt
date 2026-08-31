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

# ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    # Docking branch
    GIT_TAG fd13a1e8923a0a7077b404fc36fd063b25a0c0b5
)

FetchContent_Declare(
    rlimgui
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
    GIT_TAG Raylib_6_0
)

FetchContent_MakeAvailable(raylib imgui rlimgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)

add_library(rlimgui STATIC
    ${rlimgui_SOURCE_DIR}/rlImGui.cpp
)

target_include_directories(rlimgui PUBLIC
    ${rlimgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}
)

target_link_libraries(rlimgui PUBLIC
    raylib
    imgui
)

set(SRC_FILES
    src/main.cpp
    src/ui.cpp
    src/globals.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_link_libraries(${PROJECT_NAME} PRIVATE
    raylib
    imgui
    rlimgui
)
target_include_directories(${PROJECT_NAME} PRIVATE src)
