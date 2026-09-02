
#include <ui/project_explorer.hpp>

#include <imgui.h>

namespace ui
{
AppState ProjectExplorer::draw(f32 dpi_scale)
{
    AppState state = AppState::ProjectExplorer;

    ImGui::Begin("Something");

    if (ImGui::Button("Open editor"))
    {
        state = AppState::Editor;
    }

    ImGui::End();

    return state;
}
} // namespace ui
