
#include <module_behaviour.h>
#include <darray.h>

void Nx_module_behaviour_SpawnRate(NxCallbackData *callback_data, NxModuleSpawnRate *module)
{
    NxEmitterOnUpdateData *data = Nx_CALLBACK_DATA(SpawnRate, callback_data);
    module->elapsed_time += data->delta_time;

    const NxF32 interval = 1.0f / module->emit_speed;

    while (module->elapsed_time >= interval)
    {
        module->elapsed_time -= interval;

        Nx_emitter_add_particle(data->emitter, (NxParticle){
                                                   .position = {0, 0, 0},
                                                   .velocity = {0, 0, 0},
                                                   .scale    = {0.1, 0.1f, 0.1f},
                                               });
    }
}

void Nx_module_behaviour_SpawnBurst(NxCallbackData *callback_data, NxModuleSpawnBurst *module)
{
    NxEmitterOnUpdateData *data = Nx_CALLBACK_DATA(SpawnBurst, callback_data);

    if (module->trigger_count >= 1)
    {
        --module->trigger_count;
        for (NxU32 i = 0; i < module->particle_count; i++)
        {
            Nx_emitter_add_particle(data->emitter, (NxParticle){
                                                       .position = {0, 0, 0},
                                                       .velocity = {0, 0, 0},
                                                       .scale    = {0.1, 0.1f, 0.1f},
                                                   });
        }
    }
}

void Nx_module_behaviour_AddVelocity(NxCallbackData *callback_data, NxModuleAddVelocity *module)
{
    NxParticleOnSpawnData *data = Nx_CALLBACK_DATA(AddVelocity, callback_data);

    data->particle->velocity = Nx_vec3_scale(Nx_vec3_norm(module->direction), module->speed);
}

void Nx_module_behaviour_GravityForce(NxCallbackData *callback_data, NxModuleGravityForce *module)
{
    NxParticleOnUpdateData *data = Nx_CALLBACK_DATA(GravityForce, callback_data);

    *data->acceleration = Nx_vec3_add(*data->acceleration, Nx_vec3_scale(Nx_vec3_norm(module->direction), module->strength));
}

void Nx_module_behaviour_SolveVelocityAndForces(NxCallbackData *callback_data, NxModuleSolveVelocityAndForces *module)
{
    NxParticleOnUpdateData *data = Nx_CALLBACK_DATA(SolveVelocityAndForces, callback_data);

    *data->velocity     = Nx_vec3_add(*data->velocity, Nx_vec3_scale(*data->acceleration, data->delta_time));
    *data->position     = Nx_vec3_add(*data->position, Nx_vec3_scale(*data->velocity, data->delta_time));
    *data->acceleration = Nx_vec3(0.0f, 0.0f, 0.0f);
}

void Nx_module_behaviour_InitParticle(NxCallbackData *callback_data, NxModuleInitParticle *module)
{
    NxParticleOnSpawnData *data = Nx_CALLBACK_DATA(InitParticle, callback_data);

    data->particle->lifetime = module->lifetime;
    data->particle->color = module->color;
}

void Nx_module_behaviour_SolveLifetime(NxCallbackData *callback_data, NxModuleSolveLifetime *module)
{
    NxParticleOnUpdateData *data = Nx_CALLBACK_DATA(SolveLifetime, callback_data);

    if (*data->lifetime <= 0.0f)
    {
        NxU32 last = Nx_darray_len(data->particles->position) - 1;
        
#define X(type, name)\
*data->name = data->particles->name[last];\
Nx_darray_len_set(data->particles->name, last);

Nx_PARTICLE_FIELDS(X)
#undef X

    }

    *data->lifetime -= data->delta_time;
}
