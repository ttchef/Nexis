
#pragma once

#include <Nexis/particle.h>

//
// NOTE: This file is for serializing the NxSystem struct.
// So it defines a load and store function to read it from memory.
// 

typedef struct
{
	void *data;
	NxUsize size;
} NxBuffer;

// NOTE: Returns a buffer allocated on the heap with the serialized data.
// This then can be written to a file directly.
NxBool Nx_system_store(const NxSystem *system, NxBuffer *out);

// NOTE: Also only loading from memory.
void Nx_system_load(NxSystem *system, NxBuffer *buffer);
