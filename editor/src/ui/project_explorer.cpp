
#include <ui/project_explorer.hpp>
#include <utils.hpp>
#include <project.hpp>

#include <ctime>

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include <raylib.h>

// Not thread save right now ik but right now it doestn matter
static const char *format_time(i64 time)
{
    static char buffer[32];

    std::time_t mod_time = static_cast<std::time_t>(time);
    std::tm    *tm_info  = std::localtime(&mod_time);
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", tm_info);

    return buffer;
}

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

        ImGuiID dock_projects_id = dockspace_id;

        ImGui::DockBuilderDockWindow("Projects", dock_projects_id);
        ImGui::DockBuilderGetNode(dock_projects_id)->LocalFlags |= ImGuiDockNodeFlags_HiddenTabBar;

        ImGui::DockBuilderFinish(dockspace_id);
    }

    ImGui::DockSpace(dockspace_id);
    ImGui::End();
}

namespace ui
{
ProjectExplorer::ProjectExplorer()
{
    wallpaper     = LoadTexture(utils::path_abs("assets/textures/wallpaper.png").c_str());
    hovered_child = -1;
}

ProjectExplorer::~ProjectExplorer()
{
    if (wallpaper.id)
    {
        UnloadTexture(wallpaper);
    }
}

AppState ProjectExplorer::draw(AppContext &ctx)
{
    AppState state = AppState::ProjectExplorer;

    setup_explorer_dockspace();

    ImGui::Begin("Projects", nullptr, ImGuiWindowFlags_NoTitleBar);

    if (ImGui::BeginTable("layout", 2, ImGuiTableFlags_SizingStretchSame))
    {
        ImGui::TableSetupColumn("wallpaper");
        ImGui::TableSetupColumn("projects");

        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);

        const f32 image_aspect = static_cast<f32>(wallpaper.width) / static_cast<f32>(wallpaper.height);
        ImVec2    available    = ImGui::GetContentRegionAvail();

        f32 image_width  = available.x;
        f32 image_height = image_width / image_aspect;

        if (image_height > available.y)
        {
            image_height = available.y;
            image_width  = image_height * image_aspect;
        }

        ImGui::Image((ImTextureID)wallpaper.id, ImVec2(image_width, image_height));

        ImGui::TableSetColumnIndex(1);
        ImGui::PushFont(ctx.header_font);
        ImGui::SeparatorText("Projects");
        ImGui::PopFont();

        ImGui::PushFont(ctx.mdedium_font);
        if (ImGui::Button("New Project", ImVec2(-FLT_MIN, 0)))
        {
            ImGui::OpenPopup("add_project");
        }
        ImGui::PopFont();

        if (ImGui::BeginPopup("add_project"))
        {
            ImGui::BeginDisabled(ctx.project->file_name.empty());
            if (ImGui::Button("Create Project"))
            {
                ctx.project->file_path = std::format("{}/{}{}", global.project_path, ctx.project->file_name, NEXIS_PF_EX);
                state                  = AppState::Editor;
            }
            ImGui::EndDisabled();

            ImGui::SameLine();
            ImGui::InputTextWithHint("##name", "Enter project name", &ctx.project->file_name);

            ImGui::EndPopup();
        }

        i32 project_to_remove = -1;
        for (u32 i = 0; i < ctx.projects->size(); i++)
        {
            auto &project = ctx.projects->at(i);

            ImGui::PushID(i);

            if (hovered_child == i)
            {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_TabHovered));
            }
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f * ctx.dpi_scale);
            ImGui::BeginChild("project_file", ImVec2(0, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY);

            ImGui::PushFont(ctx.mdedium_font);
            ImGui::Text("%s", project.file_name.c_str());
            ImGui::SameLine();

            f32 button_width = ImGui::CalcTextSize("Delete").x + ImGui::GetStyle().FramePadding.x * 2.0f;

            ImGui::SetCursorPosX(
                ImGui::GetWindowWidth() - button_width - ImGui::GetStyle().WindowPadding.x);

            if (ImGui::Button("Delete"))
            {
                project_to_remove = i;
            }

            std::string last_modified =
                std::string("Last modified: ") + format_time(project.mod_time);
            ImGui::TextDisabled("%s", last_modified.c_str());

            ImGui::PopFont();

            ImGui::EndChild();

            if (hovered_child == i)
            {
                hovered_child = -1;
                ImGui::PopStyleColor();
            }
            if (ImGui::IsItemHovered())
            {
                hovered_child = i;
            }

            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                *ctx.project = project;
                state        = AppState::Editor;
            }

            ImGui::PopStyleVar();
            ImGui::PopID();
        }

        if (project_to_remove != -1)
        {
            ctx.projects->at(project_to_remove).remove();
            ctx.projects->erase(ctx.projects->begin() + project_to_remove);
        }

        ImGui::EndTable();
    }

    ImGui::End();

    return state;
}
} // namespace ui
