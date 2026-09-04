#pragma once

#include <types.hpp>

#include <raylib.h>

struct Window
{
    u32 width;
    u32 height;
    f32 dpi_scale;

    Window()
    {
        width = 800;
        height = 600;
        
        SetTraceLogLevel(LOG_WARNING);
        SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
        InitWindow(width, height, "Nexis");
        SetExitKey(KEY_NULL);
        dpi_scale = compute_scale();
    }

    ~Window() { CloseWindow(); }

    void update()
    {
        width = GetScreenWidth();
        height = GetScreenHeight();
    }

  private:
    static f32 compute_scale()
    {
        i32 monitor  = GetCurrentMonitor();
        i32 width_px = GetMonitorWidth(monitor);
        i32 width_mm = GetMonitorPhysicalWidth(monitor);
        if (width_mm <= 0)
        {
            return 1.0f;
        }
        f32 width_inches = width_mm / 25.4f;
        f32 dots_per_inc = width_px / width_inches;
        return dots_per_inc / 96.0f;
    }
};
