
#include <project.hpp>

#include <fstream>
#include <iostream>

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
	std::ofstream file(header.file_path, std::ios::binary);
	if (!file)
	{
		std::cout << "Failed to open: " << header.file_path << std::endl;
	}

	NxBuffer system_data;
	Nx_system_store(&this->system, &system_data);

	std::cout << system_data.size << std::endl;

	file.write(static_cast<char *>(system_data.data), static_cast<std::streamsize>(system_data.size));

	if (!file)
	{
		std::cout << "Failed to write" << std::endl;
	}
}
