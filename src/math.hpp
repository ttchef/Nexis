
#pragma once

#include <types.hpp>

// I have no idea where max is and i dont care right now all i know
// is that this provides max
#include <algorithm>

// Will probaly be removed later anyways
#include <raylib.h>

static inline f32 lerp(f32 a, f32 b, f32 t)
{
    return t * b + (1.0f - t) * a;
}

struct Vec2
{
    f32 x;
    f32 y;

    Vec2 operator*(const f32 other) const {
        return Vec2(x * other, y * other);
    }


    Vec2 operator-(const Vec2 other) const {
        return Vec2(x - other.x, y - other.y);
    }

    Vec2 operator+(const Vec2 other) const {
        return Vec2(x + other.x, y + other.y);
    }

    void max(f32 max) {
        x = std::max(x, max);
        y = std::max(y, max);
    }
};

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

    Vec3 operator-(const Vec3 other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 operator+(const Vec3 other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }
};

struct Vec4
{
    f32 x;
    f32 y;
    f32 z;
    f32 w;

    Vec4 lerp(const Vec4 &other, f32 t) const {
        return Vec4(::lerp(x, other.x, t), ::lerp(y, other.y, t), ::lerp(z, other.z, t), ::lerp(w, other.w, t));
    }

    Color raylib_color() const {
        return {static_cast<u8>(x * 255), static_cast<u8>(y  * 255), static_cast<u8>(z * 255), static_cast<u8>(w * 255)};
    }
};
