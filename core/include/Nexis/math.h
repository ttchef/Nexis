
#pragma once

#include <Nexis/types.h>

#include <float.h>
#include <math.h>

static inline NxF32 Nx_lerp(NxF32 a, NxF32 b, NxF32 t)
{
    return t * b + (1.0f - t) * a;
}

typedef struct
{
    NxF32 x;
    NxF32 y;
} NxVec2;

typedef struct
{
    NxF32 x;
    NxF32 y;
    NxF32 z;
} NxVec3;

static inline NxVec3 Nx_vec3(NxF32 x, NxF32 y, NxF32 z)
{
    return (NxVec3){x, y, z};
}

static inline NxVec3 Nx_vec3_add(NxVec3 a, NxVec3 b)
{
    return Nx_vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline NxVec3 Nx_vec3_scale(NxVec3 v, NxF32 scalar)
{
    return Nx_vec3(v.x * scalar, v.y * scalar, v.z * scalar);
}

static inline NxF32 Nx_vec3_lensq(NxVec3 v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

static inline NxF32 Nx_vec3_len(NxVec3 v)
{
    return sqrtf(Nx_vec3_lensq(v));
}

static inline NxVec3 Nx_vec3_norm(NxVec3 v)
{
    NxF32 len = Nx_vec3_len(v);
    if (len < FLT_EPSILON)
    {
        return Nx_vec3(0.0f, 0.0f, 0.0f);
    }
    return Nx_vec3_scale(v, 1.0f / len);
}

typedef struct
{
    NxF32 x;
    NxF32 y;
    NxF32 z;
    NxF32 w;
} NxVec4;
