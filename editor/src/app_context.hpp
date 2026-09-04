
#pragma once

#include <types.hpp>

#include <vector>

struct ImFont;
struct Project;
struct ProjectHeader;
struct SceneCamera;

namespace asset
{
struct Manager;
}

struct AppContext
{
    f32 dpi_scale;
    ImFont *normal_font;
    ImFont *mdedium_font;
    ImFont *header_font;

    Project *project;
    std::vector<ProjectHeader> *projects;
    SceneCamera *camera;
    asset::Manager *asset_manager;
};

