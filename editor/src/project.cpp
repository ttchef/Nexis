
#include <project.hpp>

#include <fstream>

#include <Nexis/core.h>

void Project::create()
{
	Nx_system_create(&system);
	initialized = true;
}

void Project::destroy()
{
	if (initialized)
	{
		Nx_system_destroy(&system);
		initialized = false;
	}
}

void Project::load(ProjectHeader header)
{
	destroy();
	// TODO: Implement after NxSystem serializing is finished	
	create();
}

void Project::store()
{
	std::ofstream file(header.file_path);
	if (file.is_open())
	{
		file << "Was good\n";
	}	
}
