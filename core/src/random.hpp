
#pragma once

#include <types.hpp>

#include <random>

struct Random
{
    std::mt19937 gen{std::random_device{}()};

    f32 operator()(f32 min, f32 max)
    {
        return std::uniform_real_distribution<f32>(min, max)(gen);
    }
};

struct RandomF32
{
    f32 value;
    // range
    f32 offset;

    f32 sample(Random &gen) const
    {
        if (offset <= 0.0f)
        {
            return value;
        }
        return gen(std::max(value - offset, 0.0f), value + offset);
    }
};
