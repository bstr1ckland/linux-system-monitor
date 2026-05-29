#include <filesystem>
#include <fstream>
#include <string>
#include "utils.h"

using namespace std;
namespace fs = std::filesystem;

/**
 * Reference:
 *   https://stackoverflow.com/questions/23834624/remove-first-and-last-character-c
 *   https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
 * 
 * @returns Name of GPU model.
 */
std::string get_gpu_name()
{
    std::string raw = exec("lspci | grep -E 'VGA|3D'");
    
    size_t start = raw.rfind('[');
    size_t end = raw.rfind(']');
    
    return raw.substr(start + 1, end - start - 1);
}

/**
 * Reference:
 *   https://unix.stackexchange.com/questions/576707/how-to-monitor-amd-gpu-on-linux-server
 * 
 * @returns Temperature of GPU in degrees (C).
 */
double get_gpu_temp()
{
    fs::path path = hwmon_iter("amdgpu");
    std::ifstream file(path.string() + "/temp1_input");
    double temp;

    file >> temp;
    file.close();

    return temp;
}

/**
 * Reference:
 *   https://unix.stackexchange.com/questions/576707/how-to-monitor-amd-gpu-on-linux-server
 * 
 * @returns GPU Usage in percent for single GPU systems, because who can afford more than one these days.
 */
double get_gpu_usage()
{
    std::ifstream file("/sys/class/drm/card1/device/gpu_busy_percent");
    double usage;

    file >> usage;
    file.close();

    return usage;
}

double get_total_vram()
{
    std::ifstream file("/sys/class/drm/card1/device/mem_info_vram_total");
    double total_vram;

    file >> total_vram;
    file.close();

    return total_vram;
}

double get_vram_usage()
{
    std::ifstream file("/sys/class/drm/card1/device/mem_info_vram_used");
    double vram_used;

    file >> vram_used;
    file.close();

    return vram_used;
}