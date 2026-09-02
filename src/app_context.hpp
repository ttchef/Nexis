// app_context.hpp
#pragma once

#include <types.hpp>
#include <particle.hpp>
#include <camera.hpp>
#include <project.hpp>

#include <vector>

struct ImFont;

struct AppContext
{
    f32 dpi_scale;
    ImFont *normal_font;
    ImFont *mdedium_font;
    ImFont *header_font;

    Project *project;
    std::vector<Project> *projects;
    SceneCamera *camera;
};
