
#pragma once

#include <Nexis/math.h>
#include <Nexis/types.h>

typedef enum
{
    // NOTE: Important because the enum maps directly to indecies
    // in the queue array of NxModules.
    NxModuleQueue_None = -1,
    NxModuleQueue_EmitterSpawn,
    NxModuleQueue_EmitterUpdate,
    NxModuleQueue_ParticleSpawn,
    NxModuleQueue_ParticleUpdate,
    NxModuleQueue_Count,
} NxModuleQueueIndex;

// NOTE: Working of MODULE macro
//
// Parameter 1: Name of module. This will generate the appropriate
// NxModule##name struct and NxModuleType_##name enum. It will also generate two
// functions 'Nx_modules_add_##name' and 'Nx_module_##name##_make_default'.
//
// Parameter 2: Display name of the module. For example 'Spawn Burst' instead of the
// struct name 'SpawnBurst'. This is not used in the core library but doesnt add any overhead
// and makes is possible for other programs such as the editor to have less boilerplate code.
//
// Parameter 3: Queue index. This indicated the emitter queue where the module
// should live. For example for an AddVelocity module the only emitter queue
// where it belongs to is in ParticleSpawn because it acts as a initial velocity on spawn.
//
// Parameter 4: Now follows a va args list of fields constructing the module struct.
//
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
//
// NOTE: To add a module just add a new MODULE() declaration here
// and define the corresponding behavour function in module_behaviour.c
// The funtion decleration will be automatically generated in module_behaviour.h.
#define Nx_MODULES(MODULE, FIELD)                           \
    MODULE(                                                 \
        SpawnRate,                                          \
        "Spawn Rate",                                       \
        NxModuleQueue_EmitterUpdate,                        \
        FIELD(NxF32, emit_speed, 2.0f)                      \
            FIELD(NxF32, elapsed_time, 0.0f))               \
    MODULE(                                                 \
        SpawnBurst,                                         \
        "Spawn Burst",                                      \
        NxModuleQueue_EmitterUpdate,                        \
        FIELD(NxU32, particle_count, 1)                     \
            FIELD(NxU32, trigger_count, 1))                 \
    MODULE(                                                 \
        AddVelocity,                                        \
        "Add Velocity",                                     \
        NxModuleQueue_ParticleSpawn,                        \
        FIELD(NxVec3, direction, {0.0f, 0.0f, 0.0f})        \
            FIELD(NxF32, speed,                             \
                  1.0f))                                    \
    MODULE(                                                 \
        GravityForce,                                       \
        "Gravity Force",                                    \
        NxModuleQueue_ParticleUpdate,                       \
        FIELD(NxVec3, direction, {0.0f, 0.0f, 0.0f})        \
            FIELD(NxF32, strength, 1.0f))                   \
    MODULE(                                                 \
        SolveVelocityAndForces,                             \
        "Solve Velocity And Forces",                        \
        NxModuleQueue_ParticleUpdate, )                     \
    MODULE(                                                 \
        InitParticle,                                       \
        "Init Particle",                                    \
        NxModuleQueue_ParticleSpawn,                        \
        FIELD(NxF32, lifetime, 1.0f)                        \
            FIELD(NxVec4, color, {1.0f, 1.0f, 1.0f, 1.0f})) \
    MODULE(                                                 \
        SolveLifetime,                                      \
        "Solve Lifetime",                                   \
        NxModuleQueue_ParticleUpdate, )

static const char *Nx_MODULE_NAME_LOOKUP[] = {
#define Nx_FIELD(...)
#define Nx_MODULE(name, display, queue_index, ...) display,
    Nx_MODULES(Nx_MODULE, Nx_FIELD)
#undef Nx_MODULE
#undef Nx_FIELD
};

typedef enum
{
#define Nx_FIELD(...)
#define Nx_MODULE(name, display, queue_index, ...) NxModuleType_##name,
    Nx_MODULES(Nx_MODULE, Nx_FIELD)
#undef Nx_MODULE
#undef Nx_FIELD
} NxModuleType;

typedef struct
{
    NxModuleType type;
    NxUsize      size;
} NxModuleHeader;

#define Nx_FIELD(type, name, ...) type name;
#define Nx_MODULE(name, display, queue_index, ...) \
    typedef struct                                 \
    {                                              \
        __VA_ARGS__                                \
    } NxModule##name;
Nx_MODULES(Nx_MODULE, Nx_FIELD)
#undef Nx_MODULE
#undef Nx_FIELD

    typedef struct
{
    NxU8   *data;
    NxUsize used;
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
#define Nx_FIELD(...)
#define Nx_MODULE(name, display, queue_index, ...) void Nx_modules_add_##name(NxModules *modules, NxModule##name module);
Nx_MODULES(Nx_MODULE, Nx_FIELD)
#undef Nx_FIELD
#undef Nx_MODULE

// NOTE: Make default functions
#define Nx_FIELD(type, name, ...) __VA_ARGS__,
#define Nx_MODULE(name, display, queue_index, ...)                     \
    static inline NxModule##name Nx_module_##name##_make_default(void) \
    {                                                                  \
        return (NxModule##name){                                       \
            __VA_ARGS__};                                              \
    }
    Nx_MODULES(Nx_MODULE, Nx_FIELD)
#undef Nx_MODULE
#undef Nx_FIELD
