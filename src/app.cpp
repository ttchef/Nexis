
#include <app.hpp>
#include <globals.hpp>
#include <ui/context.hpp>
#include <utils.hpp>

#include <iostream>

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

void load_projects(std::vector<Project> &projects)
{
    std::string project_path = utils::path_abs("projects");
    if (!DirectoryExists(project_path.c_str()))
    {
        if (MakeDirectory(project_path.c_str()) != 0)
        {
            std::cout << "Failed to make projects directory" << std::endl;
            std::exit(1);
        }
        return;
    }

    FilePathList files = LoadDirectoryFilesEx(project_path.c_str(), NEXIS_PF_EX, false);

    projects.reserve(files.count);
    for (u32 i = 0; i < files.count; i++)
    {
        projects.push_back({
            .file_path = files.paths[i],
            .file_name = GetFileNameWithoutExt(files.paths[i]),
            .mod_time  = GetFileModTime(files.paths[i]),
        });
    }
}

App::App()
    : state(AppState::ProjectExplorer), window(), ui(window.dpi_scale)
{
    // Native file dialog
    NFD::Init();

    load_projects(this->projects);

    grid_shader.handle         = LoadShader(utils::path_abs("../src/shaders/grid.vert").c_str(), utils::path_abs("../src/shaders/grid.frag").c_str());
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

AppContext App::make_context()
{
    return AppContext{
        .dpi_scale    = ui.dpi_scale,
        .normal_font  = ui.normal_font,
        .mdedium_font = ui.medium_font,
        .header_font  = ui.header_font,
        .project      = &project,
        .projects     = &projects,
        .camera       = &camera,
    };
}

void App::update()
{
    AppContext ctx = make_context();
    global.update_window_size(GetScreenWidth(), GetScreenHeight());
    f32 dt = GetFrameTime();

    camera.update(ui.editor.scene_texture_active, dt);
    SetShaderValue(grid_shader.handle, grid_shader.camera_pos_loc, &camera.raylib.position, SHADER_UNIFORM_VEC3);

    if (state == AppState::Editor)
    {
        project.system.update(dt);
    }
}

static void scene_draw(App &app)
{
    BeginTextureMode(app.ui.editor.scene);
    ClearBackground(BLACK);

    BeginMode3D(app.camera.raylib);

    BeginShaderMode(app.grid_shader.handle);
    DrawPlane({0.0f, 0.0f, 0.0f}, {100.0f, 100.0f}, RED);

    EndShaderMode();

    app.project.system.draw(app.camera);

    EndMode3D();
    EndTextureMode();
}

void App::draw()
{
    AppContext ctx = make_context();
    if (state == AppState::Editor)
    {
        scene_draw(*this);
    }

    BeginDrawing();
    ClearBackground(BLACK);
    state = ui.draw(state, ctx);
    EndDrawing();
}
