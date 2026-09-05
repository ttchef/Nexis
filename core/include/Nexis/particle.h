
#pragma once

#include <Nexis/math.h>
#include <Nexis/types.h>

// NOTE: Forward declaration
typedef struct NxRenderer NxRenderer;

typedef NxU64 NxTextureHandle;

// NOTE: This makes up the fields of a particle and all the dynamic arrays for the SoA
#define Nx_PARTICLE_FIELDS(X) \
    X(NxVec3, position)       \
    X(NxVec3, velocity)       \
    X(NxVec3, acceleration)   \
    X(NxVec3, scale)          \
    X(NxF32, lifetime)

// NOTE: Only used as parameter for 'Nx_emitter_add_particle'
typedef struct
{
#define X(type, name) type name;
    Nx_PARTICLE_FIELDS(X)
#undef X
} NxParticle;

typedef struct
{
    // NOTE: All are darrays
    // If you are searching for the fields look at the 'Nx_PARTICLE_FIELDS' macro
#define X(type, name) type *name;
    Nx_PARTICLE_FIELDS(X)
#undef X
} NxParticles;

typedef enum
{
    NxBlendingOpaque,
    NxBlendingAdditive,
} NxBlending;

// NOTE: This is all the data an emitter needs to work
// when the emitter gets exported this is the only data that gets saved
typedef struct
{
    NxParticles     particles;
    NxChar          name[32];
    NxBool          enabled;
    NxTextureHandle texture;
    NxBlending      blending;
} NxEmitterConfig;

// NOTE: Runtime data which is just default constructed and changed
// by the simulation itself not configured by the user
typedef struct
{
    NxF32 elapsed_time;
} NxEmitterRuntime;

typedef struct
{
    NxEmitterConfig  config;
    NxEmitterRuntime runtime;
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
