#pragma once

#include <types.hpp>
#include <particle.hpp>
#include <app_state.hpp>
#include <app_context.hpp>

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
    AppState draw(AppContext &ctx);
};
} // namespace ui
