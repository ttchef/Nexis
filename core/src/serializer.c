
#include <Nexis/serializer.h>
#include <darray.h>
#include <alloc.h>

typedef struct
{
    NxU8   *data;
    NxUsize size;
    NxUsize at;
} NxMemoryStream;

// NOTE: Write functions

static inline void write(NxMemoryStream *stream, void *data, NxUsize size)
{
    assert(stream->at + size <= stream->size);
    memcpy(&stream->data[stream->at], data, size);
    stream->at += size;
}

static inline void write_NxF32(NxMemoryStream *stream, NxF32 v)
{
    write(stream, &v, sizeof(v));
}

static inline void write_NxU32(NxMemoryStream *stream, NxU32 v)
{
    write(stream, &v, sizeof(v));
}

static inline void write_NxU64(NxMemoryStream *stream, NxU64 v)
{
    write(stream, &v, sizeof(v));
}

void write_NxVec3(NxMemoryStream *stream, NxVec3 v)
{
    write_NxF32(stream, v.x);
    write_NxF32(stream, v.y);
    write_NxF32(stream, v.z);
}

void write_name(NxMemoryStream *stream, NxChar name[Nx_EMITTER_NAME_LEN])
{
    write(stream, name, Nx_EMITTER_NAME_LEN);
}

void write_enabled(NxMemoryStream *stream, NxBool enabled)
{
    write(stream, &enabled, sizeof(enabled));
}

void write_modules(NxMemoryStream *stream, NxModules *modules)
{
    for (NxU32 i = 0; i < NxModuleQueue_Count; i++)
    {
        NxModuleQueue *q = &modules->queues[i];
        
        write_NxU64(stream, q->used);
        write(stream, q->data, q->used);
    }
}
// -------

// NOTE: Read functions

static inline void *read(NxMemoryStream *stream, NxUsize size)
{
    assert(stream->at - size <= stream->at);
    void *data = &stream->data[stream->size - stream->at];
    stream->at -= size;
    return data;
}

static inline NxU8 read_NxU8(NxMemoryStream *stream)
{
    return *(NxU8 *)read(stream, sizeof(NxU8));
}

static inline NxU32 read_NxU32(NxMemoryStream *stream)
{
    return *(NxU32 *)read(stream, sizeof(NxU32));
}

static inline NxU64 read_NxU64(NxMemoryStream *stream)
{
    return *(NxU64 *)read(stream, sizeof(NxU64));
}

void read_name(NxMemoryStream *stream, NxChar name[Nx_EMITTER_NAME_LEN])
{
    memcpy(name, read(stream, Nx_EMITTER_NAME_LEN), Nx_EMITTER_NAME_LEN);
}

void read_enabled(NxMemoryStream *stream, NxBool *enabled)
{
    *enabled = read_NxU8(stream);
}

void read_modules(NxMemoryStream *stream, NxModules *modules)
{
    for (NxU32 i = 0; i < NxModuleQueue_Count; i++)
    {
        NxModuleQueue *q = &modules->queues[i];

        q->used = read_NxU64(stream);
        void *data = read(stream, q->used);
        memcpy(q->data, data, q->used);
    }
}

// -------

NxBool Nx_system_store(const NxSystem *system, NxBuffer *out)
{
    if (!system || !system->emitters || !out)
    {
        return false;
    }

    const NxU64 size = Nx_GB(10ull);

    NxMemoryStream stream = {
          .at = 0,
          .data = Nx_virtual_alloc(size),
          .size = size,
    };

    NxU32 emitter_count = Nx_darray_len(system->emitters);
    write_NxU32(&stream, emitter_count);

    for (NxU32 i = 0; i < emitter_count; i++)
    {
        NxEmitterConfig *e = &system->emitters[i].config;

        write_name(&stream, e->name);
        write_enabled(&stream, e->enabled);
        write_modules(&stream, &e->modules);
    }

    *out = (NxBuffer){
        .data = stream.data,
        .size = stream.at, 
    };

    return true;
}

void Nx_system_load(NxSystem *system, NxBuffer *buffer)
{
    NxMemoryStream stream = {
        .at = buffer->size,
        .size = buffer->size,
        .data = buffer->data,    
    };
    
    NxU32 emitter_count = read_NxU32(&stream);

    for (NxU32 i = 0; i < emitter_count; i++)
    {
        NxEmitter e = {0};
        Nx_emitter_create(&e);

        read_name(&stream, e.config.name);
        read_enabled(&stream, &e.config.enabled);
        read_modules(&stream, &e.config.modules);

        Nx_system_add_emitter(system, &e);
    }
}
