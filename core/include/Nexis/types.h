
#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef int8_t NxI8;
typedef int16_t NxI16;
typedef int32_t NxI32;
typedef int64_t NxI64;

typedef uint8_t NxU8;
typedef uint16_t NxU16;
typedef uint32_t NxU32;
typedef uint64_t NxU64;

typedef float NxF32;
typedef double NxF64;

typedef bool NxBool;

#define Nx_ARRAY_COUNT(x) (sizeof(x) / sizeof((x)[0]))
#define Nx_MAX(a, b)              ((a) > (b) ? (a) : (b))
#define Nx_MIN(a, b)              ((a) < (b) ? (a) : (b))

