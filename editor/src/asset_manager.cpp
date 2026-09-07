
#include <asset_manager.hpp>

#include <functional>
#include <iostream>
#include <ranges>

namespace asset
{
TextureHandle Texture::hash() const
{
    return std::hash<std::string>()(this->path);
}

Manager::~Manager()
{
    for (auto &tex : this->textures | std::views::values)
    {
        if (tex.handle.id)
        {
            UnloadTexture(tex.handle);
        }
    }
}

TextureHandle Manager::load_texture(const std::string &path)
{
    Texture2D handle = LoadTexture(path.c_str());
    if (!handle.id)
    {
        std::cout << "Failed to load texture: " << path << std::endl;
        std::exit(1);
    }

    Texture       texture = {handle, path, GetFileNameWithoutExt(path.c_str())};
    TextureHandle key     = texture.hash();
    this->textures[key]         = texture;

    return key;
}
} // namespace asset
