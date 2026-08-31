
#include <cstdint>

#define CLAY_IMPLEMENTATION
#include <clay.h>
#include <renderers/raylib/clay_renderer_raylib.c>

#include <ui.hpp>

std::int32_t main() {
	constexpr std::uint32_t start_width = 800;
	constexpr std::uint32_t start_height = 600;
	
	SetTraceLogLevel(LOG_WARNING);
	Clay_Raylib_Initialize(start_width, start_height, "Nexis", FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);

	Font fonts[] = {
		// TODO: Absolite path
		LoadFontEx("assets/fonts/AdwaitaSans-Regular.ttf", 20, nullptr, 0),	
	};

	ui::Context ui{start_width, start_height};

	while (!WindowShouldClose()) {
		auto ui_commands = ui.compute_layout();
		
		BeginDrawing();
		ClearBackground(BLACK);

		Clay_Raylib_Render(ui_commands, fonts);
		
		EndDrawing();
	}
	Clay_Raylib_Close();
	return 0;
}
