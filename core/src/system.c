
#include <Nexis/system.h>
#include <darray.h>
#include <module_behaviour.h>

void Nx_emitter_create(NxEmitter *out)
{
    if (!out)
    {
        return;
    }
    memset(out, 0, sizeof(NxEmitter));

    NxParticles *particles = &out->runtime.particles;

#define X(type, name) particles->name = Nx_darray_create(sizeof(type));
    Nx_PARTICLE_FIELDS(X)
#undef X

        Nx_modules_create(&out->config.modules);

    out->config.enabled = true;
}

void Nx_emitter_destroy(NxEmitter *emitter)
{
    if (!emitter)
    {
        return;
    }

    NxParticles *particles = &emitter->runtime.particles;

#define X(type, name)                   \
    Nx_darray_destroy(particles->name); \
    particles->name = NULL;
    Nx_PARTICLE_FIELDS(X)
#undef X

        Nx_modules_destroy(&emitter->config.modules);
}

static void on_event(NxModuleType type, void *module_data, void *userdata)
{
    switch (type)
    {
#define Nx_FIELD(...)
#define Nx_MODULE(name, display, queue_index, ...)         \
    case NxModuleType_##name:                              \
    {                                                      \
        Nx_module_behaviour_##name(userdata, module_data); \
    }                                                      \
    break;
        Nx_MODULES(Nx_MODULE, Nx_FIELD)
#undef Nx_MODULE
#undef Nx_FIELD
            default : break;
    }
}

void Nx_emitter_add_particle(NxEmitter *emitter, NxParticle particle)
{
    if (!emitter)
    {
        return;
    }

    NxParticles *particles = &emitter->runtime.particles;

#define X(type, name) !particles->name ||
    if (Nx_PARTICLE_FIELDS(X) 0)
    {
        fprintf(stderr, "[NEXIS] Tried calling 'Nx_emitter_add_particle' on an uninitialized emitter\n");
        return;
    }
#undef X

    NxParticleOnSpawnData data = {
        .particle = &particle,
    };
    NxCallbackData callback_data = {
        .queue = NxModuleQueue_ParticleSpawn,
        .data  = &data,
    };
    Nx_modules_for_each(&emitter->config.modules, NxModuleQueue_ParticleSpawn, on_event, &callback_data);

#define X(type, name) Nx_darray_push((void **)&particles->name, &particle.name);
    Nx_PARTICLE_FIELDS(X)
#undef X
}

static void particles_assert_same_len(NxParticles *particles)
{
    NxU32 position_len = Nx_darray_len(particles->position);
#define X(type, name) assert(Nx_darray_len(particles->name) == position_len);
    Nx_PARTICLE_FIELDS(X)
#undef X
}

void Nx_emitter_update_particles(NxEmitter *emitter, NxF32 delta_time)
{
    if (!emitter || !emitter->config.enabled)
    {
        return;
    }

    NxParticles *particles = &emitter->runtime.particles;
    particles_assert_same_len(particles);

    NxEmitterOnUpdateData data = {
        .emitter    = emitter,
        .delta_time = delta_time};
    NxCallbackData callback_data = {
        .queue = NxModuleQueue_EmitterUpdate,
        .data  = &data,
    };
    Nx_modules_for_each(&emitter->config.modules, NxModuleQueue_EmitterUpdate, on_event, &callback_data);

    for (NxU32 i = 0; i < Nx_darray_len(particles->position); i++)
    {
        NxParticleOnUpdateData data = {
#define X(type, name) .name = &particles->name[i],
            Nx_PARTICLE_FIELDS(X)
#undef X
                .delta_time = delta_time,
        };

        NxCallbackData callback_data = {
            .queue = NxModuleQueue_ParticleUpdate,
            .data  = &data,
        };
        Nx_modules_for_each(&emitter->config.modules, NxModuleQueue_ParticleUpdate, on_event, &callback_data);
    }
}

void Nx_emitter_render_particles(NxEmitter *emitter, NxRenderer *renderer)
{
    if (!emitter || !emitter->config.enabled || !renderer)
    {
        return;
    }

    NxParticles *particles = &emitter->runtime.particles;
    particles_assert_same_len(particles);

    NxParticleBatch batch = {
        .particles      = particles,
        .particle_count = Nx_darray_len(particles->position),
        .blending       = emitter->config.blending,
    };

    renderer->particles_draw(batch);
}

void Nx_system_create(NxSystem *out)
{
    if (!out)
    {
        return;
    }
    memset(out, 0, sizeof(NxSystem));

    out->emitters = Nx_darray_create(sizeof(NxEmitter));
}

void Nx_system_destroy(NxSystem *system)
{
    if (!system || !system->emitters)
    {
        return;
    }

    for (NxU32 i = 0; i < Nx_darray_len(system->emitters); i++)
    {
        Nx_emitter_destroy(&system->emitters[i]);
    }

    Nx_darray_destroy(system->emitters);
}

void Nx_system_add_emitter(NxSystem *system, NxEmitter *emitter)
{
    if (!system || !emitter)
    {
        return;
    }

    if (!system->emitters)
    {
        fprintf(stderr, "[NEXIS] Tried to call 'Nx_system_add_emitter' on an uninitialized system\n");
        return;
    }

    Nx_darray_push((void **)&system->emitters, emitter);

    // NOTE: Emitter resources will be managed from the system now
#define X(type, name) emitter->runtime.particles.name = NULL;
    Nx_PARTICLE_FIELDS(X)
#undef X
        for (NxU32 i = 0; i < NxModuleQueue_Count; i++)
    {
        emitter->config.modules.queues[i].data = NULL;
        emitter->config.modules.queues[i].used = 0;
    }
}

void Nx_system_update_emitters(NxSystem *system, NxF32 delta_time)
{
    if (!system)
    {
        return;
    }

    for (NxU32 i = 0; i < Nx_darray_len(system->emitters); i++)
    {
        Nx_emitter_update_particles(&system->emitters[i], delta_time);
    }
}

void Nx_system_render_emitters(NxSystem *system, NxRenderer *renderer)
{
    if (!system || !renderer)
    {
        return;
    }

    for (NxU32 i = 0; i < Nx_darray_len(system->emitters); i++)
    {
        Nx_emitter_render_particles(&system->emitters[i], renderer);
    }
}

NxU32 Nx_system_emitter_count(NxSystem *system)
{
    if (!system)
    {
        return 0;
    }

    return Nx_darray_len(system->emitters);
}
