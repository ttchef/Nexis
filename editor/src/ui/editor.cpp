
#include <asset_manager.hpp>
#include <project.hpp>
#include <ui/editor.hpp>
#include <ui/widgets.hpp>
#include <utils.hpp>

#include <algorithm>
#include <iostream>
#include <cstring>

#include <imgui.h>
#include <imgui_internal.h>
// #include <misc/cpp/imgui_stdlib.h>
#include <nfd.hpp>

static const char *emitter_blending_names(NxBlending blending)
{
    switch (blending)
    {
    case NxBlendingOpaque:
        return "Opaque";
    case NxBlendingAdditive:
        return "Additive";
    default:
        return "Unknown";
    }
}

static void setup_editor_dockspace()
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
    ImGui::Begin("EditorDock", nullptr, host_flags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspace_id = ImGui::GetID("EditorDockspace");

    if (ImGui::DockBuilderGetNode(dockspace_id) == nullptr)
    {
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->WorkSize);

        ImGuiID dock_viewport_id = dockspace_id;
        ImGuiID dock_settings_id = ImGui::DockBuilderSplitNode(dock_viewport_id, ImGuiDir_Right, 0.30f, nullptr, &dock_viewport_id);
        ImGuiID dock_assets_id   = ImGui::DockBuilderSplitNode(dock_viewport_id, ImGuiDir_Down, 0.30f, nullptr, &dock_viewport_id);

        ImGui::DockBuilderDockWindow("Settings", dock_settings_id);
        ImGui::DockBuilderDockWindow("Viewport", dock_viewport_id);
        ImGui::DockBuilderDockWindow("Assets", dock_assets_id);

        ImGui::DockBuilderFinish(dockspace_id);
    }

    ImGui::DockSpace(dockspace_id);
    ImGui::End();
}

namespace ui
{
Editor::Editor()
{
    scene                = LoadRenderTexture(2560, 1440);
    scene_texture_active = false;
    add_emitter          = NxEmitter{};
}

AppState Editor::draw(AppContext &ctx)
{
    AppState state = AppState::Editor;

    setup_editor_dockspace();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Home"))
            {
                utils::load_projects(ctx.projects);
                state        = AppState::ProjectExplorer;
                *ctx.project = Project();
            }
            if (ImGui::MenuItem("Save"))
            {
                // TODO: Implement
                assert(0);
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Exit"))
            {
                state = AppState::Exit;
            }

            ImGui::EndMenu();
        }

        ImGui::TextDisabled("Project: %s", ctx.project->header.file_name.c_str());

        ImGui::EndMainMenuBar();
    }

    ImGui::Begin("Settings");

    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
    {
        ImGui::OpenPopup("AddEmitterPopup");
    }

    if (ImGui::BeginPopup("AddEmitterPopup"))
    {
        bool can_create = std::strlen(add_emitter.name) == 0;
        ImGui::InputTextWithHint("##Emitter Name", "Emitter name...", add_emitter.name, sizeof(add_emitter.name));

        ImGui::BeginDisabled(!can_create);
        if (ImGui::Button("Create Emitter"))
        {
            Nx_system_add_emitter(ctx.system, &add_emitter);
            add_emitter = NxEmitter{};
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndDisabled();

        ImGui::EndPopup();
    }

    for (u32 i = 0; i < Nx_system_emitter_count(ctx.system); i++)
    {
        auto &e = ctx.project->system.emitters[i];

        ImGui::PushID(i);

        ImGui::Checkbox("##enabled", &e.enabled);
        ImGui::SameLine();

        if (ImGui::CollapsingHeader(e.name))
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f * ctx.dpi_scale);
            ImGui::BeginChild("emitter_container", ImVec2(0, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY);

            ImGui::SeparatorText("Motion");

            ImGui::SeparatorText("Appearance");

            ImGui::Combo("Blend Mode", reinterpret_cast<i32 *>(&e.blending), emitter_blending_names(e.blending), ARRAY_COUNT(emitter_blending_names(e.blending)));

            ImVec2 texture_size = ImVec2(100.0f * ctx.dpi_scale, 100.0f * ctx.dpi_scale);
            if (ImGui::BeginChild("texture", texture_size, ImGuiChildFlags_Borders))
            {
                auto tex = ctx.asset_manager->get_texture_handle(e.texture);
                if (tex)
                {
                    ImGui::Image(static_cast<ImTextureRef>(tex.value().id), ImGui::GetContentRegionAvail());
                }
                else
                {
                    ImGui::Dummy(ImGui::GetContentRegionAvail());
                }
                if (ImGui::BeginDragDropTarget())
                {
                    if (auto payload = ImGui::AcceptDragDropPayload("asset_texture"))
                    {
                        if (payload->DataSize == sizeof(asset::TextureHandle))
                        {
                            e.texture = *static_cast<asset::TextureHandle *>(payload->Data);
                        }
                    }
                    ImGui::EndDragDropTarget();
                }
            }
            ImGui::EndChild();
            ImGui::EndChild();
            ImGui::PopStyleVar();
        }
        ImGui::PopID();
    }

