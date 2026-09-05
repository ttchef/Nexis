
#pragma once

#include <Nexis/math.h>
#include <Nexis/types.h>

typedef enum
{
    NxModuleQueue_None = -1,
    NxModuleQueue_EmitterSpawn,
    NxModuleQueue_EmitterUpdate,
    NxModuleQueue_ParticleSpawn,
    NxModuleQueue_ParticleUpdate,
    NxModuleQueue_Count,
} NxModuleQueueIndex;

// NOTE: Working of MODULES macro
// 
// Parameter 1: Name of module. This will generate the appropriate
// NxModule##name struct and NxModuleType_##name enum.
//
// Parameter 2: Queue index. This indicated the emitter queue where the module
// should live. For example for an AddVelocity module the only emitter queue
// where it belongs to is in ParticleSpawn because it acts as a initial velocity on spawn.
//
// Parameter 3: Now follows a va args list of fields constructing the module struct.
// 

// NOTE: Working of the FIELD macro
//
// Parameter 1: Type of the field. This is the type of the field variable
// used in the module struct.
//
// Parameter 2: Name of the field. This is the name of the field variable
// used in the model struct.
//
// Parameter 3: A va args list of 'default values'. This needs to be a va args list
// for special types like vectors where the definition requires ',' itself example: {0.0f, 0.0f, 0.0f}.
// This would be treated as multiple arguments to the macro so we need to make this one a va args list
// to get it working.

#define MODULES(MODULE, FIELD)                       \
    MODULE(                                          \
        SpawnRate,                                   \
        NxModuleQueue_EmitterUpdate,                 \
        FIELD(NxF32, emit_speed, 2.0f)               \
            FIELD(NxF32, elapsed_time, 0.0f))        \
    MODULE(                                          \
        SpawnBurst,                                  \
        NxModuleQueue_EmitterUpdate,                 \
        FIELD(NxU32, particle_count, 1)              \
            FIELD(NxU32, trigger_count, 1))          \
    MODULE(                                          \
        AddVelocity,                                 \
        NxModuleQueue_ParticleSpawn,                 \
        FIELD(NxVec3, direction, {0.0f, 0.0f, 0.0f}) \
            FIELD(NxF32, speed,                      \
                  1.0f))                             \
    MODULE(                                          \
        GravityForce,                                \
        NxModuleQueue_ParticleUpdate,                \
        FIELD(NxVec3, direction, {0.0f, 0.0f, 0.0f}) \
            FIELD(NxF32, speed, 1.0f))

static const NxU32 Nx_QUEUE_LOOKUP[] = {
#define FIELD(...)
#define MODULE(name, queue_index, ...)
    MODULES(MODULE, FIELD)
#undef MODULE
#undef FIELD
};

typedef enum
{
#define FIELD(...)
#define MODULE(name, queue_index, ...) NxModuleType_##name,
    MODULES(MODULE, FIELD)
#undef MODULE
#undef FIELD
} NxModuleType;

typedef struct
{
    NxModuleType type;
    NxU32        size;
} NxModuleHeader;

#define FIELD(type, name, ...) type name;
#define MODULE(name, queue_index, ...) \
    typedef struct                     \
    {                                  \
        __VA_ARGS__                    \
    } NxModule##name;
MODULES(MODULE, FIELD)
#undef MODULE
#undef FIELD

typedef struct
{
    NxU8 *data;
    NxU64 used;
} NxModuleQueue;

typedef struct
{
    NxModuleQueue queues[NxModuleQueue_Count];
} NxModules;

typedef void (*Nx_for_each_module_func)(NxModuleType type, void *module_data, void *userdata);

void Nx_modules_create(NxModules *out);

void Nx_modules_destroy(NxModules *modules);

void Nx_modules_for_each(NxModules *modules, NxModuleQueueIndex queue, Nx_for_each_module_func func, void *userdata);

// NOTE: Add module functions
#define FIELD(...)
#define MODULE(name, queue_index, ...) void Nx_modules_add_##name(NxModules *modules, NxModuleQueueIndex queue, NxModule##name module);
MODULES(MODULE, FIELD)
#undef FIELD
#undef MODULE

// NOTE: Make default functions
#define FIELD(type, name, ...) __VA_ARGS__,
#define MODULE(name, queue_index, ...) \
static inline NxModule##name Nx_module_##name##_make_default(void)\
{\
    return (NxModule##name){\
        __VA_ARGS__\
    };\
}
MODULES(MODULE, FIELD)
#undef MODULE
#undef FIELD
