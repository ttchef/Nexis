
#include <Nexis/module.h>
#include <alloc.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

static const NxU64 QUEUE_SIZE = Nx_GB(1ull);

void Nx_module_queue_create(NxModuleQueue *out)
{
    if (!out)
    {
        return;
    }

    out->used = 0;
    out->data = Nx_virtual_alloc(QUEUE_SIZE);
    assert(out->data);
}

void Nx_module_queue_destroy(NxModuleQueue *queue)
{
    if (!queue)
    {
        return;
    }

    Nx_virtual_free(queue->data, QUEUE_SIZE);
    queue->used = 0;
    queue->data = NULL;
}

void Nx_module_queue_push(NxModuleQueue *queue, void *data, NxU64 size)
{
    if (!queue || !data)
    {
        return;
    }

    if (!queue->data)
    {
        fprintf(stderr, "[NEXIS] Tried to push into unitialized model queue\n");
        return;
    }

    if (queue->used + size >= QUEUE_SIZE)
    {
        fprintf(stderr, "[NEXIS] Module queue ran out of memory\n");
        return;
    }

    memcpy(queue->data + queue->used, data, size);
    queue->used += size;
}

void Nx_modules_create(NxModules *out)
{
    if (!out)
    {
        return;
    }

    for (NxU32 i = 0; i < NxModuleQueue_Count; i++)
    {
        Nx_module_queue_create(&out->queues[i]);
    }
}

void Nx_modules_destroy(NxModules *modules)
{
    if (!modules)
    {
        return;
    }

    for (NxU32 i = 0; i < NxModuleQueue_Count; i++)
    {
        Nx_module_queue_destroy(&modules->queues[i]);
    }
}

void Nx_modules_for_each(NxModules *modules, NxModuleQueueIndex queue, Nx_for_each_module_func func, void *userdata)
{
    if (!modules || !func)
    {
        return;
    }
    
    NxModuleQueue *q = &modules->queues[queue];
    NxU8 *at = q->data;

    while (at < q->data + q->used)
    {
        NxModuleHeader *header = (NxModuleHeader *)at;
        if (header->size == 0)
        {
            fprintf(stderr, "[NEXIS] Encouterd module header size == 0 will break out of loop\n");
            break;
        }

        void *data = at + sizeof(NxModuleHeader);

        func(header->type, data, userdata);

        at += header->size;
    }
}

// NOTE: Add module functions
#define FIELD(...)
#define MODULE(name, ...)                                                                        \
    void Nx_modules_add_##name(NxModules *modules, NxModuleQueueIndex queue, NxModule##name module) \
    {                                                                                            \
        if (!modules)                                                                            \
        {                                                                                        \
            return;                                                                              \
        }                                                                                        \
                                                                                                 \
        if (queue < 0 || queue >= NxModuleQueue_Count)                                            \
        {                                                                                        \
            return;                                                                              \
        }                                                                                        \
                                                                                                 \
        NxModuleQueue *q = &modules->queues[queue];                                               \
                                                                                                 \
        NxModuleHeader header = {                                                                \
            .type = NxModuleType_##name,                                                         \
            .size = sizeof(NxModuleHeader) + sizeof(NxModule##name),                                                      \
        };                                                                                       \
                                                                                                 \
        Nx_module_queue_push(q, &header, sizeof(header));                                        \
        Nx_module_queue_push(q, &module, sizeof(module));                                        \
    }
MODULES(MODULE, FIELD)
#undef FIELD
#undef MODULE
