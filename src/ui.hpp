#pragma once

#include <types.hpp>
#include <globals.hpp>

#include <raylib.h>

namespace ui
{

struct Context
{
    RenderTexture2D scene;
    
    Context();
    ~Context();

    void draw();
};

} // namespace ui
