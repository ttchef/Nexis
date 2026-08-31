
#include <ui.hpp>

#include <imgui.h>
#include <rlImGui.h>

ui::Context::Context() {
    rlImGuiSetup(true);

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	
}

ui::Context::~Context() {
	rlImGuiShutdown();
}

void ui::Context::draw(Texture2D scene) {
        rlImGuiBegin();

		ImGui::DockSpaceOverViewport();

        ImGui::Begin("Nexis");

        ImGui::Text("Hello from Dear ImGui!");

        if (ImGui::Button("Click me"))
        {
            // Do something
        }

        ImVec2 size = ImGui::GetContentRegionAvail();

        ImGui::Image(
            (ImTextureID)(uintptr_t)scene.id,
            size,
            ImVec2(0, 1),
            ImVec2(1, 0));

        ImGui::End();

        rlImGuiEnd();
}

