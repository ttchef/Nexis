
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
    src/ui/context.cpp
    src/ui/editor.cpp
    src/ui/project_explorer.cpp
)

target_compile_features(Nexis PRIVATE cxx_std_23)
target_include_directories(Nexis PRIVATE
    src
    ${nfd_SOURCE_DIR}/src/include
)

target_link_libraries(Nexis PRIVATE
    Nexis::Core
    raylib
    imgui
    rlimgui
    nfd
)
