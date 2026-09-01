
#pragma once

#include <globals.hpp>
#include <math.hpp>
#include <types.hpp>

#include <algorithm>
#include <string>
#include <variant>
#include <vector>

#include <raylib.h>
#include <rlgl.h>

using namespace math;

struct Particle
{
    Vec4                     birth_color;
    Vec4                     death_color;
    Vec3                     pos;
    Vec3                     vel;
    Vec3                     acc;
    f32                      age;
    f32                      lifetime;
    f32                      birth_size;
    f32                      death_size;
    std::optional<Texture2D> texture;

    void update(f32 dt)
    {
        vel += acc * dt;
        pos += vel * dt;

        acc = Vec3::zero();

        age -= dt;
    }

    void draw(const Camera3D &camera) const
    {
        f32 t = std::clamp(1.0f - (age / lifetime), 0.0f, 1.0f);

        f32  size  = lerp(birth_size, death_size, t);
        auto color = birth_color.lerp(death_color, t);

        if (!texture)
        {
            DrawSphere({pos.x, pos.y, pos.z}, size, color.raylib_color());
        }
        else
        {
            DrawBillboard(camera, texture.value(), {pos.x, pos.y, pos.z}, size, color.raylib_color());
        }
    }
};

struct EmitterShapeRectangle
{
    Vec2 pos;
    Vec2 size;
};

// Temporary
static void DrawCubeWiresThick(Vector3 position, float width, float height, float length, float thickness, Color color)
{
    float x = width / 2.0f;
    float y = height / 2.0f;
    float z = length / 2.0f;

    Vector3 corners[8] = {
        {position.x - x, position.y - y, position.z - z},
        {position.x + x, position.y - y, position.z - z},
        {position.x + x, position.y + y, position.z - z},
        {position.x - x, position.y + y, position.z - z},
        {position.x - x, position.y - y, position.z + z},
        {position.x + x, position.y - y, position.z + z},
        {position.x + x, position.y + y, position.z + z},
        {position.x - x, position.y + y, position.z + z},
    };

    int edges[12][2] = {
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 0},
        {4, 5},
        {5, 6},
        {6, 7},
        {7, 4},
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7},
    };

    for (auto &e : edges)
        DrawCylinderEx(corners[e[0]], corners[e[1]], thickness, thickness, 6, color);
}

enum struct EmitterBlending : i32
{
    Opaque,
    Additive,
};

static const char *emitter_blending_names[] = {
    "Opaque",
    "Additive",
};

struct Emitter
{
    std::string           name;
    std::vector<Particle> particles;
    // particles per second
    f32       speed    = 2.0f;
    RandomF32 lifetime = {1.0f, 0.0f};

    std::variant<EmitterShapeRectangle> shape        = {EmitterShapeRectangle{}};
    bool                                render_shape = true;

    Vec4 birth_color = {1.0f, 1.0f, 1.0f, 1.0f};
    Vec4 death_color = {1.0f, 1.0f, 1.0f, 1.0f};

    RandomF32 birth_size = {0.08f, 0.0f};
    RandomF32 death_size = {0.08f, 0.0f};

    Vec3 direction = {0.0f, 1.0f, 0.0f};

    std::string              texture_path{};
    std::optional<Texture2D> texture{};

    EmitterBlending blending = EmitterBlending::Opaque;

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

            f32 particle_lifetime = lifetime.sample(global.random);
            particles.push_back({
                .birth_color = birth_color,
                .death_color = death_color,
                .pos         = pos,
                .vel         = direction,
                .age         = particle_lifetime,
                .lifetime    = particle_lifetime,
                .birth_size  = birth_size.sample(global.random),
                .death_size  = death_size.sample(global.random),
                .texture     = texture,
            });
        }

        for (u32 i = 0; i < particles.size();)
        {
            if (particles[i].age <= 0.0f)
            {
                particles[i] = particles.back();
                particles.pop_back();
            }
            else
            {
                i++;
            }
        }

        for (auto &p : particles)
        {
            p.update(dt);
        }
    }

    void draw(const Camera3D &camera) const
    {
        if (render_shape)
        {
            std::visit([](auto &value)
                       {
                        using T = std::decay_t<decltype(value)>;

                        if constexpr (std::is_same_v<T, EmitterShapeRectangle>)
                        {
                            DrawCubeWiresThick({value.pos.x, 0.0f, value.pos.y}, value.size.x, 0.1f, value.size.y, 0.01f, YELLOW);
                        } }, shape);
        }
        if (blending == EmitterBlending::Additive)
        {
            rlDisableDepthMask();
            BeginBlendMode(BLEND_ADDITIVE);
        }
        for (const auto &p : particles)
        {
            p.draw(camera);
        }
        if (blending == EmitterBlending::Additive)
        {
            EndBlendMode();
            rlEnableDepthMask();
        }
    }

  private:
    f32 elapsed_time = 0.0f;
};
