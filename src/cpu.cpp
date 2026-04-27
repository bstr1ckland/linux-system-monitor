#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <vector>
#include "cpu.h"
#include "utils.h"

using namespace std;
namespace fs = std::filesystem;

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
 * @returns Name of the CPU model.
 */
std::string get_cpu_name()
{
    std::ifstream file("/proc/cpuinfo");
    std::string line;

    // grabs first instance of the model name in cpuinfo.
    // it appears core_count amount of times in that file
    while (getline(file, line))
    {
        if (line.find("model name") != std::string::npos)
        {
            int pos = line.find(":");
            std::string model_name = line.substr(pos + 2);
            return model_name;
        }
    }

    file.close();

    return "";
}

/**
 * @returns Max clock speed/frequency for all CPU cores (GHz).
 */
double get_max_cpu_freq()
{
    // all CPU cores have same max freq, grabs first instance (any instance works)
    std::ifstream file("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
    double max_freq;

    file >> max_freq;
    file.close();

    max_freq /= 1000000.0;
    return std::round(max_freq * 100.0) / 100.0; // convert to GHz
}

/**
 * @returns Average clock speed/frequency of all CPU cores (GHz).
 */
double get_avg_cpu_freq()
{
    std::vector<double> cpu_freqs = get_cpu_freq_per_core();
    double sum = 0;

    for (int i = 0; i < get_core_count(); i++)
    {
        sum += cpu_freqs[i];
    }

    sum /= get_core_count();

    return std::round(sum * 100.0) / 100.0; // convert to GHz
}

/**
 * Reference:
 *   https://askubuntu.com/questions/218567/any-way-to-check-the-clock-speed-of-my-processor
 * 
 * @returns Vector of CPU clock speed/frequency per core (GHz).
 */
std::vector<double> get_cpu_freq_per_core()
{
    std::vector<double> cpu_freqs;
    std::ifstream file("/proc/cpuinfo");
    std::string line;

    for (int i = 0; i < get_core_count(); i++)
    {
        while (getline(file, line))
        {
            if (line.find("cpu MHz") != std::string::npos)
            {
                size_t value = line.find_first_of("0123456789");
                double freq = std::stod(line.substr(value));

                freq /= 1000.0;
                freq = std::round(freq * 100.0) / 100.0;
                cpu_freqs.push_back(freq);
            }
        }
    }

    return cpu_freqs;
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

/**
 * Reference:
 *   https://medium.com/@balemarthyvamsi/part-1-introduction-to-the-hwmon-subsystem-8000dbed11c4
 * 
 * @returns Temperature of CPU in degrees (C).
 */
double get_cpu_temperature()
{
    fs::path path = hwmon_iter("k10temp");
    std::ifstream file(path.string() + "/temp1_input");
    double temp;

    file >> temp;

    file.close();

    return temp / 1000.0;
}