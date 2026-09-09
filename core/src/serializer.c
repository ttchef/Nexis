
#include <Nexis/serializer.h>
#include <darray.h>
#include <alloc.h>

typedef struct
{
    NxU8   *data;
    NxUsize size;
    NxUsize at;
} NxWriter;

static inline void write(NxWriter *writer, void *data, NxUsize size)
{
    // NOTE: I actually dont know if it should be <= idk future me solve this
    assert(writer->at + size < writer->size);
    memcpy(&writer->data[writer->at], data, size);
    writer->at += size;
}

static inline void write_NxF32(NxWriter *writer, NxF32 v)
{
    write(writer, &v, sizeof(v));
}

static inline void write_NxU32(NxWriter *writer, NxU32 v)
{
    write(writer, &v, sizeof(v));
}

void write_NxVec3(NxWriter *writer, NxVec3 v)
{
    write_NxF32(writer, v.x);
    write_NxF32(writer, v.y);
    write_NxF32(writer, v.z);
}

void write_particles(NxWriter *writer, NxParticles *particles)
{
    NxU32 particle_count = Nx_darray_len(particles->position);
    write_NxU32(writer, particle_count);
    
    for (NxU32 i = 0; i < particle_count; i++)
    {
#define X(type, name) \
    write_##type(writer, particles->name[i]);
        Nx_PARTICLE_FIELDS(X)
#undef X
    }
}

void write_name(NxWriter *writer, NxChar name[Nx_EMITTER_NAME_LEN])
{
    write(writer, name, Nx_EMITTER_NAME_LEN);
}

void write_enabled(NxWriter *writer, NxBool enabled)
{
    write(writer, &enabled, sizeof(enabled));
}

void write_modules(NxWriter *writer, NxModules *modules)
{
    for (NxU32 i = 0; i < NxModuleQueue_Count; i++)
    {
        NxModuleQueue *q = &modules->queues[i];
        
        write_NxU32(writer, q->used);
        write(writer, q->data, q->used);
    }
}

NxBool Nx_system_store(const NxSystem *system, NxBuffer *out)
{
    if (!system || !system->emitters || !out)
    {
        return false;
    }

    const NxU64 size = Nx_GB(10ull);

    NxWriter writer = {
          .at = 0,
          .data = Nx_virtual_alloc(size),
          .size = size,
    };

    NxU32 emitter_count = Nx_darray_len(system->emitters);
    write_NxU32(&writer, emitter_count);

    for (NxU32 i = 0; i < emitter_count; i++)
    {
        NxEmitterConfig *e = &system->emitters[i].config;

        write_particles(&writer, &e->particles);
        write_name(&writer, e->name);
        write_enabled(&writer, e->enabled);
        write_modules(&writer, &e->modules);
    }

    *out = (NxBuffer){
        .data = writer.data,
        .size = writer.at, 
    };

    return true;
}

void Nx_system_load(NxBuffer *buffer, NxSystem *out)
{
}
