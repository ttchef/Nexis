
#pragma once

#include <types.hpp>

#include <vector>
#include <string>
#include <optional>

#include <raylib.h>

namespace asset
{
using TextureHandle = usize;

struct Texture
{
	Texture2D handle;
	std::string path;
	// only the filename from the path
	std::string name;

	TextureHandle hash() const;
};

struct Manager
{
	std::vector<Texture> textures;

	~Manager();

	TextureHandle load_texture(const std::string &path);
	std::optional<Texture2D> get_texture_handle(TextureHandle handle);
};
} // namespace asset
