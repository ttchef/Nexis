
#pragma once

#include <Nexis/math.h>
#include <Nexis/types.h>

// NOTE: Forward declaration
typedef struct NxRenderer NxRenderer;

typedef NxU64 NxTextureHandle;

// NOTE: Only used as parameter for 'Nx_emitter_add_particle'
typedef struct
{
    NxVec3 position;
    NxVec3 velocity;
    NxVec3 acceleration;
    NxVec3 scale;
} NxParticle;

typedef struct
{
    // NOTE: All are darrays
    NxVec3 *positions;
    NxVec3 *velocities;
    NxVec3 *accelerations;
    NxVec3 *scales;
} NxParticles;

typedef enum
{
    NxBlendingOpaque,
    NxBlendingAdditive,
} NxBlending;

typedef struct
{
    NxParticles     particles;
    NxChar          name[32];
    NxBool          enabled;
    NxTextureHandle texture;
    NxBlending      blending;
} NxEmitter;

void Nx_emitter_create(NxEmitter *out);

void Nx_emitter_destroy(NxEmitter *emitter);

void Nx_emitter_add_particle(NxEmitter *emitter, NxParticle particle);

void Nx_emitter_update_particles(NxEmitter *emitter, NxF32 delta_time);

void Nx_emitter_render_particles(NxEmitter *emitter, NxRenderer *renderer);

typedef struct
{
    // NOTE: darray
    NxEmitter *emitters;
} NxSystem;

void Nx_system_create(NxSystem *out);

void Nx_system_destroy(NxSystem *system);

// NOTE: Kinda performs a move on emitter it will get zeroed out
void Nx_system_add_emitter(NxSystem *system, NxEmitter *emitter);

void Nx_system_update_emitters(NxSystem *system, NxF32 delta_time);

void Nx_system_render_emitters(NxSystem *system, NxRenderer *renderer);

NxU32 Nx_system_emitter_count(NxSystem *system);

typedef struct
{
    NxParticles *particles;
    NxU32        particle_count;
    NxBlending   blending;
} NxParticleBatch;

typedef void (*Nx_particles_draw_func)(NxParticleBatch batch);

struct NxRenderer
{
    Nx_particles_draw_func particles_draw;
};
