#pragma once

#include <types.hpp>
#include <app_state.hpp>
#include <app_context.hpp>

#include <Nexis/core.h>

#include <raylib.h>

namespace ui
{

constexpr i32 MODULE_INDEX_NONE = -1;
    
struct SelectedModule
{
    u32 emitter_index;
    NxModuleQueueIndex queue_index;
    i32 module_index;

    // NOTE: Special case settings menu
    bool settings;
};
    
struct Editor
{
    RenderTexture2D scene;
    // Clicked on scene texture while hovering
    bool scene_texture_active;

    NxEmitter add_emitter;
    SelectedModule module;

    Editor();
    AppState draw(AppContext &ctx);
};
} // namespace ui
