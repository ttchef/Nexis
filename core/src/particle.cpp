
#include <camera.hpp>
#include <particle.hpp>
#include <app_context.hpp>

#include <rlgl.h>

using namespace math;

static Color raylib_color(Vec4 c) {
    return {static_cast<u8>(c.x * 255), static_cast<u8>(c.y  * 255), static_cast<u8>(c.z * 255), static_cast<u8>(c.w * 255)};
}


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
    {
        DrawCylinderEx(corners[e[0]], corners[e[1]], thickness, thickness, 6, color);
    }
}

static void DrawVector3D(Vec3 pos, Vec3 vec, Color body_color, Color tip_color, float thickness = 0.02f)
{
    f32 length = vec.lensq();
    if (length < FLT_EPSILON)
    {
        return;
    }
    length = sqrtf(length);

    Vec3 tip = pos + vec;

    DrawCylinderEx({pos.x, pos.y, pos.z}, {tip.x, tip.y, tip.z}, thickness, thickness, 8, body_color);
    // DrawSphere({tip.x, tip.y, tip.z}, thickness + 0.02f, tip_color);
    DrawCube({tip.x, tip.y, tip.z}, thickness + 0.02f, thickness + 0.02f, thickness + 0.02f, tip_color);
}

namespace particle
{
void Particle::update(f32 dt)
{
    vel += acc * dt;
    pos += vel * dt;

    acc = Vec3::zero();

    age -= dt;
}

void Particle::draw(AppContext &ctx) const
{
    f32 t = std::clamp(1.0f - (age / lifetime), 0.0f, 1.0f);

    f32  size  = lerp(birth_size, death_size, t);
    auto color = birth_color.lerp(death_color, t);

    if (!texture)
    {
        DrawSphere({pos.x, pos.y, pos.z}, size, raylib_color(color));
    }
    else
    {
        auto tex = ctx.asset_manager->get_texture_handle(texture);
        if (tex)
        {
            DrawBillboard(ctx.camera->raylib, tex.value(), {pos.x, pos.y, pos.z}, size, raylib_color(color));
        }
    }
}

Vec3 Force::compute(Vec3 pos) const
{
    if (!enabled)
    {
        return Vec3::zero();
    }

    return std::visit([&](auto &&value) -> Vec3
                      {
            using T = std::decay_t<decltype(value)>;

            if constexpr (std::is_same_v<T, ForceGravity>)
            {
                return value.direction.norm() * value.strength;
            }
            else if constexpr (std::is_same_v<T, ForcePoint>)
            {
                Vec3 to_point = value.pos - pos;
                f32 dist = to_point.lensq();
                if (dist < FLT_EPSILON)
                {
                    return Vec3::zero();
                }
                dist = sqrtf(dist);

                Vec3 dir = to_point / dist;
                f32 falloff = value.falloff_radius > 0.0f ? std::clamp(1.0f - dist / value.falloff_radius, 0.0f, 1.0f) : 1.0f;
                return dir * value.strength * falloff;
            } }, this->type);
}

const char *Force::name() const
{
    return std::visit([](auto &&value) -> const char *
                      {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, ForceGravity>) return "Gravity";
        else if constexpr (std::is_same_v<T, ForcePoint>) return "Point"; }, this->type);
}

Vec3 Emitter::get_force(Vec3 pos) const
{
    Vec3 sum{};
    for (const auto &force : forces)
    {
        sum += force.compute(pos);
    }
    return sum;
}

void Emitter::update(f32 dt)
{
    if (!enabled)
    {
        return;
    }

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
        p.acc = get_force(p.pos);
        p.update(dt);
    }
}

void Emitter::draw_forcefield(u32 cell_count, f32 cell_size) const
{
    f32 half = cell_count * cell_size * 0.5f;

    for (u32 y = 0; y <= cell_count; y++)
    {
        for (u32 z = 0; z <= cell_count; z++)
        {
            for (u32 x = 0; x <= cell_count; x++)
            {
                Vec3 pos = {
                    -half + x * cell_size,
                    y * cell_size,
                    -half + z * cell_size,
                };

                Vec3 vec = get_force(pos);

                DrawVector3D(pos, vec.norm() * 0.08f, YELLOW, BLUE);
            }
        }
    }
}

void Emitter::draw(AppContext &ctx) const
{
    if (!enabled)
    {
        return;
    }

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
    if (render_force_field)
    {
        draw_forcefield(15, 0.25f);
    }
    if (blending == EmitterBlending::Additive)
    {
        rlDisableDepthMask();
        BeginBlendMode(BLEND_ADDITIVE);
    }
    for (const auto &p : particles)
    {
        p.draw(ctx);
    }
    if (blending == EmitterBlending::Additive)
    {
        EndBlendMode();
        rlEnableDepthMask();
    }
}

void System::update(f32 dt)
{
    for (auto &e : emitters)
    {
        e.update(dt);
    }
}

void System::draw(AppContext &ctx) const
{
    for (const auto &e : emitters)
    {
        e.draw(ctx);
    }
}
} // namespace particle

