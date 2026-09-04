
#pragma once

#include <math.h>
#include <types.h>

typedef NxU64 NxTextureHandle;

struct NxParticleDrawParams
{
    NxVec3          position;
    NxVec3          scale;
    NxVec4          color;
    NxTextureHandle texture;
};

typedef enum
{
    NxBlendingOpaque,
    NxBlendingAdditive,
} NxBlending;

struct NxParticleBatch
{
    NxParticleDrawParams *particles;
    NxU32                 particle_count;
    NxBlending            blending;
};

struct NxRenderer
{
    void (*particles_draw)(NxParticleBatch batch);
};
