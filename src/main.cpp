#include <cmath>
#include <chrono>
#include <iostream>
#include <filesystem>
#include <thread>

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/screen/screen.hpp>

#include "cpu.h"
#include "gpu.h"
#include "utils.h"
#include "system.h"
#include "ram.h"

using namespace std;
namespace fs = std::filesystem;

// Default values for non-constant variable (to be updated)
double avg_cpu_freq = 0.0;
double cpu_usage = 0.0;
double cpu_temperature = 0.0;
double gpu_temp = 0.0;
double gpu_usage = 0.0;
double idle_time = 0.0;
double system_uptime = 0.0;
double used_ram = 0.0;
double vram_usage = 0.0;

std::vector<double> cpu_freq_per_core = {};
std::vector<double> cpu_usage_per_core = {};

// Thread function that updates non-constant values
bool run_thread = true;

void update_values(ftxui::ScreenInteractive& screen)
{
    while (run_thread)
    {
        avg_cpu_freq = get_avg_cpu_freq();
        cpu_usage = get_cpu_usage();
        cpu_temperature = get_cpu_temperature();
        cpu_freq_per_core = get_cpu_freq_per_core();
        cpu_usage_per_core = get_cpu_usage_per_core();
        gpu_temp = get_gpu_temp();
        gpu_usage = get_gpu_usage();
        idle_time = get_idle_time();
        system_uptime = get_system_uptime();
        used_ram = get_used_ram();
        vram_usage = get_vram_usage();

        screen.PostEvent(ftxui::Event::Custom); // redraws UI
    }
}

int main()
{
    using namespace ftxui;

    // Constants
    int core_count = get_core_count();
    double max_cpu_freq = get_max_cpu_freq();
    double total_ram = get_total_ram();
    double total_vram = get_total_vram();

    std::string cpu_name = get_cpu_name();
    std::string gpu_name = get_gpu_name();
    std::string system_version = get_version();

    // Create an interactive screen instance
    auto screen = ftxui::ScreenInteractive::Fullscreen();

    // Thread to update non-constant values
    thread updater(update_values, std::ref(screen));

    // Component sub-panels, then combined into one
    auto system_info_renderer = Renderer([&] {
        return vbox({
            text("  version: " + system_version),
            text("  uptime: " + std::to_string(system_uptime)),
            // ...
        });
    });

    auto system_info = Collapsible(" system ", system_info_renderer, true);

    auto cpu_info_renderer = Renderer([&] {
        return vbox({
            text("CPU: " + cpu_name),
            // ...
        });
    });

    auto cpu_info = Collapsible(" cpu ", cpu_info_renderer, true);

    auto gpu_info_renderer = Renderer([&] {
        return vbox({
            text("GPU: " + gpu_name),
            // ...
        });
    });

    auto gpu_info = Collapsible(" gpu ", gpu_info_renderer, true);

    auto ram_info_renderer = Renderer([&] {
        return vbox({
            text("RAM: " + std::to_string(used_ram) + " / " + std::to_string(total_ram) + " GB"),
            // ...
        });
    });
    
    auto ram_info = Collapsible(" ram ", ram_info_renderer, true);

    // Combine into one
    auto main_container = Container::Vertical({
        system_info,
        cpu_info,
        gpu_info,
        ram_info,
    });

    auto renderer = Renderer(main_container, [&] {
        return window(
            text(" linux (amd) system monitor "),
            main_container->Render()
        );
    });

    // Loops program
    screen.Loop(renderer); 
    
    // End of program
    run_thread = false;
    updater.join();
    
    return 0;
}