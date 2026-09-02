
#pragma once

#include <string>
#include <format>

#include <raylib.h>

namespace utils
{
static inline std::string path_abs(const char *path)
{
	const char *base_path = GetApplicationDirectory();
	return std::format("{}/{}", base_path, path);
}
} // namespace utils
