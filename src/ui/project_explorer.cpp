
#include <ui/project_explorer.hpp>

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

static void setup_explorer_dockspace()
{
    ImGuiViewport *viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags host_flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("ExplorerDock", nullptr, host_flags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspace_id = ImGui::GetID("ExplorerDockspace");

    if (ImGui::DockBuilderGetNode(dockspace_id) == nullptr)
    {
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->WorkSize);

        ImGuiID dock_picture_id = dockspace_id;
        ImGuiID dock_projects_id = ImGui::DockBuilderSplitNode(dock_picture_id, ImGuiDir_Right, 0.50f, nullptr, &dock_picture_id);

        ImGui::DockBuilderDockWindow("Projects", dock_projects_id);
        ImGui::DockBuilderDockWindow("Picture", dock_picture_id);

        ImGui::DockBuilderFinish(dockspace_id);
    }

    ImGui::DockSpace(dockspace_id);
    ImGui::End();
}

namespace ui
{
AppState ProjectExplorer::draw(f32 dpi_scale, ImFont *header_font)
{
    AppState state = AppState::ProjectExplorer;

    setup_explorer_dockspace();

    ImGui::Begin("Projects");

    ImGui::PushFont(header_font);
    ImGui::SeparatorText("Projects");
    ImGui::PopFont();
    
    if (ImGui::Button("Open editor"))
    {
        state = AppState::Editor;
    }

    ImGui::End();

    ImGui::Begin("Picture");

    ImGui::End();

    return state;
}
} // namespace ui
