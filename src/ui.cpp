
#include <ui.hpp>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.cpp>
#include <misc/cpp/imgui_stdlib.h>
#include <nfd.hpp>
#include <rlImGui.h>

#include <iostream>

// https://github.com/ocornut/imgui/issues/707
static void setup_style(ImGuiStyle &style)
{
    ImVec4 *colors = style.Colors;

    // --- 1. Sizing and Spacing (Clean & Balanced) ---
    style.WindowPadding = ImVec2(10.0f, 10.0f);
    style.FramePadding  = ImVec2(6.0f, 4.0f);
    style.ItemSpacing   = ImVec2(8.0f, 6.0f);
    style.ScrollbarSize = 14.0f;
    style.GrabMinSize   = 12.0f;

    // --- 2. Borders & Rounding ---
    style.WindowRounding    = 6.0f;
    style.FrameRounding     = 4.0f;
    style.PopupRounding     = 4.0f;
    style.ScrollbarRounding = 12.0f;
    style.GrabRounding      = 4.0f;
    style.TabRounding       = 4.0f;

    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize  = 1.0f;

    // --- 3. The Dracula Color Palette ---
    // Background: #282a36 | Selection: #44475a | Foreground: #f8f8f2
    // Comment: #6272a4    | Cyan: #8be9fd      | Green: #50fa7b
    // Orange: #ffb86c     | Pink: #ff79c6      | Purple: #bd93f9
    // Red: #ff5555        | Yellow: #f1fa8c

    // Text
    colors[ImGuiCol_Text]         = ImVec4(0.97f, 0.97f, 0.95f, 1.00f); // #f8f8f2
    colors[ImGuiCol_TextDisabled] = ImVec4(0.38f, 0.45f, 0.64f, 1.00f); // #6272a4

    // Backgrounds
    colors[ImGuiCol_WindowBg] = ImVec4(0.16f, 0.16f, 0.21f, 1.00f); // #282a36
    colors[ImGuiCol_ChildBg]  = ImVec4(0.16f, 0.16f, 0.21f, 0.00f);
    colors[ImGuiCol_PopupBg]  = ImVec4(0.16f, 0.16f, 0.21f, 0.96f);

    // Borders
    colors[ImGuiCol_Border]       = ImVec4(0.27f, 0.28f, 0.35f, 1.00f); // #44475a
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    // Frames (Inputs, etc.)
    colors[ImGuiCol_FrameBg]        = ImVec4(0.27f, 0.28f, 0.35f, 1.00f); // #44475a
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.45f, 0.64f, 1.00f); // #6272a4
    colors[ImGuiCol_FrameBgActive]  = ImVec4(0.48f, 0.55f, 0.74f, 1.00f);

    // Title Bars
    colors[ImGuiCol_TitleBg]          = ImVec4(0.13f, 0.14f, 0.18f, 1.00f); // Darker
    colors[ImGuiCol_TitleBgActive]    = ImVec4(0.16f, 0.16f, 0.21f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.13f, 0.14f, 0.18f, 1.00f);

    // Menus
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.13f, 0.14f, 0.18f, 1.00f);

    // Scrollbars
    colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.16f, 0.16f, 0.21f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.38f, 0.45f, 0.64f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.48f, 0.55f, 0.74f, 1.00f);

    // Interactables
    colors[ImGuiCol_CheckMark]        = ImVec4(0.31f, 0.98f, 0.48f, 1.00f); // #50fa7b (Green)
    colors[ImGuiCol_SliderGrab]       = ImVec4(0.74f, 0.58f, 0.98f, 1.00f); // #bd93f9 (Purple)
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.84f, 0.68f, 1.00f, 1.00f);
    colors[ImGuiCol_Button]           = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
    colors[ImGuiCol_ButtonHovered]    = ImVec4(1.00f, 0.47f, 0.78f, 1.00f); // #ff79c6 (Pink)
    colors[ImGuiCol_ButtonActive]     = ImVec4(0.80f, 0.37f, 0.62f, 1.00f);
    colors[ImGuiCol_Header]           = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
    colors[ImGuiCol_HeaderHovered]    = ImVec4(0.38f, 0.45f, 0.64f, 1.00f);
    colors[ImGuiCol_HeaderActive]     = ImVec4(0.48f, 0.55f, 0.74f, 1.00f);

    // Tabs
    colors[ImGuiCol_Tab]                = ImVec4(0.16f, 0.16f, 0.21f, 1.00f);
    colors[ImGuiCol_TabHovered]         = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
    colors[ImGuiCol_TabActive]          = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
    colors[ImGuiCol_TabUnfocused]       = ImVec4(0.13f, 0.14f, 0.18f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.16f, 0.16f, 0.21f, 1.00f);

    // Tables
    colors[ImGuiCol_TableHeaderBg]     = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.38f, 0.45f, 0.64f, 1.00f);
    colors[ImGuiCol_TableBorderLight]  = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);

    // Misc
    colors[ImGuiCol_PlotLines]      = ImVec4(0.55f, 0.91f, 0.99f, 1.00f); // #8be9fd (Cyan)
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
    colors[ImGuiCol_NavHighlight]   = ImVec4(0.74f, 0.58f, 0.98f, 1.00f);

