#pragma once

#include <types.hpp>
#include <app_state.hpp>
#include <app_context.hpp>

struct ImFont;

namespace ui
{
struct ProjectExplorer
{
	AppState draw(AppContext &ctx);
};
} // namespace ui
