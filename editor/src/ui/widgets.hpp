#pragma once

#include <types.hpp>
#include <random.hpp>

#include <algorithm>

#include <imgui.h>

namespace ui::widgets
{
static bool DragRandomFloat(const char *label, RandomF32 &r, f32 speed = 0.05f, f32 min = 0.0f)
{
    bool changed = false;

    ImGui::PushID(label);

    ImGui::SetNextItemWidth(ImGui::CalcItemWidth() * 0.55f);
    changed |= ImGui::DragFloat("##base", &r.value, speed);
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Right))
    {
        ImGui::SetTooltip("%s", "Actual value");
    }

    ImGui::SameLine(0.0f, 4.0f);
    ImGui::SetNextItemWidth(ImGui::CalcItemWidth() * 0.35f);
    changed |= ImGui::DragFloat("##offset", &r.offset, speed);
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Right))
    {
        ImGui::SetTooltip("%s", "Offset range for random values");
    }

    ImGui::SameLine();
    ImGui::TextUnformatted(label);

    r.value  = std::max(r.value, min);
    r.offset = std::max(r.offset, 0.0f);

    ImGui::PopID();

    return changed;
}	
} // namesapce ui
