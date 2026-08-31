
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
