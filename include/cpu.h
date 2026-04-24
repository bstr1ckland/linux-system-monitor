#ifndef CPU_H_
#define CPU_H_

#include <cstdint>

int get_core_count();
double get_cpu_usage();
std::vector<double> get_cpu_usage_per_core();

#endif // CPU_H_