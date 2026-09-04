
#pragma once

#include <types.h>

typedef NxU64 NxTextureHandle;

typedef enum
{
    NxBlendingOpaque,
    NxBlendingAdditive,
} NxBlending;

typedef struct
{
    NxBlending            blending;
} NxParticleBatch;

typedef struct
{
    void (*particles_draw)(NxParticleBatch batch);
} NxRenderer;
