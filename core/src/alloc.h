
#pragma once

#include <Nexis/types.h>

//
// NOTE: Provides a cross platform API for virtual memory allocations across
// the most popular operating systems (linux, windows, macos)
//

void *Nx_virtual_alloc(NxUsize size);

void Nx_virtual_free(void *mem, NxUsize size);
