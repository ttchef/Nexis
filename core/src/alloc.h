
#pragma once

#include <Nexis/types.h>

//
// NOTE: Provides a cross platform API for virtual memory allocations across
// the most popular operating systems (linux, windows, macos)
//

#define Nx_KB(n) ((n) << 10)
#define Nx_MB(n) ((n) << 20)
#define Nx_GB(n) ((n) << 30)

void *Nx_virtual_alloc(NxU64 size);

void Nx_virtual_free(void *mem, NxU64 size);
