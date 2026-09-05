
#pragma once

#include <Nexis/types.h>

#define MODULES(MODULE, FIELD)          \
    MODULE(                             \
        SpawnRate,                      \
        FIELD(NxF32, emit_speed)        \
            FIELD(NxF32, elapsed_time)) \
    MODULE(                             \
        SpawnBurst,                     \
        FIELD(NxU32, particle_count))

typedef enum
{
#define MODULE(name, ...) NxModuleType_##name,
#define FIELD(...)
    MODULES(MODULE, FIELD)
#undef FIELD
#undef MODULE
} NxModuleType;

typedef struct
{
    NxModuleType type;
    NxU32        size;
} NxModuleHeader;

#define FIELD(type, name) type name;
#define MODULE(name, ...) \
    typedef struct        \
    {                     \
        __VA_ARGS__       \
    } NxModule##name;
MODULES(MODULE, FIELD)
#undef FIELD
#undef MODULE

typedef struct
{
    NxU8 *data;
    NxU64 used;
} NxModuleQueue;

typedef enum
{
    NxModuleQueue_None = -1,
    NxModuleQueue_EmitterUpdate,
    NxModuleQueue_Count,
} NxModuleQueueIndex;

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
#define MODULE(name, ...) void Nx_modules_add_##name(NxModules *modules, NxModuleQueueIndex queue, NxModule##name module);
MODULES(MODULE, FIELD)
#undef FIELD
#undef MODULE
