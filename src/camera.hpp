
#pragma once

#include <types.hpp>

#include <algorithm>
#include <cmath>

#include <raylib.h>

struct SceneCamera
{
    Camera3D raylib;

    // 3D spherical coordinates
    f32 azimuthal_angle;
    f32 polar_angle;
    f32 rho;

    SceneCamera()
    {
        raylib = {
            .target     = {0.0f, 0.0f, 0.0f},
            .up         = {0.0f, 1.0f, 0.0f},
            .fovy       = 45.0f,
            .projection = CAMERA_PERSPECTIVE,
        };

        azimuthal_angle = 0.0f;
        polar_angle = 45.0f;
        rho = 10.0f;
    }

    void update(bool scene_active, f32 dt)
    {
        if (scene_active)
        {
            auto delta = GetMouseDelta();
            constexpr f32 sensitivity = 0.1f;
            azimuthal_angle += delta.x * sensitivity;
            polar_angle += -delta.y * sensitivity;
        }

        f32 scroll = GetMouseWheelMove();
        constexpr f32 zoom_speed = 1.0f;
        rho += -scroll * zoom_speed;

        rho = std::max(rho, 0.1f);
        polar_angle = std::clamp(polar_angle, 0.01f, 89.99f);

        f32 az = azimuthal_angle * DEG2RAD;
        f32 pol = polar_angle * DEG2RAD;

        // Convert spherical coordinates to cartesian coordinates
        f32 x = rho * std::sin(pol) * std::cos(az);
        f32 y = rho * std::sin(pol) * std::sin(az);
        f32 z = rho * std::cos(pol);

        // Physics coordinate system
        raylib.position = {x, z, y};
    }
};


