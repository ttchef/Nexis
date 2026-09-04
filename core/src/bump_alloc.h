
#pragma once

#include <types.h>

//
// NOTE: Simple bump allocator
// 

typedef struct
{
	void *data;
	// NOTE: In bytes
	NxU32 capacity;
	NxU32 count;
} NxBumpAlloc;

// NOTE: start_capacity in bytes
NxBumpAlloc Nx_bump_alloc_create(NxU32 start_capacity);

void Nx_bump_alloc_destroy(NxBumpAlloc *alloc);

void *Nx_bump_alloc_push_size(NxBumpAlloc *alloc, NxU32 size);

void Nx_bump_alloc_push(NxBumpAlloc *alloc, void *data, NxU32 size);
