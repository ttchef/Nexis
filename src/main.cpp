
#include <globals.hpp>
#include <particle_system.hpp>
#include <types.hpp>
#include <ui.hpp>

#include <nfd.hpp>
#include <raylib.h>

struct SceneCamera
{
    Camera3D raylib;

    // 3D spherical coordinates
    f32 azimuthal_angle;
    f32 polar_angle;
    f32 rho;

    SceneCamera()
    {
        raylib = {
            .target     = {0.0f, 0.0f, 0.0f},
            .up         = {0.0f, 1.0f, 0.0f},
            .fovy       = 45.0f,
            .projection = CAMERA_PERSPECTIVE,
        };

        azimuthal_angle = 0.0f;
        polar_angle = 0.0f;
        rho = 10.0f;
    }

    void update(f32 dt)
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
        {
            auto delta = GetMouseDelta();
            azimuthal_angle += delta.x * dt;
            polar_angle += delta.y * dt;
        }

        f32 scroll = GetMouseWheelMove();
        rho += -scroll * 300 * dt;

        // Convert spherical coordinates to cartesian coordinates
        f32 x = rho * sin(polar_angle) * cos(azimuthal_angle);
        f32 y = rho * sin(polar_angle) * sin(azimuthal_angle);
        f32 z = rho * cos(polar_angle);

        raylib.position = {x, z, y};
    }
};

i32 main()
{
    NFD::Init();

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(global.window_width, global.window_height, "Nexis");
    SetExitKey(KEY_NULL);

    // TODO: Absolute paths
    Shader grid_shader = LoadShader("src/shaders/grid.vert", "src/shaders/grid.frag");
    
    ui::Context ui{};

    SceneCamera camera;
    std::vector<Emitter> emitters;

    while (!WindowShouldClose())
    {
        global.update_window_size(GetScreenWidth(), GetScreenHeight());
        f32 dt = GetFrameTime();

        camera.update(dt);

        for (auto &e : emitters)
        {
            e.update(dt);
        }

        BeginTextureMode(ui.scene);
        ClearBackground(BLACK);
        BeginMode3D(camera.raylib);

        BeginShaderMode(grid_shader);
        DrawPlane({0.0f, 0.0f, 0.0f}, {10.0f, 10.0f}, RED);
        EndShaderMode();
        
        for (const auto &e : emitters)
        {
            e.draw(camera.raylib);
        }

        EndMode3D();
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);
        ui.compute(emitters);
        EndDrawing();
    }
    CloseWindow();

    NFD::Quit();
    return 0;
}
