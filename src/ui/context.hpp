#pragma once

#include <types.hpp>
#include <app_state.hpp>
#include <ui/editor.hpp>
#include <ui/project_explorer.hpp>

struct ImFont;

namespace ui
{
struct Context
{
    ProjectExplorer explorer;
    Editor editor;
    
    f32 dpi_scale;

    ImFont *normal_font;
    ImFont *header_font;

    Context(f32 dpi_scale);
    ~Context();

    AppState draw(particle::System &system, AppState state);
};
} // namespace ui
