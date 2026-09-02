// app_context.hpp
#pragma once

#include <types.hpp>
#include <particle.hpp>
#include <camera.hpp>
#include <vector>
#include <string>

struct ImFont;

struct AppContext
{
    f32 dpi_scale;
    ImFont *normal_font;
    ImFont *header_font;

    particle::System *system;
    std::vector<std::string> *project_files;
    SceneCamera *camera;
};
