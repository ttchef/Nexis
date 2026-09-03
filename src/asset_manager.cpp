
#include <asset_manager.hpp>

#include <functional>
#include <iostream>

namespace asset
{
TextureHandle Texture::hash() const
{
	return std::hash<std::string>()(path);
}

Manager::~Manager()
{
	for (auto &tex : textures)
	{
		if (tex.handle.id)
		{
			UnloadTexture(tex.handle);
		}
	}
}

TextureHandle Manager::load_texture(const std::string &path)
{
	Texture2D texture = LoadTexture(path.c_str());
	if (!texture.id)
	{
		std::cout << "Failed to load texture: " << path << std::endl;
		std::exit(1);
	}

	textures.push_back({texture, path, GetFileNameWithoutExt(path.c_str())});

	return textures.back().hash();
}

std::optional<Texture2D> Manager::get_texture_handle(TextureHandle handle)
{
	for (auto &tex : textures)
	{
		if (handle == tex.hash())
		{
			return tex.handle;
		}
	}

	return std::optional<Texture2D>();
}
} // namespace asset
