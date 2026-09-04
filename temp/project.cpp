
#include <project.hpp>
#include <globals.hpp>

#include <fstream>
#include <iostream>
#include <filesystem>

void Project::write()
{	
	std::ofstream file(file_path);
	if (!file.is_open())
	{
		std::cout << "Failed to write project: " << file_name << std::endl;
		std::exit(1);
	}

	file << "Hello wsp";
}

void Project::remove()
{
	if (!std::filesystem::remove(file_path))
	{
		std::cout << "Failed to remove project" << std::endl;
		std::exit(1);
	}	
}


