
#pragma once

#include <types.hpp>

#include <string>
#include <format>
#include <vector>

#include <raylib.h>

struct ProjectHeader;

namespace utils
{	
static inline std::string path_abs(const char *path)
{
	const char *base_path = GetApplicationDirectory();
	return std::format("{}{}", base_path, path);
}

void load_projects(std::vector<ProjectHeader> *projects);
} // namespace utils
