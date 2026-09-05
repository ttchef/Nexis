
#pragma once

#include <Nexis/types.h>

#define MODULES(MODULE, FIELD) \
    MODULE(                    \
        SpawnRate,              \
        FIELD(NxU32, test0)    \
        FIELD(NxU32, test1) \
    )

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
        __VA_ARGS__     \
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
} NxModuleQueueType;

typedef struct
{
    NxModuleQueue queues[NxModuleQueue_Count];
} NxModules;

void Nx_modules_create(NxModules *out);

void Nx_modules_destroy(NxModules *modules);

// NOTE: Add module functions
#define FIELD(...)
#define MODULE(name, ...) void Nx_modules_add_##name(NxModules *modules, NxModuleQueueType queue, NxModule##name module);
MODULES(MODULE, FIELD)
#undef FIELD
#undef MODULE
