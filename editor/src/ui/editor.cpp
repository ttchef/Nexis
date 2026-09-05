
#include <asset_manager.hpp>
#include <project.hpp>
#include <ui/editor.hpp>
#include <ui/widgets.hpp>
#include <utils.hpp>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <ranges>

#include <imgui.h>
#include <imgui_internal.h>
// #include <misc/cpp/imgui_stdlib.h>
#include <nfd.hpp>

static const char *EMITTER_BLENDING_NAMES[] = {
    "Opaque",
    "Additive",
};

static const char *MODULE_QUEUE_TYPE_NAMES[] = {
    "Emitter Update",
};

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
        ImGuiID dock_emitter_id  = ImGui::DockBuilderSplitNode(dock_viewport_id, ImGuiDir_Right, 0.35f, nullptr, &dock_viewport_id);
        ImGuiID dock_module_id   = ImGui::DockBuilderSplitNode(dock_emitter_id, ImGuiDir_Right, 0.50f, nullptr, &dock_emitter_id);
        ImGuiID dock_assets_id   = ImGui::DockBuilderSplitNode(dock_viewport_id, ImGuiDir_Down, 0.30f, nullptr, &dock_viewport_id);

        ImGui::DockBuilderDockWindow("Emitters", dock_emitter_id);
        ImGui::DockBuilderDockWindow("Module", dock_module_id);
        ImGui::DockBuilderDockWindow("Viewport", dock_viewport_id);
        ImGui::DockBuilderDockWindow("Assets", dock_assets_id);

        ImGui::DockBuilderFinish(dockspace_id);
    }

    ImGui::DockSpace(dockspace_id);
    ImGui::End();
}

static AppState setup_menu(AppContext &ctx)
{
    AppState state = AppState::Editor;

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

    return state;
}

static void setup_emitters(AppContext &ctx, NxEmitter &add_emitter, ui::SelectedModule &module)
{
    ImGui::Begin("Emitters");

    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
    {
        ImGui::OpenPopup("AddEmitterPopup");
    }

    if (ImGui::BeginPopup("AddEmitterPopup"))
    {
        ImGui::InputTextWithHint("##Emitter Name", "Emitter name...", add_emitter.config.name, sizeof(add_emitter.config.name));
        bool can_create = std::strlen(add_emitter.config.name) != 0;

        ImGui::BeginDisabled(!can_create);
        if (ImGui::Button("Create Emitter"))
        {
            Nx_system_add_emitter(&ctx.project->system, &add_emitter);
            Nx_emitter_create(&add_emitter);
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndDisabled();

        ImGui::EndPopup();
    }

    for (u32 i = 0; i < Nx_system_emitter_count(&ctx.project->system); i++)
    {
        auto &e = ctx.project->system.emitters[i].config;

        ImGui::PushID(i);

        ImGui::Checkbox("##enabled", &e.enabled);
        ImGui::SameLine();

        if (ImGui::CollapsingHeader(e.name))
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f * ctx.dpi_scale);
            ImGui::BeginChild("emitter_container", ImVec2(0, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY);

            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f * ctx.dpi_scale);
            ImGui::BeginChild("emitter_settings", ImVec2(0, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY);
            if (ImGui::Selectable("Emitter Settings"))
            {
                module = ui::SelectedModule{NxModuleQueue_None, i, true};
            }
            ImGui::EndChild();
            ImGui::PopStyleVar();

            for (u32 j = 0; j < NxModuleQueue_Count; j++)
            {
                ImGui::PushID(j);

                ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f * ctx.dpi_scale);
                ImGui::BeginChild("##module_type", ImVec2(0, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY);

                if (ImGui::Selectable(MODULE_QUEUE_TYPE_NAMES[j]))
                {
                    module = ui::SelectedModule{static_cast<NxModuleQueueIndex>(j), i};
                }

                ImGui::SameLine();

                if (ImGui::Button("Add"))
                {
                    ImGui::OpenPopup("AddModulePopup");
                }
                if (ImGui::BeginPopup("AddModulePopup"))
                {
                    if (ImGui::Button("Add Spawn Rate"))
                    {
                        NxModuleSpawnRate spawn_rate = {
                            .test0 = 67,
                            .test1 = 187,
                        };
                        Nx_modules_add_SpawnRate(&e.modules, static_cast<NxModuleQueueIndex>(j), spawn_rate);
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
                ImGui::EndChild();
                ImGui::PopStyleVar();
                ImGui::PopID();
            }

            ImGui::EndChild();
            ImGui::PopStyleVar();
        }
        ImGui::PopID();
    }

    ImGui::End();
}

static void setup_module(AppContext &ctx, ui::SelectedModule &module)
{
    ImGui::Begin("Module");

    auto &e = ctx.project->system.emitters[module.emitter_index];
    if (module.settings)
    {
        ImGui::TextUnformatted("Settings");
        ImGui::Checkbox("Test", &e.config.enabled);
    }
    else
    {
        switch (module.type)
        {
        case NxModuleQueue_EmitterUpdate:
        {
            ImGui::TextUnformatted("Emitter Update");
        }
        break;
        default:
            break;
        }
    }

    ImGui::End();
}

static void setup_viewport(RenderTexture2D &scene, bool &scene_texture_active)
{
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
}

static void setup_assets(AppContext &ctx)
{
    ImGui::Begin("Assets");

    const f32 item_width  = 150.0f * ctx.dpi_scale;
    const f32 item_height = 150.0f * ctx.dpi_scale;
    const f32 spacing     = ImGui::GetStyle().ItemSpacing.x;

    f32 available_width = ImGui::GetContentRegionAvail().x;

    u32 columns = std::max(static_cast<u32>((available_width + spacing) / (item_width + spacing)), 1u);
    u32 index   = 0;

    for (const auto &tex : ctx.asset_manager->textures | std::views::values)
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

            auto result = NFD::OpenDialog(out_path, filters, ARRAY_COUNT(filters));
            if (result != NFD_OKAY && result != NFD_CANCEL)
            {
                std::cout << "Filedialog error: " << NFD::GetError() << std::endl;
                std::exit(1);
            }
            else if (result != NFD_CANCEL)
            {
                ctx.asset_manager->load_texture(out_path.get());
            }
        }
    }
    ImGui::EndChild();

    ImGui::End();
}

namespace ui
{
Editor::Editor()
{
    scene                = LoadRenderTexture(2560, 1440);
    scene_texture_active = false;
    Nx_emitter_create(&add_emitter);
    module = {static_cast<NxModuleQueueIndex>(-1), 0, false};
}

AppState Editor::draw(AppContext &ctx)
{
    AppState state = AppState::Editor;

    setup_editor_dockspace();
    state = setup_menu(ctx);
    setup_emitters(ctx, add_emitter, module);
    setup_module(ctx, module);
    setup_viewport(scene, scene_texture_active);
    setup_assets(ctx);

    return state;
}
} // namespace ui
