
#include <Nexis/core.h>
#include <raylib.h>

#include <stdio.h>

// NOTE: Using raylib backend from root backends/ folder
#include <nexis_backend_raylib.c>

// Helpers
static inline const char *path_abs(const char *path)
{
    static char buffer[2046];
    const char *base_path = GetApplicationDirectory();
    snprintf(buffer, sizeof(buffer), "%s%s", base_path, path);
    return buffer;
}

NxBuffer read_whole_file(const char *path)
{
    FILE *file = fopen(path, "rb");
    if (!file)
    {
        printf("Failed to open particle system asset file!\n");
        return (NxBuffer){};
    }

    fseek(file, 0, SEEK_END);
    NxUsize size = ftell(file);
    rewind(file);

    void *data = malloc(size);
    fread(data, 1, size, file);

    fclose(file);

    return (NxBuffer){
        .data = data,
        .size = size,
    };
}

int main()
{
    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(800, 600, "Titel");

    // Get the data of the particle system asset
    NxBuffer asset = read_whole_file(path_abs("assets/simple_system.nxp"));

    // Load the particle system from memory.
    // If you wanna have multiple particle effects you will have multiple
    // of these systems, one corresponding to one effect.
    NxSystem system;
    NxParseResult result = Nx_system_load(&system, asset.data, asset.size);

    if (result != NxParse_Success)
    {
        printf("Failed to load nexis system from memory: %s\n", Nx_parse_result_string(result));
        return 1;
    }

    // This is the renderer which descripeds how the particles will be rendered.
    // You only need to pass in a valid particles_draw function which you can ehter make yourself
    // or use one of the available backends like here where there is already a raylib example one.
    NxRenderer renderer = {
          .particles_draw = Nx_backend_raylib_render,
    };

    // Raylib camera because of 3d stuff
    Camera3D camera = {
        .position = { 10.0f, 0.0f, 0.0f},
    };

    while (!WindowShouldClose())
    {
        NxF32 dt = GetFrameTime();

        Nx_system_update_emitters(&system, dt);
        
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);

        Nx_system_render_emitters(&system, &renderer);

        EndMode3D();
        
        EndDrawing();
    }

    Nx_system_destroy(&system);

    CloseWindow();

    return 0;
}
