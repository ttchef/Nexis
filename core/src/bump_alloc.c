
#include <bump_alloc.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

NxBumpAlloc Nx_bump_alloc_create(NxU32 start_capacity)
{
    NxBumpAlloc result;

    result.capacity = start_capacity;
    result.data     = malloc(start_capacity);
    result.count    = 0;
    assert(result.data);

    return result;
}

void Nx_bump_alloc_destroy(NxBumpAlloc *alloc)
{
    if (alloc)
    {
        if (alloc->data)
        {
            free(alloc->data);
        }
        alloc->data     = NULL;
        alloc->capacity = 0;
        alloc->count    = 0;
    }
}

void *Nx_bump_alloc_push_size(NxBumpAlloc *alloc, NxU32 size)
{
    if (!alloc || alloc->capacity == 0 || !alloc->data)
    {
        fprintf(stderr, "[NEXIS] Tried to push into an unitialized bump allocator\n");
        return NULL;
    }

    if (alloc->count + size > alloc->capacity)
    {
        NxU32 new_capacity = alloc->capacity * 2;
        void *new_data = realloc(alloc->data, new_capacity);
        assert(new_data);

        alloc->data = new_data;
        alloc->capacity = new_capacity;
    }

    void *result = &alloc->data[alloc->count];
    alloc->count += size;

    return result;
}

void Nx_bump_alloc_push(NxBumpAlloc *alloc, void *data, NxU32 size)
{
    void *copy_dest = Nx_bump_alloc_push_size(alloc, size);
    assert(copy_dest);
    memcpy(copy_dest, data, size);
}
