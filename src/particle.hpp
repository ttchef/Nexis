
#pragma once

#include <globals.hpp>
#include <math.hpp>
#include <types.hpp>
#include <camera.hpp>

#include <string>
#include <variant>
#include <vector>

#include <raylib.h>

namespace particle
{
struct Particle
{
    math::Vec4               birth_color;
    math::Vec4               death_color;
    math::Vec3               pos;
    math::Vec3               vel;
    math::Vec3               acc;
    f32                      age;
    f32                      lifetime;
    f32                      birth_size;
    f32                      death_size;
    std::optional<Texture2D> texture;

    void update(f32 dt);
    void draw(const SceneCamera &camera) const;
};

struct EmitterShapeRectangle
{
    math::Vec2 pos;
    math::Vec2 size;
};

enum struct EmitterBlending : i32
{
    Opaque,
    Additive,
};

static const char *emitter_blending_names[] = {
    "Opaque",
    "Additive",
};

struct ForceGravity
{
    math::Vec3 direction = {0.0f, -1.0f, 0.0f};
    f32  strength  = 4.0f;
};

struct ForcePoint
{
    math::Vec3 pos = {0.0f, 0.0f, 0.0f};
    f32  strength;
    f32  falloff_radius; // 0 = no falloff
};

using ForceType = std::variant<ForceGravity, ForcePoint>;

struct Force
{
    ForceType type;
    bool      enabled = true;

    math::Vec3 compute(math::Vec3 pos) const;
    const char *name() const;
};

struct Emitter
{
    bool                  enabled = true;
    std::string           name;
    std::vector<Particle> particles;
    std::vector<Force>    forces;

    // particles per second
    f32       speed    = 2.0f;
    RandomF32 lifetime = {1.0f, 0.0f};

    std::variant<EmitterShapeRectangle> shape              = {EmitterShapeRectangle{}};
    bool                                render_shape       = true;
    bool                                render_force_field = false;

    math::Vec4 birth_color = {1.0f, 1.0f, 1.0f, 1.0f};
    math::Vec4 death_color = {1.0f, 1.0f, 1.0f, 1.0f};

    RandomF32 birth_size = {0.08f, 0.0f};
    RandomF32 death_size = {0.08f, 0.0f};

    math::Vec3 direction = {0.0f, 1.0f, 0.0f};

    std::string              texture_path{};
    std::optional<Texture2D> texture{};

    EmitterBlending blending = EmitterBlending::Opaque;

    Emitter() {}
    Emitter(std::string name) : name(name) {}

    math::Vec3 get_force(math::Vec3 pos) const;
    void update(f32 dt);
    void draw_forcefield(u32 cell_count, f32 cell_size) const;
    void draw(const SceneCamera &camera) const;
    
  private:
    f32 elapsed_time = 0.0f;
};

struct System
{
    std::vector<Emitter> emitters;

    void update(f32 dt);
    void draw(const SceneCamera &camera) const;
};
} // namespace particle
