
#include <ui.hpp>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.cpp>
#include <misc/cpp/imgui_stdlib.h>
#include <rlImGui.h>

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

    for (auto &e : emitters)
    {
        if (ImGui::CollapsingHeader(e.name.c_str()))
        {
            ImGui::DragFloat("Emitter speed", &e.speed, 0.05f);
            e.speed = std::max(e.speed, 0.0f);

            ImGui::DragFloat("Particle lifetime", &e.lifetime, 0.05f);
            e.lifetime = std::max(e.lifetime, 0.0f);

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

            std::visit([](auto &value){
                using T = std::decay_t<decltype(value)>;

                if constexpr (std::is_same_v<T, EmitterShapeRectangle>) {
                    ImGui::Text("Position");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.0f);
                    ImGui::DragFloat("##emitter_shape_rectangle_drag_float_pos_x", &value.pos.x, 0.05f);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.0f);
                    ImGui::DragFloat("##emitter_shape_rectangle_drag_float_pos_y", &value.pos.y, 0.05f);
                    ImGui::Text("Size");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.0f);
                    ImGui::DragFloat("##emitter_shape_rectangle_drag_float_size_x", &value.size.x, 0.05f);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.0f);
                    ImGui::DragFloat("##emitter_shape_rectangle_drag_float_size_y", &value.size.y, 0.05f);
                }
            }, e.shape);
        }
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
