
#pragma once

#include <types.hpp>

#include <string>

#include <Nexis/core.h>

struct ProjectHeader
{
	std::string file_path;
	// Without extension
	std::string file_name;
	i64 mod_time;
};

struct Project
{
	ProjectHeader header;
	NxSystem system;
};

static const char *NEXIS_PF_EX = ".nxp";
static const char *PROJECT_PATH = "projects";
