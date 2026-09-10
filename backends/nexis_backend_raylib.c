
#include <raylib.h>

#include <Nexis/core.h>

void Nx_backend_raylib_render(NxParticleBatch batch)
{
    if (batch.blending == NxBlendingAdditive)
    {
        BeginBlendMode(BLEND_ADDITIVE);
    }

    NxParticles *particles = batch.particles;
    for (NxU32 i = 0; i < batch.particle_count; i++)
    {
        NxVec3 pos = particles->position[i];
        NxVec4 color = particles->color[i]; 
        DrawSphere((Vector3){pos.x, pos.y, pos.z}, particles->scale[i].x, (Color){(NxU8)(color.x * 255.0f), (NxU8)(color.y * 255.0f), (NxU8)(color.z * 255.0f), (NxU8)(color.w * 255.0f)});
    }

    if (batch.blending == NxBlendingAdditive)
    {
        EndBlendMode();
    }
}
