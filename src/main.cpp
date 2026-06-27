#include <cmath>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <filesystem>
#include <map>
#include <sstream>
#include <thread>

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>

#include "cpu.h"
#include "gpu.h"
#include "utils.h"
#include "system.h"
#include "ram.h"

using namespace std;
namespace fs = std::filesystem;

// Default values for non-constant variable (to be updated)
double cpu_usage = 0.0;
double cpu_temp = 0.0;
double gpu_temp = 0.0;
double gpu_usage = 0.0;
double system_uptime = 0.0;
double used_ram = 0.0;
double vram_usage = 0.0;

std::vector<double> cpu_usage_per_core = {};

// Thread function that updates non-constant values
bool run_thread = true;

// Temperature colour setter for GPU / CPU
ftxui::Color get_temp_color(double temp, double yellow_threshold, double red_threshold)
{
    if (temp < yellow_threshold) return ftxui::Color::Green;
    if (temp < red_threshold) return ftxui::Color::Yellow;
    return ftxui::Color::Red;
}

// Used to colour the OS line in the UI
ftxui::Color get_distro_color(const std::string& distro_id)
{
    static const std::map<std::string, ftxui::Color> distro_colors = {
        {"fedora",   ftxui::Color::Blue},
        {"ubuntu",   ftxui::Color::Orange1},
        {"debian",   ftxui::Color::Red},
        {"arch",     ftxui::Color::Cyan},
        {"manjaro",  ftxui::Color::Green},
        {"opensuse", ftxui::Color::GreenLight},
        {"mint",     ftxui::Color::GreenLight},
        {"pop",      ftxui::Color::Blue},
    };

    auto it = distro_colors.find(distro_id);
    if (it != distro_colors.end())
    {
        return it->second;
    }

    return ftxui::Color::White; // default
}

// Round double
std::string format_doubles(double value)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << value;
    return oss.str();
}

// Used to fix indentation in the cpu core display section
std::string pad_core_label(int core_index, double usage)
{
    std::ostringstream oss;
    oss << "   core " << std::setw(2) << core_index << ": "
        << std::fixed << std::setprecision(2) << std::setw(6) << usage << "%";
    return oss.str();
}

void update_values(ftxui::ScreenInteractive& screen)
{
    while (run_thread)
    {
        cpu_usage = get_cpu_usage();
        cpu_temp = get_cpu_temperature();
        cpu_usage_per_core = get_cpu_usage_per_core();
        gpu_temp = get_gpu_temp();
        gpu_usage = get_gpu_usage();
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
    std::string distro_id = get_distro_id();
    std::string gpu_name = get_gpu_name();
    std::string os_info = get_os_information();
    std::string system_version = get_version();

    // Create an interactive screen instance
    auto screen = ftxui::ScreenInteractive::Fullscreen();

    // Thread to update non-constant values
    thread updater(update_values, std::ref(screen));

    // Component sub-panels, then combined into one
    auto system_info_renderer = Renderer([&] {
        int uptime_seconds = static_cast<int>(system_uptime);
        int uptime_hours = uptime_seconds / 3600;
        int uptime_minutes = uptime_seconds % 3600 / 60;

        return vbox({
            text("   kernel: " + system_version),
            text("   os: " + os_info) | color(get_distro_color(distro_id)),
            text("   uptime: " + std::to_string(uptime_hours) + "h, " + std::to_string(uptime_minutes) + "m, " + 
                                 std::to_string(uptime_seconds % 60) + "s"),
            text(""),
        });
    });

    auto system_info = Collapsible(" [ system ] ", system_info_renderer, true);


    auto cpu_info_renderer = Renderer([&] {
        int cols = 4;
        int rows = (core_count + cols - 1) / cols;

        std::vector<Element> core_rows;

        for (int row = 0; row < rows; ++row)
        {
            std::vector<Element> row_cells;

            for (int col = 0; col < cols; ++col)
            {
                int core_index = row * cols + col;
                if (core_index >= core_count) break;

                double usage = (core_index < (int)cpu_usage_per_core.size()) ? cpu_usage_per_core[core_index] : 0.0;

                row_cells.push_back(
                    text(pad_core_label(core_index, usage)) | color(Color::Yellow)
                    | flex
                );
            }

            core_rows.push_back(hbox(row_cells));
        }

        return vbox({
            text("   model: " + cpu_name + " @ " + format_doubles(max_cpu_freq) + "GHz") | color(ftxui::Color::RGB(255, 127, 0)),
            text("   usage: " + format_doubles(cpu_usage) + "%") | color(ftxui::Color::RGB(255, 127, 0)),
            text("   usage per core:") | color(Color::White),
            hbox({
                text("   "),
                gauge(cpu_usage / 100.0) | color(get_temp_color(cpu_usage, 60.0, 85.0)) | bgcolor(Color::GrayDark) | flex,
                text("   "),
            }),
            vbox(core_rows),
            text("   temperature: " + format_doubles(cpu_temp) + "°C") | color(get_temp_color(cpu_temp, 60.0, 80.0)),
            text(""),
        });
});

    auto cpu_info = Collapsible(" [ cpu ] ", cpu_info_renderer, true);


    auto gpu_info_renderer = Renderer([&] {
        return vbox({
            text("   model: " + gpu_name) | color(Color::BlueLight),
            text("   usage: " + format_doubles(gpu_usage) + "%") | color(Color::BlueLight),
            hbox({
                text("   "),
                gauge(gpu_usage / 100.0) | color(get_temp_color(gpu_usage, 70.0, 85.0)) | bgcolor(Color::GrayDark) | flex,
                text("   "),
            }),
            text("   vram used: " + format_doubles(vram_usage) + "GB / " +
                                    format_doubles(total_vram) + "GB"),
            text("   temperature: " + format_doubles(gpu_temp) + "°C") | color(get_temp_color(gpu_temp, 70.0, 85.0)),
            text(""),
        });
    });

    auto gpu_info = Collapsible(" [ gpu ] ", gpu_info_renderer, true);


    auto ram_info_renderer = Renderer([&] {
        double ram_percent = (total_ram > 0.0) ? (used_ram / total_ram) * 100.0 : 0.0;

        return vbox({
            text("   used ram: " + format_doubles(used_ram) + "GB / " + format_doubles(total_ram) + "GB") | color(Color::MagentaLight),
            hbox({
                text("   "),
                gauge(ram_percent / 100.0) | color(get_temp_color(ram_percent, 60.0, 85.0)) | bgcolor(Color::GrayDark) | flex,
                text("   "),
            }),
            text(""),
        });
    });
    
    auto ram_info = Collapsible(" [ memory ] ", ram_info_renderer, true);


    // Combine into one container
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