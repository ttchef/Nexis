
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
        polar_angle = 45.0f;
        rho = 10.0f;
    }

    void update(f32 dt)
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
        {
            auto delta = GetMouseDelta();
            constexpr f32 sensitivity = 0.1f;
            azimuthal_angle += delta.x * sensitivity;
            polar_angle += -delta.y * sensitivity;
        }

        f32 scroll = GetMouseWheelMove();
        constexpr f32 zoom_speed = 1.0f;
        rho += -scroll * zoom_speed;

        rho = std::max(rho, 0.1f);
        polar_angle = std::clamp(polar_angle, 0.01f, 89.99f);

        f32 az = azimuthal_angle * DEG2RAD;
        f32 pol = polar_angle * DEG2RAD;

        // Convert spherical coordinates to cartesian coordinates
        f32 x = rho * sin(pol) * cos(az);
        f32 y = rho * sin(pol) * sin(az);
        f32 z = rho * cos(pol);

        // Physics coordinate system
        raylib.position = {x, z, y};
    }
};

static f32 scale_from_monitor()
{
    i32 monitor = GetCurrentMonitor();
    i32 width_px = GetMonitorWidth(monitor);
    i32 width_mm = GetMonitorPhysicalWidth(monitor);

    if (width_mm <= 0)
    {
        return 1.0f;
    }

    f32 width_inches = width_mm / 25.4f;
    f32 dots_per_inc = width_px / width_inches;
    return dots_per_inc / 96.0f;
}

i32 main()
{
    NFD::Init();

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    InitWindow(global.window_width, global.window_height, "Nexis");
    SetExitKey(KEY_NULL);

    f32 dpi_scale = scale_from_monitor();

    // TODO: Absolute paths
    Shader grid_shader = LoadShader("src/shaders/grid.vert", "src/shaders/grid.frag");
    int camera_pos_loc = GetShaderLocation(grid_shader, "camera_pos");
    
    ui::Context ui{dpi_scale};

    SceneCamera camera;
    std::vector<Emitter> emitters;

    while (!WindowShouldClose())
    {
        global.update_window_size(GetScreenWidth(), GetScreenHeight());
        f32 dt = GetFrameTime();

        camera.update(dt);
        SetShaderValue(grid_shader, camera_pos_loc, &camera.raylib.position, SHADER_UNIFORM_VEC3);

        for (auto &e : emitters)
        {
            e.update(dt);
        }

        BeginTextureMode(ui.scene);
        ClearBackground(BLACK);
        BeginMode3D(camera.raylib);

        BeginShaderMode(grid_shader);
        DrawPlane({0.0f, 0.0f, 0.0f}, {100.0f, 100.0f}, RED);
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
