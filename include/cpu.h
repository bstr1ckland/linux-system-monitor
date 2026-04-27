#ifndef CPU_H
#define CPU_H

#include <cstdint>

int get_core_count();
double get_max_cpu_freq();
double get_avg_cpu_freq();
double get_cpu_usage();
double get_cpu_temperature();

std::string get_cpu_name();
std::vector<double> get_cpu_usage_per_core();
std::vector<double> get_cpu_freq_per_core();

#endif // CPU_H_