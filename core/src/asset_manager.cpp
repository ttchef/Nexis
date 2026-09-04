
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
	for (auto &[key, tex] : textures)
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

	Texture texture = {handle, path, GetFileNameWithoutExt(path.c_str())};
	TextureHandle key = texture.hash();
	textures[key] = texture;

	return key;
}

std::optional<Texture2D> Manager::get_texture_handle(TextureHandle handle)
{
	for (auto &[key, tex] : textures)
	{
		if (handle == key)
		{
			return tex.handle;
		}
	}

	return std::optional<Texture2D>();
}
} // namespace asset
