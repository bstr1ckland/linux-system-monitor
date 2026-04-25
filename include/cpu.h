#ifndef CPU_H
#define CPU_H

#include <cstdint>

int get_core_count();
double get_cpu_usage();
double get_cpu_temperature();

std::vector<double> get_cpu_usage_per_core();

#endif // CPU_H_