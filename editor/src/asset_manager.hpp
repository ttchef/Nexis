
#pragma once

#include <types.hpp>

#include <string>
#include <unordered_map>

#include <raylib.h>

namespace asset
{
using TextureHandle = usize;

struct Texture
{
    Texture2D   handle;
    std::string path;
    // only the filename from the path
    std::string name;

    TextureHandle hash() const;
};

struct Manager
{
    std::unordered_map<TextureHandle, Texture> textures;

    ~Manager();

    TextureHandle load_texture(const std::string &path);
};
} // namespace asset
