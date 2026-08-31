
#define CLAY_IMPLEMENTATION
#include <clay.h>
#include <renderers/raylib/clay_renderer_raylib.c>

int main() {
	SetTraceLogLevel(LOG_WARNING);
	Clay_Raylib_Initialize(800, 600, "Title", FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);
		DrawCircle(10, 10, 20, WHITE);
		EndDrawing();
	}
	Clay_Raylib_Close();
	return 0;
}
