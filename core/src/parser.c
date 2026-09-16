
#include <Nexis/parser.h>
#include <alloc.h>
#include <darray.h>

typedef struct
{
    NxU8         *data;
    NxUsize       size;
    NxUsize       at;
    NxParseResult result;
} NxMemoryStream;

#define Nx_MAX_FILE_SIZE (Nx_GB(10ull))

typedef struct
{
    NxU32 patch : 12;
    NxU32 minor : 12;
    NxU32 major : 8;
} NxFileVersion;

#define Nx_FOURCC_LE(a, b, c, d)                      ((NxU32)(a) | ((NxU32)(b) << 8) | ((NxU32)(c) << 16) | ((NxU32)(d) << 24))
#define Nx_MAKE_VERSION(major_in, minor_in, patch_in) ((NxFileVersion){ \
    .patch = patch_in,                                                  \
    .minor = minor_in,                                                  \
    .major = major_in,                                                  \
})

const NxU32         SIGNATURE              = Nx_FOURCC_LE('n', 'x', 'p', ' ');
const NxFileVersion MIN_COMPATIBLE_VERSION = Nx_MAKE_VERSION(0, 1, 0);

typedef union
{
    NxU32         u32;
    NxFileVersion file_version;
} NxConversionUnion;

// NOTE: Write functions

static inline void write(NxMemoryStream *stream, void *data, NxUsize size)
{
    if (stream->result != NxParse_Success)
    {
        return;
    }

    if (size > stream->size - stream->at)
    {
        stream->result = NxParse_BufferOverflow;
        return;
    }

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
    write(stream, &enabled, sizeof(NxU8));
}

void write_blending(NxMemoryStream *stream, NxBlending blending)
{
    write_NxU32(stream, blending);
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
    if (stream->result != NxParse_Success)
    {
        return NULL;
    }

    if (size > stream->at)
    {
        stream->result = NxParse_UnexpectedEOF;
        return NULL;
    }

    void *data = &stream->data[stream->size - stream->at];
    stream->at -= size;
    return data;
}

static inline NxU8 read_NxU8(NxMemoryStream *stream)
{
    NxU8 *p = read(stream, sizeof(NxU8));
    return p ? *p : 0;
}

static inline NxU32 read_NxU32(NxMemoryStream *stream)
{
    NxU32 *p = read(stream, sizeof(NxU32));
    return p ? *p : 0;
}

static inline NxU64 read_NxU64(NxMemoryStream *stream)
{
    NxU64 *p = read(stream, sizeof(NxU64));
    return p ? *p : 0;
}

void read_name(NxMemoryStream *stream, NxChar name[Nx_EMITTER_NAME_LEN])
{
    void *p = read(stream, Nx_EMITTER_NAME_LEN);
    if (p)
    {
        memcpy(name, p, Nx_EMITTER_NAME_LEN);
    }
}

void read_enabled(NxMemoryStream *stream, NxBool *enabled)
{
    *enabled = read_NxU8(stream);
}

void read_blending(NxMemoryStream *stream, NxBlending *blending)
{
    *blending = read_NxU32(stream);
}

void read_modules(NxMemoryStream *stream, NxModules *modules)
{
    for (NxU32 i = 0; i < NxModuleQueue_Count; i++)
    {
        NxModuleQueue *q = &modules->queues[i];

        NxU64 used = read_NxU64(stream);
        if (used > Nx_QUEUE_SIZE)
        {
            stream->result = NxParse_CorruptData;
            return;
        }
        void *data = read(stream, used);
        if (!data)
        {
            return;
        }

        q->used = used;
        memcpy(q->data, data, q->used);
    }
}

// -------

NxParseResult Nx_system_store(const NxSystem *system, NxBuffer *out)
{
    if (!system || !system->emitters || !out)
    {
        return NxParse_InvalidParameters;
    }

    NxMemoryStream stream = {
        .at     = 0,
        .data   = Nx_virtual_alloc(Nx_MAX_FILE_SIZE),
        .size   = Nx_MAX_FILE_SIZE,
        .result = NxParse_Success,
    };

    write_NxU32(&stream, SIGNATURE);
    write(&stream, (void *)&MIN_COMPATIBLE_VERSION, sizeof(MIN_COMPATIBLE_VERSION));

    NxU32 emitter_count = Nx_darray_len(system->emitters);
    write_NxU32(&stream, emitter_count);

    for (NxU32 i = 0; i < emitter_count; i++)
    {
        NxEmitterConfig *e = &system->emitters[i].config;

        write_name(&stream, e->name);
        write_enabled(&stream, e->enabled);
        write_blending(&stream, e->blending);
        write_modules(&stream, &e->modules);
    }

    if (stream.result != NxParse_Success)
    {
        Nx_virtual_free(stream.data, Nx_MAX_FILE_SIZE);
        return stream.result;
    }

    *out = (NxBuffer){
        .data = stream.data,
        .size = stream.at,
    };

    return NxParse_Success;
}

NxParseResult Nx_system_load(NxSystem *system, const void *data, NxUsize size)
{
    if (!system || !data)
    {
        return NxParse_InvalidParameters;
    }

    NxMemoryStream stream = {
        .at     = size,
        .size   = size,
        .data   = (void *)data,
        .result = NxParse_Success,
    };

    if (read_NxU32(&stream) != SIGNATURE)
    {
        return NxParse_InvalidSignature;
    }

    NxConversionUnion version = (NxConversionUnion){
        .u32 = read_NxU32(&stream),
    };

    if (version.file_version.major < MIN_COMPATIBLE_VERSION.major)
    {
        return NxParse_InvalidVersion;
    }

    NxU32 emitter_count = read_NxU32(&stream);

    for (NxU32 i = 0; i < emitter_count; i++)
    {
        NxEmitter e = {0};
        Nx_emitter_create(&e);

        read_name(&stream, e.config.name);
        read_enabled(&stream, &e.config.enabled);
        read_blending(&stream, &e.config.blending);
        read_modules(&stream, &e.config.modules);

        if (stream.result != NxParse_Success)
        {
            break;
        }

        Nx_system_add_emitter(system, &e);
    }

    return stream.result;
}

void Nx_buffer_free(NxBuffer *buffer)
{
    if (buffer && buffer->data)
    {
        Nx_virtual_free(buffer->data, Nx_MAX_FILE_SIZE);
        buffer->data = 0;
        buffer->size = 0;
    }
}
