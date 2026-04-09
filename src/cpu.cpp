#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include "cpu.h"

using namespace std;

/**
 * @returns The amount of total cores in the CPU.
 */
uint8_t get_core_count()
{
    std::ifstream file("/proc/cpuinfo");
    std::string line;

    while (getline(file, line))
    {
        if (line.find("siblings") != std::string::npos)
        {
            size_t value = line.find_first_of("0123456789");
            return std::stoi(line.substr(value));
        }
    }
    file.close();

    return -1;
}

// https://www.linuxhowtos.org/System/procstat.htm
/**
 * @returns The total CPU usage in percentage.
 */
uint8_t get_cpu_usage()
{
    std::ifstream file("/proc/stat");
    std::string line;

    return 0;
}

// /proc/stat
std::vector<uint8_t> get_cpu_usage_per_core()
{
    return {0};
}