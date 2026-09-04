
#pragma once

#include <types.h>

#include <float.h>

static inline NxF32 Nx_lerp(NxF32 a, NxF32 b, NxF32 t)
{
    return t * b + (1.0f - t) * a;
}

struct NxVec2
{
    NxF32 x;
    NxF32 y;
};

struct NxVec3
{
    NxF32 x;
    NxF32 y;
    NxF32 z;
};

struct NxVec4
{
    NxF32 x;
    NxF32 y;
    NxF32 z;
    NxF32 w;
};