    ImGui::End();

    ImGui::Begin("Viewport");

    ImVec2 available = ImGui::GetContentRegionAvail();

    f32 aspect = scene.texture.width / static_cast<f32>(scene.texture.height);
    f32 width  = available.x;
    f32 height = width / aspect;

    if (height > available.y)
    {
        height = available.y;
        width  = height * aspect;
    }

    ImVec2 size{width, height};

    ImGui::Image(static_cast<ImTextureRef>(scene.texture.id), size, ImVec2(0, 1), ImVec2(1, 0));

    if (ImGui::IsItemClicked(ImGuiMouseButton_Middle))
    {
        scene_texture_active = true;
    }
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Middle))
    {
        scene_texture_active = false;
    }
    ImGui::End();

    ImGui::Begin("Assets");

    const f32 item_width  = 150.0f * ctx.dpi_scale;
    const f32 item_height = 150.0f * ctx.dpi_scale;
    const f32 spacing     = ImGui::GetStyle().ItemSpacing.x;

    f32 available_width = ImGui::GetContentRegionAvail().x;

    u32 columns = std::max(static_cast<u32>((available_width + spacing) / (item_width + spacing)), 1u);
    u32 index   = 0;

    for (const auto &[key, tex] : ctx.asset_manager->textures)
    {
        ImGui::PushID(index);

        ImGui::BeginChild("texture", ImVec2(item_width, item_height), ImGuiChildFlags_Borders);

        ImVec2 size = ImGui::GetContentRegionAvail();

        ImGui::InvisibleButton("drag_texture", size);
        if (ImGui::BeginDragDropSource())
        {
            auto hash = tex.hash();
            ImGui::SetDragDropPayload("asset_texture", &hash, sizeof(hash));
            ImGui::Image((ImTextureRef)tex.handle.id, ImVec2(64, 64));
            ImGui::EndDragDropSource();
        }

        ImDrawList *draw_list = ImGui::GetWindowDrawList();

        ImVec2 min = ImGui::GetItemRectMin();
        ImVec2 max = ImGui::GetItemRectMax();

        draw_list->AddImage(
            (ImTextureID)tex.handle.id,
            min,
            max);
        if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Right))
        {
            ImGui::SetTooltip("%s", tex.path.c_str());
        }

        ImGui::EndChild();

        ImGui::PopID();
        ++index;

        if (index % columns != 0)
        {
            ImGui::SameLine();
        }
    }

    // Add texture button
    if (ImGui::BeginChild("add_texture", ImVec2(item_width, item_height), ImGuiChildFlags_Borders))
    {
        if (ImGui::Button("Add", ImVec2(-FLT_MIN, -FLT_MIN)))
        {
            NFD::UniquePathN  out_path;
            nfdu8filteritem_t filters[] = {
                {"Pictues", "png"},
            };

            if (NFD::OpenDialog(out_path, filters, ARRAY_COUNT(filters)) != NFD_OKAY)
            {
                std::cout << "Filedialog error: " << NFD::GetError() << std::endl;
                std::exit(1);
            }
            else
            {
                ctx.asset_manager->load_texture(out_path.get());
            }
        }
    }
    ImGui::EndChild();

    ImGui::End();

    return state;
}
} // namespace ui
