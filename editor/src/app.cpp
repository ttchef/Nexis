
#include <app.hpp>
#include <ui/context.hpp>
#include <utils.hpp>

#include <nfd.hpp>

#include <nexis_backend_raylib.c>

App::App()
    : state(AppState::ProjectExplorer), window(), ui(window.dpi_scale)
{
    // Native file dialog
    NFD::Init();

    utils::load_projects(&projects);

    this->grid_shader.handle         = LoadShader(utils::path_abs("shaders/grid.vert").c_str(), utils::path_abs("shaders/grid.frag").c_str());
    this->grid_shader.camera_pos_loc = GetShaderLocation(grid_shader.handle, "camera_pos");

    this->renderer = {
        .particles_draw = Nx_backend_raylib_render,
    };

    this->camera = SceneCamera{};
}

App::~App()
{
    this->project.destroy();
    NFD::Quit();
}

bool App::should_close() const
{
    return WindowShouldClose() || this->state == AppState::Exit;
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
    this->window.update();
    f32 dt = GetFrameTime();

    this->camera.update(this->ui.editor.scene_texture_active, dt);
    SetShaderValue(this->grid_shader.handle, this->grid_shader.camera_pos_loc, &this->camera.raylib.position, SHADER_UNIFORM_VEC3);

    if (this->state == AppState::Editor)
    {
        Nx_system_update_emitters(&this->project.system, dt);
    }
}

void App::draw()
{
    AppContext ctx = make_context();
    if (this->state == AppState::Editor)
    {
        BeginTextureMode(this->ui.editor.scene);
        ClearBackground(BLACK);

        BeginMode3D(this->camera.raylib);

        BeginShaderMode(this->grid_shader.handle);
        DrawPlane({0.0f, 0.0f, 0.0f}, {20.0f, 20.0f}, RED);

        EndShaderMode();

        Nx_system_render_emitters(&this->project.system, &this->renderer);

        EndMode3D();
        EndTextureMode();
    }

    BeginDrawing();
    ClearBackground(BLACK);
    this->state = this->ui.draw(this->state, ctx);
    EndDrawing();
}
