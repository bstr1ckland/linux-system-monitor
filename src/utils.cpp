#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

/**
 * References: 
 *   https://stackoverflow.com/questions/67273/how-do-you-iterate-through-every-file-directory-recursively-in-standard-c
 *   https://en.cppreference.com/cpp/filesystem/path/operator_slash?utm_source=chatgpt.com
 * 
 * @param target_name wanted information (amdgpu, k10temp (CPU), nvme).
 * @returns the directory where target_info is located.
 */
fs::path hwmon_iter(std::string target_name)
{
    std::string path = "/sys/class/hwmon";
    
    for (const auto& entry : fs::directory_iterator(path))
    {
        fs::path name_file = entry.path() / "name";

        std::ifstream file(name_file);
        std::string name;
        file >> name;

        file.close();

        if (name == target_name)
        {
            return entry.path();
        }
    }

    return fs::path(); // empty path, not found
}