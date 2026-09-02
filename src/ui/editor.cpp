
#include <ui/editor.hpp>
#include <ui/widgets.hpp>

#include <iostream>

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include <nfd.hpp>

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

        ImGui::DockBuilderDockWindow("Settings", dock_settings_id);
        ImGui::DockBuilderDockWindow("Viewport", dock_viewport_id);

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
    add_emitter          = particle::Emitter{};
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
                state = AppState::ProjectExplorer;
                *ctx.project = Project();
            }
            if (ImGui::MenuItem("Save"))
            {
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Exit"))
            {
            }

            ImGui::EndMenu();
        }

        ImGui::TextDisabled("Project %s", ctx.project->file_name.c_str());

        ImGui::EndMainMenuBar();
    }

    ImGui::Begin("Settings");

    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
    {
        ImGui::OpenPopup("AddEmitterPopup");
    }

    if (ImGui::BeginPopup("AddEmitterPopup"))
    {
        bool can_create = !add_emitter.name.empty();
        ImGui::InputTextWithHint("##Emitter Name", "Emitter name...", &add_emitter.name);

        ImGui::BeginDisabled(!can_create);
        if (ImGui::Button("Create Emitter"))
        {
            ctx.project->system.emitters.push_back(add_emitter);
            add_emitter = particle::Emitter();
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndDisabled();

        ImGui::EndPopup();
    }

    for (u32 i = 0; i < ctx.project->system.emitters.size(); i++)
    {
        auto &e = ctx.project->system.emitters[i];

        ImGui::PushID(i);

        ImGui::Checkbox("##enabled", &e.enabled);
        ImGui::SameLine();

        if (ImGui::CollapsingHeader(e.name.c_str()))
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f * ctx.dpi_scale);
            ImGui::BeginChild("emitter_container", ImVec2(0, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY);

            ImGui::SeparatorText("Motion");

            ImGui::DragFloat("Emitter speed", &e.speed, 0.05f);
            e.speed = std::max(e.speed, 0.0f);

            widgets::DragRandomFloat("Particle lifetime", e.lifetime);

            ImGui::DragFloat3("Direction", &e.direction.x, 0.05f);

            ImGui::Checkbox("Draw Forcefield", &e.render_force_field);

            i32 force_to_remove = -1;
            for (u32 i = 0; i < e.forces.size(); i++)
            {
                auto &force = e.forces[i];

                ImGui::PushID(i);

                ImGui::Checkbox("##enabled", &force.enabled);
                ImGui::SameLine();

                bool open = ImGui::TreeNodeEx("##node", ImGuiTreeNodeFlags_None, "%s", force.name());

                ImGui::SameLine(ImGui::GetContentRegionAvail().x - 12.0f * ctx.dpi_scale);
                if (ImGui::SmallButton("x"))
                {
                    force_to_remove = i;
                }

                if (open)
                {
                    std::visit([](auto &&value)
                               {
                                   using T = std::decay_t<decltype(value)>;

                                   if constexpr (std::is_same_v<T, particle::ForceGravity>)
                                   {
                                        ImGui::DragFloat3("Direction", &value.direction.x, 0.05f);
                                        ImGui::DragFloat("Strength", &value.strength, 0.05f);
                                   }
                                   else if constexpr (std::is_same_v<T, particle::ForcePoint>)
                                   {
                                       ImGui::DragFloat3("Position", &value.pos.x, 0.05f);
                                       ImGui::DragFloat("Strength", &value.strength, 0.05f);
                                       ImGui::DragFloat("Falloff Radius", &value.falloff_radius, 0.05f);
                                   } },
                               force.type);
                    ImGui::TreePop();
                }

                ImGui::PopID();
            }

            if (force_to_remove >= 0)
            {
                e.forces.erase(e.forces.begin() + force_to_remove);
            }

            if (ImGui::Button("Add Force", ImVec2(-FLT_MIN, 0)))
            {
                ImGui::OpenPopup("AddForcePopup");
            }

            if (ImGui::BeginPopup("AddForcePopup"))
            {
                if (ImGui::Selectable("Gravity"))
                {
                    e.forces.push_back({particle::ForceGravity{}, true});
                }
                if (ImGui::Selectable("Point"))
                {
                    e.forces.push_back({particle::ForcePoint{}, true});
                }
                ImGui::EndPopup();
            }

            ImGui::SeparatorText("Appearance");

            ImGui::ColorEdit4("Birth Color", static_cast<f32 *>(&e.birth_color.x), ImGuiColorEditFlags_AlphaBar);
            ImGui::ColorEdit4("Death Color", static_cast<f32 *>(&e.death_color.x), ImGuiColorEditFlags_AlphaBar);

            widgets::DragRandomFloat("Birth Size", e.birth_size);
            widgets::DragRandomFloat("Death Size", e.death_size);

            ImGui::Combo("Blend Mode", reinterpret_cast<i32 *>(&e.blending), particle::emitter_blending_names, ARRAY_COUNT(particle::emitter_blending_names));

            if (ImGui::Button("Load Texture"))
            {
                NFD::UniquePathN  out_path;
                nfdu8filteritem_t filters[] = {
                    {"Pictues", "png"},
                };

                if (NFD::OpenDialog(out_path, filters, ARRAY_COUNT(filters)) != NFD_OKAY)
                {
                    std::cout << "Filedialog error: " << NFD::GetError() << std::endl;
                }
                else
                {
                    if (e.texture && e.texture->id != 0)
                    {
                        UnloadTexture(e.texture.value());
                    }
                    e.texture_path = out_path.get();
                    auto image     = LoadImage(e.texture_path.c_str());
                    e.texture      = LoadTextureFromImage(image);
                    UnloadImage(image);
                }
            }
            if (e.texture)
            {
                ImGui::SameLine();
                if (ImGui::Button("Unload Texture"))
                {
                    UnloadTexture(e.texture.value());
                    e.texture = std::optional<Texture2D>{};
                    e.texture_path.clear();
                }
            }
            ImGui::SameLine();
            ImGui::TextDisabled("Path: %s", e.texture_path.c_str());
            if (ImGui::IsItemHovered() && !e.texture_path.empty())
            {
                ImGui::SetTooltip("%s", e.texture_path.c_str());
            }

            ImGui::SeparatorText("Emitter Shape");

            const char *current = "Unkown";

            if (std::holds_alternative<particle::EmitterShapeRectangle>(e.shape))
            {
                current = "Rectangle";
            }
            if (ImGui::BeginCombo("Emitter Shape", current))
            {
                if (ImGui::Selectable("Rectangle", std::holds_alternative<particle::EmitterShapeRectangle>(e.shape)))
                {
                    e.shape = particle::EmitterShapeRectangle{};
                }

                ImGui::EndCombo();
            }
            ImGui::Checkbox("Render Emitter Shape", &e.render_shape);

            std::visit([](auto &value)
                       {
                using T = std::decay_t<decltype(value)>;

                if constexpr (std::is_same_v<T, particle::EmitterShapeRectangle>) {
                    ImGui::DragFloat2("##pos", &value.pos.x, 0.05f);
                    ImGui::SameLine();
                    ImGui::Text("Position");
                    ImGui::DragFloat2("##size", &value.size.x, 0.05f);
                    ImGui::SameLine();
                    ImGui::Text("Size    ");

                     value.size.max(0.0f);
                } }, e.shape);
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

    ImGui::Image(
        (ImTextureID)(uintptr_t)scene.texture.id,
        size,
        ImVec2(0, 1),
        ImVec2(1, 0));

    if (ImGui::IsItemClicked(ImGuiMouseButton_Middle))
    {
        scene_texture_active = true;
    }
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Middle))
    {
        scene_texture_active = false;
    }
    ImGui::End();

    return state;
}
} // namespace ui
