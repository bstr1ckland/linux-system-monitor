#include <filesystem>
#include <fstream>
#include "utils.h"

using namespace std;
namespace fs = std::filesystem;

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
 * @returns GPU Usage in percent.
 */
double get_gpu_usage()
{
    std::ifstream file("/sys/class/drm/card1/device/gpu_busy_percent");
    double usage;

    file >> usage;
    file.close();

    return usage;
}

// TODO: IMPLEMENT VRAM USAGE AND TOTAL