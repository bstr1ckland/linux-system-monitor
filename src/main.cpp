#include <cmath>
#include <iostream>
#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include "cpu.h"
#include "gpu.h"
#include "utils.h"
#include "system.h"
#include "ram.h"

using namespace std;
namespace fs = std::filesystem;

int main()
{
    using namespace ftxui;

    // CPU 
    double max_cpu_freq = get_max_cpu_freq();
    int core_count = get_core_count();
    std::string cpu_name = get_cpu_name();

    double avg_cpu_freq;
    double cpu_usage;
    double cpu_temperature;
    std::vector<double> cpu_freq_per_core;
    std::vector<double> cpu_usage_per_core;


    // GPU 
    double total_vram = get_total_vram();
    std::string gpu_name = get_gpu_name();

    double gpu_temp;
    double gpu_usage;
    double vram_usage;


    // System 
    std::string system_version = get_version();

    double system_uptime;
    double idle_time;


    // RAM 
    double total_ram = get_total_ram();

    double used_ram;


    while (true)
    {
        
    }
    
    return 0;
}