#include <cstdint>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#include "cpu.h"

using namespace std;

/**
 * Reference: 
 *  https://stackoverflow.com/questions/30073839/c-extract-number-from-the-middle-of-a-string
 * 
 * @returns The amount of total cores in the CPU.
 */
uint8_t get_core_count()
{
    std::ifstream file("/proc/cpuinfo");
    std::string line;

    // siblings line appears multiple times in file, just grab first instance
    // (since they're all identical)
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


/**
 * References:
 *  https://www.linuxhowtos.org/System/procstat.htm ,
 *  https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
 * 
 * @returns The total CPU usage in percentage.
 */
uint8_t get_cpu_usage()
{
    std::ifstream file("/proc/stat");
    std::string line;
    std::string label; // cpu label, unused

    // Processes
    unsigned long long user, niced, system, idle, iowait, irq, softirq;

    file >> label >> user >> niced >> system >> idle >> iowait >> irq >> softirq;

    // Do calculation. NOTE: There are 3 trailing 0's present at the end of each CPU line in /proc/stat

    return -1;
}


std::vector<uint8_t> get_cpu_usage_per_core()
{
    std::ifstream file("/proc/stat");
    std::string line;
    std::vector<uint8_t> core_percentages;

    uint8_t core_count = get_core_count();

    return {0};
}