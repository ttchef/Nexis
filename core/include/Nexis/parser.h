
#pragma once

#include <Nexis/system.h>

//
// NOTE: This file is for serializing the NxSystem struct.
// So it defines a load and store function to read it from memory.
//

#define Nx_PARSE_RESULTS(Nx_ITEM)              \
    Nx_ITEM(NxParse_Success)                   \
    Nx_ITEM(NxParse_InvalidParameters)     \
    Nx_ITEM(NxParse_BufferOverflow)    \
    Nx_ITEM(NxParse_UnexpectedEOF) \
    Nx_ITEM(NxParse_CorruptData)\
    Nx_ITEM(NxParse_InvalidSignature)\
    Nx_ITEM(NxParse_InvalidVersion)

typedef enum
{
#define Nx_ITEM(name) name,
    Nx_PARSE_RESULTS(Nx_ITEM)
#undef Nx_ITEM
} NxParseResult;

typedef struct
{
    void   *data;
    NxUsize size;
} NxBuffer;

// NOTE: Returns a buffer allocated on the heap with the serialized data.
// This then can be written to a file directly.
NxParseResult Nx_system_store(const NxSystem *system, NxBuffer *out);

// NOTE: Also only loading from memory.
NxParseResult Nx_system_load(NxSystem *system, const void *data, NxUsize size);

void Nx_buffer_free(NxBuffer *buffer);

static inline const char *Nx_parse_result_string(NxParseResult result)
{
    switch (result)
    {
#define Nx_ITEM(name) \
    case name:        \
        return #name;
        Nx_PARSE_RESULTS(Nx_ITEM)
#undef Nx_ITEM
            default : return "Unkown Result";
    }
}
