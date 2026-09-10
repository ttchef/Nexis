
#pragma once

#include <types.hpp>

#include <filesystem>

#include <Nexis/core.h>

struct ProjectHeader
{
    std::filesystem::path file_path;
    // Without extension
    std::string file_name;
    std::filesystem::file_time_type mod_time;
};

struct Project
{
    ProjectHeader header;
    NxSystem      system;
    bool initialized;

    // NOTE: Cant be done in the constructor because there exists one
    // project in the lifetime of the application but you can go and load other
    // projects in that same lifetime. Idk i cant explain so yeah.
    void create();
    void destroy();
     
    void load(ProjectHeader header);
    void store();
};

static const char *NEXIS_PF_EX  = ".nxp";
static const char *PROJECT_PATH = "projects";
