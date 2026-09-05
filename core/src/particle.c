
#include <Nexis/particle.h>
#include <darray.h>

void Nx_emitter_create(NxEmitter *out)
{
    if (!out)
    {
        return;
    }
    memset(out, 0, sizeof(NxEmitter));

    NxParticles *particles = &out->particles;

#define X(type, name) particles->name = Nx_darray_create(sizeof(type));
    Nx_PARTICLE_FIELDS(X)
#undef X
}

void Nx_emitter_destroy(NxEmitter *emitter)
{
    if (!emitter)
    {
        return;
    }

    NxParticles *particles = &emitter->particles;

#define X(type, name) Nx_darray_destroy(particles->name); particles->name = NULL;
    Nx_PARTICLE_FIELDS(X)
#undef X
}

void Nx_emitter_add_particle(NxEmitter *emitter, NxParticle particle)
{
    if (!emitter)
    {
        return;
    }

    NxParticles *particles = &emitter->particles;

#define X(type, name) !particles->name ||
    if (Nx_PARTICLE_FIELDS(X) 0)
    {
        fprintf(stderr, "[NEXIS] Tried calling 'Nx_emitter_add_particle' on an uninitialized emitter\n");
        return;
    }
#undef X

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
    if (!emitter)
    {
        return;
    }

    NxParticles *particles = &emitter->particles;
    particles_assert_same_len(particles);

    for (NxU32 i = 0; i < Nx_darray_len(particles->position); i++)
    {
        // TODO: Add more than a constant gravity
        particles->acceleration[i] = Nx_vec3(0.0f, -10.0f, 0.0f);
        particles->velocity[i]    = Nx_vec3_add(particles->velocity[i], Nx_vec3_scale(particles->acceleration[i], delta_time));
        particles->position[i]     = Nx_vec3_add(particles->position[i], Nx_vec3_scale(particles->velocity[i], delta_time));
        particles->acceleration[i] = Nx_vec3(0.0f, 0.0f, 0.0f);
    }
}

void Nx_emitter_render_particles(NxEmitter *emitter, NxRenderer *renderer)
{
    if (!emitter || !renderer)
    {
        return;
    }

    NxParticles *particles = &emitter->particles;
    particles_assert_same_len(particles);

    NxParticleBatch batch = {
        .particles = particles,
        .particle_count = Nx_darray_len(particles->position),
        .blending = emitter->blending,
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
    if (!system)
    {
        return;
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
#define X(type, name) emitter->particles.name = NULL;
    Nx_PARTICLE_FIELDS(X)
#undef X
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
