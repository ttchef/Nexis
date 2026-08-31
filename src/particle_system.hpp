
#pragma once

#include <globals.hpp>
#include <math.hpp>
#include <types.hpp>

#include <algorithm>
#include <string>
#include <variant>
#include <vector>

#include <raylib.h>

struct Particle
{
    Vec4 birth_color;
    Vec4 death_color;
    Vec3 pos;
    Vec3 vel;
    Vec3 acc;
    f32  lifetime;
    f32  start_lifetime;

    void update(f32 dt)
    {
        vel += acc * dt;
        pos += vel * dt;

        acc = Vec3::zero();

        lifetime -= dt;
    }

    void draw() const
    {
        auto color = death_color.lerp(birth_color, lifetime / start_lifetime);
        DrawSphere({pos.x, pos.y, pos.z}, 0.1f, color.raylib_color());
    }
};

struct EmitterShapeRectangle
{
    Vec2 pos;
    Vec2 size;
};

struct Emitter
{
    std::string           name;
    std::vector<Particle> particles;
    // particles per second
    f32 speed    = 2.0f;
    f32 lifetime = 1.0f;

    std::variant<EmitterShapeRectangle> shape = {EmitterShapeRectangle{}};
    bool                                render_shape;

    Vec4 birth_color = {1.0f, 1.0f, 1.0f, 1.0f};
    Vec4 death_color = {1.0f, 1.0f, 1.0f, 1.0f};

    Emitter() {}
    Emitter(std::string name) : name(name) {}

    void update(f32 dt)
    {
        elapsed_time += dt;

        const f32 interval = 1.0f / speed;

        while (elapsed_time >= interval)
        {
            elapsed_time -= interval;

            Vec3 pos{};

            std::visit([&](const auto &value)
                       {
                           using T = std::decay_t<decltype(value)>;

                           if constexpr (std::is_same_v<T, EmitterShapeRectangle>)
                           {
                                Vec2 min = value.pos - value.size * 0.5f;
                                Vec2 max = value.pos + value.size * 0.5f;
                                pos = {global.random(min.x, max.x), 0.0f, global.random(min.y, max.y)};     
                           } }, shape);

            particles.push_back({
                .birth_color = birth_color,
                .death_color = death_color,
                .pos      = pos,
                .vel      = {0.0f, 1.0f, 0.0f},
                .lifetime = lifetime,
                .start_lifetime = lifetime,
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
        if (render_shape)
        {
            std::visit([](auto &value)
                       {
                        using T = std::decay_t<decltype(value)>;

                        if constexpr (std::is_same_v<T, EmitterShapeRectangle>)
                        {
                            DrawCubeWires({value.pos.x, 0.0f, value.pos.y}, value.size.x, 0.1f, value.size.y, YELLOW);
                        } }, shape);
        }
        for (const auto &p : particles)
        {
            p.draw();
        }
    }

  private:
    f32 elapsed_time = 0.0f;
};
