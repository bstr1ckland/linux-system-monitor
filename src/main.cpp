#include <cmath>
#include <iostream>
#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include "cpu.h"
#include "utils.h"
#include "system.h"

using namespace std;
namespace fs = std::filesystem;

int main()
{
    // FTXUI Documentation: https://arthursonzogni.github.io/FTXUI/
    using namespace ftxui;
 
    /** 
    Element document = hbox({
        text("left")   | border,
        text("middle") | border | flex,
        text("right")  | border,
    });
    
    Screen screen = Screen::Create(
        Dimension::Full(), 
        Dimension::Fit(document)
    );
    
    Render(screen, document);
    screen.Print();
    */

    int core_count = get_core_count();
    double cpu_usage = get_cpu_usage();
    double cpu_temp = get_cpu_temperature();

    std::vector<double> cpu_usage_per_core = get_cpu_usage_per_core();

    cout << std::round(cpu_usage * 100.0) / 100.0 << "% CPU Usage" << endl;
    
    for (int i = 0; i < core_count; i++)
    {
        cout << "cpu" + std::to_string(i) << ": " << std::round(cpu_usage_per_core[i] * 100.0) / 100.0 << "% CPU Usage" << endl;
    }

    cout << "CPU Temperature: " << cpu_temp << endl;

    cout << "System uptime (seconds): " << get_system_uptime() << endl;
    cout << "Sum of idle time per core: " << get_idle_time() << endl;

    cout << get_version() << endl;
    
    return 0;
}