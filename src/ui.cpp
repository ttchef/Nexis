
#include <ui.hpp>

#include <imgui.h>
#include <rlImGui.h>

ui::Context::Context()
{
    rlImGuiSetup(true);

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    scene = LoadRenderTexture(2560, 1440);
}

ui::Context::~Context()
{
    rlImGuiShutdown();
}

void ui::Context::draw()
{
    rlImGuiBegin();

    ImGui::DockSpaceOverViewport();

    ImGui::Begin("Nexis");

    ImGui::Text("Hello from Dear ImGui!");

    if (ImGui::Button("Click me"))
    {
        // Do something
    }

    ImGui::End();

    ImGui::Begin("Scene");

    ImVec2 available = ImGui::GetContentRegionAvail();

    f32 aspect = scene.texture.width / static_cast<f32>(scene.texture.height);
    f32 width = available.x;
    f32 height = width / aspect;

    if (height > available.x) {
        height = available.y;
        width = height * aspect;
    }

    ImVec2 size{width, height};
    
    ImGui::Image(
        (ImTextureID)(uintptr_t)scene.texture.id,
        size,
        ImVec2(0, 1),
        ImVec2(1, 0));
    ImGui::End();

    rlImGuiEnd();
}
