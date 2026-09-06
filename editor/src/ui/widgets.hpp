#pragma once

#include <types.hpp>
#include <random.hpp>

#include <algorithm>
#include <cstring>

#include <imgui.h>

namespace ui::widgets
{
static bool SelectableHighlighted(const char *label, const std::string &search, bool selected = false)
{
    ImGui::PushID(label);
    
    bool pressed = ImGui::Selectable("##selectable", selected, 0, ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing()));

    ImVec2 text_pos = ImGui::GetItemRectMin();

    text_pos.x += std::floor(ImGui::GetStyle().FramePadding.x);
    text_pos.y += std::floor(ImGui::GetStyle().FramePadding.y);

    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    const usize label_len = std::strlen(label);
    const usize match_len = std::min(search.size(), label_len);

    if (match_len > 0)
    {
        draw_list->AddText(text_pos, IM_COL32(255, 220, 80, 255), label, label + match_len);
        ImVec2 match_size = ImGui::CalcTextSize(label, label + match_len);
        text_pos.x += std::floor(match_size.x);
    }
    draw_list->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), label + match_len, label + label_len);

    ImGui::PopID();
    return pressed;
}
} // namesapce ui
