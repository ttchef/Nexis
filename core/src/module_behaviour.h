
#pragma once

#include <Nexis/module.h>
#include <Nexis/types.h>
#include <Nexis/system.h>

#include <assert.h>

//
// NOTE: This file defines library internal module_apply functions
// which defines the behaviour of a module.
//

typedef struct
{
    NxModuleQueueIndex queue;
    void              *data;
} NxCallbackData;

typedef struct
{
    NxEmitter *emitter;
} NxEmitterOnSpawnData;

typedef struct
{
    NxEmitter *emitter;
    NxF32      delta_time;
} NxEmitterOnUpdateData;

typedef struct
{
    NxParticle *particle;    
} NxParticleOnSpawnData;

typedef struct
{
    NxParticles *particles;
#define X(type, name) type *name;
    Nx_PARTICLE_FIELDS(X)
#undef X
    NxF32 delta_time;
} NxParticleOnUpdateData;

#define Nx_QUEUE_DATA_TYPE_NxModuleQueue_EmitterSpawn NxEmitterOnSpawnData
#define Nx_QUEUE_DATA_TYPE_NxModuleQueue_EmitterUpdate  NxEmitterOnUpdateData
#define Nx_QUEUE_DATA_TYPE_NxModuleQueue_ParticleSpawn  NxParticleOnSpawnData
#define Nx_QUEUE_DATA_TYPE_NxModuleQueue_ParticleUpdate NxParticleOnUpdateData
#define Nx_QUEUE_DATA_TYPE(queue_index) Nx_QUEUE_DATA_TYPE_##queue_index

#define Nx_FIELD(...)
#define Nx_MODULE(name, display, queue_index, ...)\
    static inline Nx_QUEUE_DATA_TYPE(queue_index) * Nx_module_##name##_callback_data(NxCallbackData *callback_data)\
    {\
        assert(callback_data->queue == queue_index);\
        return (Nx_QUEUE_DATA_TYPE(queue_index) *)callback_data->data;\
    }
Nx_MODULES(Nx_MODULE, Nx_FIELD)
#undef Nx_MODULE
#undef Nx_FIELD

#define Nx_CALLBACK_DATA(name, callback_data) Nx_module_##name##_callback_data(callback_data)

#define Nx_FIELD(...)
#define Nx_MODULE(name, display, queue_index, ...) \
    void Nx_module_behaviour_##name(NxCallbackData *callback_data, NxModule##name *module);
Nx_MODULES(Nx_MODULE, Nx_FIELD)
#undef Nx_MODULE
#undef Nx_FIELD
