
#include <raylib.h>
#include <Nexis/core.h>

int main()
{
	InitWindow(800, 600, "Titel");

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
