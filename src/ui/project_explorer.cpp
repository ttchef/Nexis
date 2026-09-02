
#include <ui/project_explorer.hpp>

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include <raylib.h>

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

        ImGuiID dock_picture_id  = dockspace_id;
        ImGuiID dock_projects_id = ImGui::DockBuilderSplitNode(dock_picture_id, ImGuiDir_Right, 0.50f, nullptr, &dock_picture_id);

        ImGui::DockBuilderDockWindow("Projects", dock_projects_id);
        ImGui::DockBuilderDockWindow("Picture", dock_picture_id);

        ImGui::DockBuilderGetNode(dock_projects_id)->LocalFlags |= ImGuiDockNodeFlags_HiddenTabBar;
        ImGui::DockBuilderGetNode(dock_picture_id)->LocalFlags |= ImGuiDockNodeFlags_HiddenTabBar;

        ImGui::DockBuilderFinish(dockspace_id);
    }

    ImGui::DockSpace(dockspace_id);
    ImGui::End();
}

namespace ui
{
AppState ProjectExplorer::draw(AppContext &ctx)
{
    AppState state = AppState::ProjectExplorer;

    setup_explorer_dockspace();

    ImGui::Begin("Projects", nullptr, ImGuiWindowFlags_NoTitleBar);

    ImGui::PushFont(ctx.header_font);
    ImGui::SeparatorText("Projects");
    ImGui::PopFont();

    for (u32 i = 0; i < ctx.project_files->size(); i++)
    {
        auto &file = ctx.project_files->at(i);
        
        ImGui::PushID(i);
        
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f * ctx.dpi_scale);
        ImGui::BeginChild("project_file", ImVec2(0, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY);

        ImGui::Text("%s", GetFileNameWithoutExt(file.c_str()));

        ImGui::EndChild();
        ImGui::PopStyleVar();
        ImGui::PopID();
    }

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