#ifdef IMGUI_HAS_DOCK
    colors[ImGuiCol_DockingPreview] = ImVec4(0.74f, 0.58f, 0.98f, 0.50f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.16f, 0.16f, 0.21f, 1.00f);
#endif
}

ui::Context::Context(f32 dpi_scale)
    : dpi_scale(dpi_scale)
{
    rlImGuiSetup(true);

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGuiStyle &style = ImGui::GetStyle();
    setup_style(style);
    style.ScaleAllSizes(dpi_scale);

    ImFontConfig config;
    config.SizePixels = 16.0f * dpi_scale;
    // TODO: Absolute path
    auto default_font = io.Fonts->AddFontFromFileTTF("assets/fonts/ovelion.ttf", 16.0f * dpi_scale, &config);
    io.FontDefault    = default_font;
    io.Fonts->Build();

    scene = LoadRenderTexture(2560, 1440);
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
        ImGui::OpenPopup("AddEmitterPopup");
    }

    if (ImGui::BeginPopup("AddEmitterPopup"))
    {
        bool can_create = !add_emitter.name.empty();
        ImGui::InputTextWithHint("##Emitter Name", "Emitter name...", &add_emitter.name);

        ImGui::BeginDisabled(!can_create);
        if (ImGui::Button("Create Emitter"))
        {
            emitters.push_back(add_emitter);
            add_emitter = Emitter();
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndDisabled();

        ImGui::EndPopup();
    }

    for (u32 i = 0; i < emitters.size(); i++)
    {
        auto &e = emitters[i];

        ImGui::PushID(i);

        if (ImGui::CollapsingHeader(e.name.c_str()))
        {
            ImGui::SeparatorText("Motion");

            ImGui::DragFloat("Emitter speed", &e.speed, 0.05f);
            e.speed = std::max(e.speed, 0.0f);

            ImGui::DragFloat("Particle lifetime", &e.lifetime, 0.05f);
            e.lifetime = std::max(e.lifetime, 0.0f);

            ImGui::DragFloat3("Direction", &e.direction.x, 0.05f);

            ImGui::SeparatorText("Appearance");

            ImGui::ColorEdit4("Birth Color", static_cast<f32 *>(&e.birth_color.x));
            ImGui::ColorEdit4("Death Color", static_cast<f32 *>(&e.death_color.x));

            ImGui::DragFloat("Birth Size", &e.birth_size, 0.005f);
            e.birth_size = std::max(e.birth_size, 0.0f);

            ImGui::DragFloat("Death Size", &e.death_size, 0.005f);
            e.death_size = std::max(e.death_size, 0.0f);

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
                    ImGui::DragFloat2("##pos", &value.pos.x, 0.05f);
                    ImGui::SameLine();
                    ImGui::Text("Position");
                    ImGui::DragFloat2("##size", &value.size.x, 0.05f);
                    ImGui::SameLine();
                    ImGui::Text("Size    ");

                     value.size.max(0.0f);
                } }, e.shape);
        }
        ImGui::PopID();
    }

    ImGui::End();

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
