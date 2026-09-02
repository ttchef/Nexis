#pragma once

#include <types.hpp>
#include <particle.hpp>
#include <app_state.hpp>

#include <raylib.h>

namespace ui
{
struct Editor
{
    RenderTexture2D scene;
    // Clicked on scene texture while hovering
    bool scene_texture_active;

    particle::Emitter add_emitter;

    Editor();
    AppState draw(particle::System &system, f32 dpi_scale);
};
} // namespace ui
