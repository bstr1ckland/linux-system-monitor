#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <vector>

#include "cpu.h"

using namespace std;

/**
 * Reference: 
 *  https://stackoverflow.com/questions/30073839/c-extract-number-from-the-middle-of-a-string
 * 
 * @returns The amount of total cores in the CPU.
 */
int get_core_count()
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
double get_cpu_usage()
{
    std::ifstream file("/proc/stat");
    std::string line;
    std::string label; // cpu label, unused

    unsigned long long user1, niced1, system1, idle1, iowait1, irq1, 
                       softirq1, steal1, guest1, guest_nice1;

    file >> label >> user1 >> niced1 >> system1 >> idle1 >> iowait1 >> irq1
                  >> softirq1 >> steal1 >> guest1 >> guest_nice1;
    file.close();

    unsigned long long total_idle1 = idle1 + iowait1;
    unsigned long long total_non_idle1 = user1 + niced1 + system1 + irq1 + softirq1 + steal1;
    unsigned long long total1 = total_idle1 + total_non_idle1;

    usleep(100000); // 100 ms

    file.open("/proc/stat");

    unsigned long long user2, niced2, system2, idle2, iowait2, irq2, 
                       softirq2, steal2, guest2, guest_nice2;

    file >> label >> user2 >> niced2 >> system2 >> idle2 >> iowait2 >> irq2
                  >> softirq2 >> steal2 >> guest2 >> guest_nice2;
    file.close();

    unsigned long long total_idle2 = idle2 + iowait2;
    unsigned long long total_non_idle2 = user2 + niced2 + system2 + irq2 + softirq2 + steal2;
    unsigned long long total2 = total_idle2 + total_non_idle2;

    unsigned long long totald = total2 - total1;
    unsigned long long idled = total_idle2 - total_idle1;

    return 100.0 * (totald - idled) / totald;
}

/**
 * @returns A vector containing CPU usage (%) per core.
 */
std::vector<double> get_cpu_usage_per_core()
{
    std::vector<double> core_percentages;
    int core_count = get_core_count();

    for (int i = 0; i < core_count; i++)
    {
        std::ifstream file("/proc/stat");
        std::string line;
        std::string label;

        double total_percent;
        
        unsigned long long user1, niced1, system1, idle1, iowait1, irq1, 
                       softirq1, steal1, guest1, guest_nice1;
        
        while (getline(file, line))
        {
            std::istringstream ss(line);
            if (line.rfind("cpu" + std::to_string(i), 0) == 0)
            {                
                ss >> label >> user1 >> niced1 >> system1 >> idle1 >> iowait1 >> irq1
                            >> softirq1 >> steal1 >> guest1 >> guest_nice1;
                break;
            }
        }

        file.close();

        unsigned long long total_idle1 = idle1 + iowait1;
        unsigned long long total_non_idle1 = user1 + niced1 + system1 + irq1 + softirq1 + steal1;
        unsigned long long total1 = total_idle1 + total_non_idle1;
                
        usleep(100000); // 100 ms

        std::ifstream file2("/proc/stat");

        unsigned long long user2, niced2, system2, idle2, iowait2, irq2, 
                        softirq2, steal2, guest2, guest_nice2;

        while (getline(file2, line))
        {
            if (line.rfind("cpu" + std::to_string(i), 0) == 0)
            {  
                std::istringstream ss(line);
                ss >> label >> user2 >> niced2 >> system2 >> idle2 >> iowait2 >> irq2
                            >> softirq2 >> steal2 >> guest2 >> guest_nice2;
                break;
            }
        }

        file2.close();

        unsigned long long total_idle2 = idle2 + iowait2;
        unsigned long long total_non_idle2 = user2 + niced2 + system2 + irq2 + softirq2 + steal2;
        unsigned long long total2 = total_idle2 + total_non_idle2;
            
        unsigned long long totald = total2 - total1;
        unsigned long long idled = total_idle2 - total_idle1;

        total_percent = 100.0 * (totald - idled) / totald;

        core_percentages.push_back(total_percent);
    }

    return core_percentages;
}