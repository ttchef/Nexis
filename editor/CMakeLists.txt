
# Raylib
FetchContent_Declare(
    raylib
    # v6.0 Release
    URL https://github.com/raysan5/raylib/archive/refs/tags/6.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# ImGui
FetchContent_Declare(
    imgui
    # Docking branch
    URL https://github.com/ocornut/imgui/archive/fd13a1e8923a0a7077b404fc36fd063b25a0c0b5.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# rlImGui
FetchContent_Declare(
    rlimgui
    # Raylib v6.0
    URL https://github.com/raylib-extras/rlImGui/archive/refs/tags/Raylib_6_0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# NativeFileDialog
FetchContent_Declare(
    nfd
    # v1.3
    URL https://github.com/btzy/nativefiledialog-extended/archive/fc168e8605bfa51aaec22ab0c4e46b9de665a437.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(raylib imgui rlimgui nfd)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

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

file(COPY ${CMAKE_SOURCE_DIR}/editor/assets DESTINATION ${CMAKE_BINARY_DIR}/editor/)
file(COPY ${CMAKE_SOURCE_DIR}/editor/src/shaders DESTINATION ${CMAKE_BINARY_DIR}/editor/)

add_executable(Nexis
    src/main.cpp
    src/app.cpp
    src/utils.cpp
    src/asset_manager.cpp
    src/ui/context.cpp
    src/ui/editor.cpp
    src/ui/project_explorer.cpp
)

target_compile_features(Nexis PRIVATE cxx_std_23)
target_include_directories(Nexis PRIVATE
    src
    ../backends
    ${nfd_SOURCE_DIR}/src/include
)

target_link_libraries(Nexis PRIVATE
    Nexis::Core
    raylib
    imgui
    rlimgui
    nfd
)
