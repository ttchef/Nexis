
#include <utils.hpp>
#include <project.hpp>

#include <iostream>

#include <raylib.h>

namespace utils
{
void load_projects(std::vector<ProjectHeader> *projects)
{
    std::string project_path = utils::path_abs("projects");

    if (!projects->empty())
    {
        projects->clear();
    }

    if (!DirectoryExists(project_path.c_str()))
    {
        if (MakeDirectory(project_path.c_str()) != 0)
        {
            std::cout << "Failed to make projects directory" << std::endl;
            std::exit(1);
        }
        return;
    }

    FilePathList files = LoadDirectoryFilesEx(project_path.c_str(), NEXIS_PF_EX, false);

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
