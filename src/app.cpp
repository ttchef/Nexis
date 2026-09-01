
#include <app.hpp>
#include <globals.hpp>
#include <ui.hpp>

#include <nfd.hpp>

static f32 scale_from_monitor()
{
    i32 monitor  = GetCurrentMonitor();
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

App::App()
 : window(), ui(window.dpi_scale)
{
    // Native file dialog
    NFD::Init();

    // TODO: Absolute paths
    grid_shader.handle         = LoadShader("src/shaders/grid.vert", "src/shaders/grid.frag");
    grid_shader.camera_pos_loc = GetShaderLocation(grid_shader.handle, "camera_pos");

    camera = SceneCamera{};
}

App::~App()
{
    NFD::Quit();
}

bool App::should_close()
{
    return WindowShouldClose();
}

void App::update()
{
    global.update_window_size(GetScreenWidth(), GetScreenHeight());
    f32 dt = GetFrameTime();

    camera.update(ui.scene_texture_active, dt);
    SetShaderValue(grid_shader.handle, grid_shader.camera_pos_loc, &camera.raylib.position, SHADER_UNIFORM_VEC3);

    system.update(dt);
}

void App::draw()
{
    BeginTextureMode(ui.scene);
    ClearBackground(BLACK);

    BeginMode3D(camera.raylib);

    BeginShaderMode(grid_shader.handle);
    DrawPlane({0.0f, 0.0f, 0.0f}, {100.0f, 100.0f}, RED);

    EndShaderMode();

    system.draw(camera);

    EndMode3D();
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);
    ui.compute(system.emitters);
    EndDrawing();
}
