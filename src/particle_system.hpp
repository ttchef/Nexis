
#pragma once

#include <types.hpp>
#include <math.hpp>

#include <vector>

#include <raylib.h>

struct Particle
{
    Vec3 pos;
    Vec3 vel;
    Vec3 acc;
    f32 lifetime;

    void update(f32 dt) {
        vel += acc * dt;
        pos += vel * dt;

        acc = Vec3::zero();
    }

    void draw() const {
        DrawSphere((Vector3){pos.x, pos.y, pos.z}, 0.3f, WHITE);
    }
};

struct Emitter
{
    std::vector<Particle> particles;
    // particles per second
    f32 speed;

    Emitter(f32 speed) : speed(speed) {}

    void update(f32 dt) {
        elapsed_time += dt;

        if (elapsed_time * speed > 1.0f) {
            elapsed_time = 0.0f;

            particles.push_back({
                .pos = {0.0f, 0.0f, 0.0f},
                .vel = {0.0f, 1.0f, 0.0f},                        
            });
        }

        for (auto &p : particles) {
            p.update(dt);
        }
    }

    void draw() const {
        for (const auto &p : particles) {
            p.draw();
        }
    }

private:
    f32 elapsed_time;
};
