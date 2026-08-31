
#include <ui.hpp>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.cpp>
#include <misc/cpp/imgui_stdlib.h>
#include <rlImGui.h>
#include <nfd.hpp>

#include <iostream>

ui::Context::Context()
{
    rlImGuiSetup(true);

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    scene             = LoadRenderTexture(2560, 1440);
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

    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
    {
        show_options_menu = true;
    }

    for (u32 i = 0; i < emitters.size(); i++)
    {
        auto &e = emitters[i];

        ImGui::PushID(i);

        if (ImGui::CollapsingHeader(e.name.c_str()))
        {
            ImGui::SetNextItemWidth(100.0f);
            ImGui::DragFloat("Emitter speed", &e.speed, 0.05f);
            e.speed = std::max(e.speed, 0.0f);

            ImGui::SetNextItemWidth(100.0f);
            ImGui::DragFloat("Particle lifetime", &e.lifetime, 0.05f);
            e.lifetime = std::max(e.lifetime, 0.0f);

            ImGui::ColorEdit4("Birth Color", static_cast<f32 *>(&e.birth_color.x));
            ImGui::ColorEdit4("Death Color", static_cast<f32 *>(&e.death_color.x));

            ImGui::SetNextItemWidth(100.0f);
            ImGui::DragFloat("Birth Size", &e.birth_size, 0.005f);
            e.birth_size = std::max(e.birth_size, 0.0f);

            ImGui::SetNextItemWidth(100.0f);
            ImGui::DragFloat("Death Size", &e.death_size, 0.005f);
            e.death_size = std::max(e.death_size, 0.0f);

            ImGui::DragFloat3("Direction", &e.direction.x, 0.05f);

            const char *current = "Unkown";

            if (std::holds_alternative<EmitterShapeRectangle>(e.shape))
            {
                current = "Rectangle";
            }
            if (ImGui::BeginCombo("Emitter Shape", current))
            {
                if (ImGui::Selectable("Rectangle", std::holds_alternative<EmitterShapeRectangle>(e.shape)))
                {
                    e.shape = EmitterShapeRectangle{};
                }

                ImGui::EndCombo();
            }
            ImGui::Checkbox("Render Emitter Shape", &e.render_shape);

            std::visit([](auto &value)
                       {
                using T = std::decay_t<decltype(value)>;

                if constexpr (std::is_same_v<T, EmitterShapeRectangle>) {
                    ImGui::Text("Position");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.0f);
                    ImGui::DragFloat2("##emitter_shape_rectangle_drag_float_pos", &value.pos.x, 0.05f);
                    ImGui::Text("Size    ");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.0f);
                    ImGui::DragFloat2("##emitter_shape_rectangle_drag_float_size", &value.size.x, 0.05f);

                    value.size.max(0.0f);
                } }, e.shape);

            if (ImGui::Button("Load Texture")) {
                NFD::UniquePathN out_path;
                nfdu8filteritem_t filters[] = {
                    { "Pictues", "png"},
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
                    auto image = LoadImage(e.texture_path.c_str());
                    e.texture = LoadTextureFromImage(image);
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
            ImGui::Text("Path: %s", e.texture_path.c_str());
        }
        ImGui::PopID();
    }

    ImGui::End();

    if (show_options_menu)
    {
        ImGui::Begin("Options", &show_options_menu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking);

        if (ImGui::Button("Create Emitter") && !add_emitter.name.empty())
        {
            emitters.push_back(add_emitter);
            add_emitter       = Emitter();
            show_options_menu = false;
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
