#pragma once

#include <types.hpp>
#include <app_state.hpp>

struct ImFont;

namespace ui
{
struct ProjectExplorer
{
	AppState draw(f32 dpi_scale, ImFont *header_font);
};
} // namespace ui
