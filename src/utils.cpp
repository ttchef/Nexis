
#include <utils.hpp>
#include <project.hpp>

#include <iostream>

#include <raylib.h>

namespace utils
{
void load_projects(std::vector<Project> *projects)
{
    if (!projects->empty())
    {
        projects->clear();
    }

    if (!DirectoryExists(global.project_path.c_str()))
    {
        if (MakeDirectory(global.project_path.c_str()) != 0)
        {
            std::cout << "Failed to make projects directory" << std::endl;
            std::exit(1);
        }
        return;
    }

    FilePathList files = LoadDirectoryFilesEx(global.project_path.c_str(), NEXIS_PF_EX, false);

    projects->reserve(files.count);
    for (u32 i = 0; i < files.count; i++)
    {
        projects->push_back({
            .file_path = files.paths[i],
            .file_name = GetFileNameWithoutExt(files.paths[i]),
            .mod_time  = GetFileModTime(files.paths[i]),
        });
    }
}
} // namespace utils
