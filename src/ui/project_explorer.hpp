#pragma once

#include <types.hpp>
#include <app_state.hpp>
#include <app_context.hpp>

#include <raylib.h>

struct ImFont;

namespace ui
{
struct ProjectExplorer
{
	Texture2D wallpaper;
	
	ProjectExplorer();
	~ProjectExplorer();
	AppState draw(AppContext &ctx);
};
} // namespace ui
