
#pragma once

#include <math.hpp>
#include <types.hpp>

#include <algorithm>
#include <string>
#include <vector>

#include <raylib.h>

struct Particle
{
    Vec3 pos;
    Vec3 vel;
    Vec3 acc;
    f32  lifetime;

    void update(f32 dt)
    {
        vel += acc * dt;
        pos += vel * dt;

        acc = Vec3::zero();

        lifetime -= dt;
    }

    void draw() const
    {
        DrawSphere((Vector3){pos.x, pos.y, pos.z}, 0.1f, WHITE);
    }
};

struct Emitter
{
    std::string           name;
    std::vector<Particle> particles;
    // particles per second
    f32 speed    = 0.0f;
    f32 lifetime = 0.0f;

    Emitter() {}
    Emitter(std::string name) : name(name) {}

    void update(f32 dt)
    {
        elapsed_time += dt;

        if (elapsed_time * speed > 1.0f)
        {
            elapsed_time = 0.0f;

            particles.push_back({
                .pos      = {0.0f, 0.0f, 0.0f},
                .vel      = {0.0f, 1.0f, 0.0f},
                .lifetime = lifetime,
            });
        }

        particles.erase(std::remove_if(particles.begin(), particles.end(), [](const auto &p)
                                       { return p.lifetime <= 0.0f; }),
                        particles.end());

        for (auto &p : particles)
        {
            p.update(dt);
        }
    }

    void draw() const
    {
        for (const auto &p : particles)
        {
            p.draw();
        }
    }

  private:
    f32 elapsed_time;
};
