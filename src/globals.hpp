#pragma once

#include <types.hpp>

struct Global
{
    u32 window_width = 800;
    u32 window_height = 600;

    // returns true if the size changed
    bool update_window_size(u32 width, u32 height)
    {
        bool changed = false;
        if (window_width != width)
        {
            window_width = width;
            changed      = true;
        }
        if (window_height != height)
        {
            window_height = height;
            changed       = true;
        }
        return changed;
    }
};

extern Global global;
