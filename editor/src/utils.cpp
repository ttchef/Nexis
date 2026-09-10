
#include <project.hpp>
#include <utils.hpp>

#include <filesystem>
#include <iostream>

namespace utils
{
void load_projects(std::vector<ProjectHeader> *projects)
{
    const std::filesystem::path project_path = utils::path_abs(PROJECT_PATH);
    projects->clear();

    std::error_code error;

    if (!std::filesystem::exists(project_path, error))
    {
        if (!std::filesystem::create_directories(project_path, error) || error)
        {
            std::cout << "Failed to make projects directory: " << error.message() << '\n';
            std::exit(1);
        }
        return;
    }

    for (const auto &entry : std::filesystem::directory_iterator(project_path, error))
    {
        if (error)
        {
            std::cout << "Failed to read projects directory: " << error.message() << '\n';
            return;
        }

        if (!entry.is_regular_file())
        {
            continue;
        }

        const std::filesystem::path &path = entry.path();
        if (path.extension() != NEXIS_PF_EX)
        {
            continue;
        }

        projects->push_back({
            .file_path = path,
            .file_name = path.stem().string(),
            .mod_time  = std::filesystem::last_write_time(path, error),
        });
    }
}
} // namespace utils
