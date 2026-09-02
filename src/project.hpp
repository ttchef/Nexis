
#pragma once

#include <types.hpp>

#include <string>

struct Project
{
	std::string file_path;
	// Without extension
	std::string file_name;
	i64 mod_time;
};

static const char *NEXIS_PF_EX = ".nxp";

