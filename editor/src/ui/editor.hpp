#pragma once

#include <types.hpp>
#include <app_state.hpp>
#include <app_context.hpp>

#include <Nexis/core.h>

#include <raylib.h>

namespace ui
{
struct Editor
{
    RenderTexture2D scene;
    // Clicked on scene texture while hovering
    bool scene_texture_active;

    NxEmitter add_emitter;

    Editor();
    AppState draw(AppContext &ctx);
};
} // namespace ui
