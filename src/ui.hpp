#pragma once

#include <types.hpp>
#include <globals.hpp>
#include <particle.hpp>

#include <raylib.h>

namespace ui
{

struct Context
{
    RenderTexture2D scene;
    // Clicked on scene texture while hovering
    bool scene_texture_active;

    f32 dpi_scale;

    particle::Emitter add_emitter;

    Context(f32 dpi_scale);
    ~Context();

    void compute(std::vector<particle::Emitter> &emitters);
};

} // namespace ui
