
#pragma once

#include <Nexis/types.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// NOTE: Darray with shadowdata
//

#define Nx_DARRAY_START_CAPACITY 10
#define Nx_DARRAY_MAGIC          0x6767187F

typedef struct
{
    NxU32 capacity;
    NxU32 len;
    NxU32 element_size;
    NxU32 magic;
} NxDarrayHeader;

static inline NxDarrayHeader *Nx_darray_base(void *Nx_darray)
{
    return ((NxDarrayHeader *)Nx_darray) - 1;
}

static inline void *Nx_darray_create(NxU32 element_size)
{
    NxDarrayHeader *header = (NxDarrayHeader *)malloc(element_size * Nx_DARRAY_START_CAPACITY + sizeof(NxDarrayHeader));
    assert(header);

    header->capacity     = Nx_DARRAY_START_CAPACITY;
    header->len          = 0;
    header->element_size = element_size;
    header->magic        = Nx_DARRAY_MAGIC;

    return (void *)(header + 1);
}

static inline void Nx_darray_destroy(void *Nx_darray)
{
    if (!Nx_darray)
    {
        fprintf(stderr, "[NEXIS] Failed to destroy Nx_darray data ptr is NULL\n");
        return;
    }

    NxDarrayHeader *header = Nx_darray_base(Nx_darray);
    assert(header->magic == Nx_DARRAY_MAGIC);
    free(header);
}

static inline NxU32 Nx_darray_magic(void *Nx_darray)
{
    if (Nx_darray)
    {
        return Nx_darray_base(Nx_darray)->magic;
    }
    return 0;
}

static inline NxU32 Nx_darray_element_size(void *Nx_darray)
{
    if (Nx_darray)
    {
        return Nx_darray_base(Nx_darray)->element_size;
    }
    return 0;
}

static inline NxU32 Nx_darray_len(void *Nx_darray)
{
    if (Nx_darray)
    {
        return Nx_darray_base(Nx_darray)->len;
    }
    return 0;
}

static inline NxU32 Nx_darray_capacity(void *Nx_darray)
{
    if (Nx_darray)
    {
        return Nx_darray_base(Nx_darray)->capacity;
    }
    return 0;
}

static inline void Nx_darray_len_set(void *Nx_darray, NxU32 len)
{
    if (Nx_darray)
    {
        Nx_darray_base(Nx_darray)->len = len;
    }
}

static inline void *Nx_darray_at(void *Nx_darray, NxU32 index)
{
    if (index >= Nx_darray_len(Nx_darray))
    {
        fprintf(stderr, "[NEXIS] Tried to index darray out of bounds: %u\n", index);
        assert(0);
    }

    return ((NxU8 *)Nx_darray) + Nx_darray_element_size(Nx_darray) * index;
}

// NOTE: Returns pointer to pushed element in the array and NULL on error
static inline void *Nx_darray_push(void **Nx_darray_ptr, void *element)
{
    void           *Nx_darray = *Nx_darray_ptr;
    NxDarrayHeader *header    = Nx_darray_base(Nx_darray);

    assert(header->magic == Nx_DARRAY_MAGIC);

    if (header->len + 1 > header->capacity)
    {
        NxU32           new_capacity = header->capacity * 2;
        NxDarrayHeader *new_header   = (NxDarrayHeader *)realloc(header, header->element_size * new_capacity + sizeof(NxDarrayHeader));
        if (!new_header)
        {
            fprintf(stderr, "[NEXIS] Out of memory.");
            return NULL;
        }

        new_header->capacity = new_capacity;
        header               = new_header;

        *Nx_darray_ptr = (void *)(new_header + 1);
        Nx_darray      = *Nx_darray_ptr;
    }

    NxU8 *data = (NxU8 *)Nx_darray;
    void *pos  = data + header->element_size * header->len;

    memcpy(pos, element, header->element_size);

    header->len++;
    return pos;
}
