
#include <app.hpp>
#include <ui/context.hpp>
#include <utils.hpp>

#include <nfd.hpp>

App::App()
    : state(AppState::ProjectExplorer), window(), ui(window.dpi_scale)
{
    // Native file dialog
    NFD::Init();

    utils::load_projects(&projects);

    grid_shader.handle         = LoadShader(utils::path_abs("shaders/grid.vert").c_str(), utils::path_abs("shaders/grid.frag").c_str());
    grid_shader.camera_pos_loc = GetShaderLocation(grid_shader.handle, "camera_pos");

    Nx_system_create(&system);

    camera = SceneCamera{};
}

App::~App()
{
    Nx_system_destroy(&system);
    NFD::Quit();
}

bool App::should_close()
{
    return WindowShouldClose() || state == AppState::Exit;
}

AppContext App::make_context()
{
    return AppContext{
        .dpi_scale     = ui.dpi_scale,
        .normal_font   = ui.normal_font,
        .mdedium_font  = ui.medium_font,
        .header_font   = ui.header_font,
        .project       = &project,
        .projects      = &projects,
        .camera        = &camera,
        .asset_manager = &asset_manager,
    };
}

void App::update()
{
    AppContext ctx = make_context();
    window.update();
    f32 dt = GetFrameTime();

    camera.update(ui.editor.scene_texture_active, dt);
    SetShaderValue(grid_shader.handle, grid_shader.camera_pos_loc, &camera.raylib.position, SHADER_UNIFORM_VEC3);

    if (state == AppState::Editor)
    {
        Nx_system_update_emitters(&system, dt);
    }
}

void App::draw()
{
    AppContext ctx = make_context();
    if (state == AppState::Editor)
    {
        BeginTextureMode(ui.editor.scene);
        ClearBackground(BLACK);

        BeginMode3D(camera.raylib);

        BeginShaderMode(grid_shader.handle);
        DrawPlane({0.0f, 0.0f, 0.0f}, {100.0f, 100.0f}, RED);

        EndShaderMode();

        EndMode3D();
        EndTextureMode();
    }

    BeginDrawing();
    ClearBackground(BLACK);
    state = ui.draw(state, ctx);
    EndDrawing();
}
