#include <filesystem>
#include <fstream>
#include "utils.h"

using namespace std;
namespace fs = std::filesystem;
/**
 * @returns Temperature of GPU in degrees (C).
 */
double get_gpu_temp()
{
    fs::path path = hwmon_iter("amdgpu");
    std::ifstream file(path.string() + "/temp1_input");

    double temp;

    file >> temp;
    return temp;
}

double get_gpu_usage()
{
    std::ifstream file("/sys/class/drm/card1/device/gpu_busy_percent");
    double usage;

    file >> usage;
    return usage;
}

// IMPLEMENT VRAM USAGE AND TOTAL