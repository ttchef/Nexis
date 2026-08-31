
#include <globals.hpp>
#include <types.hpp>
#include <ui.hpp>
#include <particle_system.hpp>

#include <raylib.h>

i32 main()
{
    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(global.window_width, global.window_height, "Nexis");

    ui::Context ui{};

    Camera3D camera{
        .position   = {-10.0f, 15.0f, -10.0f},
        .target     = {0.0f, 0.0f, 0.0f},
        .up         = {0.0f, 1.0f, 0.0f},
        .fovy       = 45.0f,
        .projection = CAMERA_PERSPECTIVE,
    };

    Emitter emitter{3.0f};

    while (!WindowShouldClose())
    {
        global.update_window_size(GetScreenWidth(), GetScreenHeight());
        f32 dt = GetFrameTime();
        
        UpdateCamera(&camera, CAMERA_ORBITAL);
        emitter.update(dt);

        BeginTextureMode(ui.scene);
        ClearBackground(BLACK);
        BeginMode3D(camera);
        DrawGrid(100, 1.0f);
        emitter.draw();

        EndMode3D();
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);
        ui.draw();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
