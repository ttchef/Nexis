
#pragma once

#include <types.hpp>

struct Vec3
{
    f32 x;
    f32 y;
    f32 z;

    static Vec3 zero() {
        return Vec3{0.0f, 0.0f, 0.0f};    
    }

    void operator+=(const Vec3 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
    }

    Vec3 operator*(const Vec3 &other) const {
        return Vec3(x * other.x, y * other.y, z * other.z);
    }

    Vec3 operator*(const f32 other) const {
        return Vec3(x * other, y * other, z * other);
    }
};
