
#include <ui.hpp>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <misc/cpp/imgui_stdlib.cpp>
#include <rlImGui.h>

ui::Context::Context()
{
    rlImGuiSetup(true);

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    scene = LoadRenderTexture(2560, 1440);
    show_options_menu = false;
}

ui::Context::~Context()
{
    rlImGuiShutdown();
}

void ui::Context::compute(std::vector<Emitter> &emitters)
{
    rlImGuiBegin();

    ImGui::DockSpaceOverViewport();

    ImGui::Begin("Settings");

    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        show_options_menu = true;
    }

    for (auto &e : emitters)
    {
        if (ImGui::CollapsingHeader(e.name.c_str()))
        {
            ImGui::DragFloat("Emitter speed", &e.speed, 0.05f);
            e.speed = std::max(e.speed, 0.0f);

            ImGui::DragFloat("Particle lifetime", &e.lifetime, 0.05f);
            e.lifetime = std::max(e.lifetime, 0.0f);
        }
    }

    ImGui::End();

    if (show_options_menu) {
        ImGui::Begin("Options", &show_options_menu);

        if (ImGui::Button("Create Emitter")) {
            emitters.push_back(add_emitter);
            add_emitter = Emitter();
        }

        ImGui::SameLine();

        ImGui::InputText("##Emitter Name", &add_emitter.name);

        ImGui::End();
    }

    ImGui::Begin("Scene");

    ImVec2 available = ImGui::GetContentRegionAvail();

    f32 aspect = scene.texture.width / static_cast<f32>(scene.texture.height);
    f32 width  = available.x;
    f32 height = width / aspect;

    if (height > available.x)
    {
        height = available.y;
        width  = height * aspect;
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
