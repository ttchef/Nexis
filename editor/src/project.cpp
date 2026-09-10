
#include <project.hpp>

#include <fstream>
#include <iostream>
#include <vector>

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
	this->create();

	std::ifstream file(header.file_path, std::ios::binary);
	if (!file)
	{
		std::cout << "Failed to open: " << header.file_path << std::endl;
		return;
	}

	file.seekg(0, std::ios::end);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<u8> data(size);
	if (!file.read(reinterpret_cast<char *>(data.data()), size))
	{
		std::cout << "Failed to read file" << std::endl;
		return;
	}

	NxBuffer buffer = {
		.data = data.data(),
		.size = data.size(),	
	};

	Nx_system_load(&this->system, &buffer);
}

void Project::store()
{
	std::ofstream file(header.file_path, std::ios::binary);
	if (!file)
	{
		std::cout << "Failed to open: " << header.file_path << std::endl;
		return;
	}

	NxBuffer system_data;
	Nx_system_store(&this->system, &system_data);

	file.write(static_cast<char *>(system_data.data), static_cast<std::streamsize>(system_data.size));
	Nx_buffer_free(&system_data);

	if (!file)
	{
		std::cout << "Failed to write" << std::endl;
		return;
	}
}
