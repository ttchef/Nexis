
#include <types.hpp>
#include <ui.hpp>

#include <raylib.h>

i32 main()
{
    constexpr u32 start_width  = 800;
    constexpr u32 start_height = 600;

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(start_width, start_height, "Nexis");

    RenderTexture2D texture = LoadRenderTexture(start_width, start_height);

    ui::Context ui{};

    while (!WindowShouldClose())
    {
        BeginTextureMode(texture);
        DrawCircle(10, 10, 25, WHITE);
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);
        ui.draw(texture.texture);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
