
#include <project.hpp>

#include <fstream>

#include <Nexis/core.h>

void Project::create()
{
	Nx_system_create(&this->system);
	this->initialized = true;
}

void Project::destroy()
{
	if (this->initialized)
	{
		Nx_system_destroy(&this->system);
		this->initialized = false;
	}
}

void Project::load(ProjectHeader header)
{
	this->destroy();

	this->header = header;
	
	// TODO: Implement after NxSystem serializing is finished	
	this->create();
}

void Project::store()
{
	std::ofstream file(header.file_path);
	if (file.is_open())
	{
		file << "Was good\n";
	}	
}
