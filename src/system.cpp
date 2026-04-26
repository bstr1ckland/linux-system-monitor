#include <fstream>

using namespace std;

/**
 * Reference:
 *   https://docs.redhat.com/en/documentation/red_hat_enterprise_linux/6/html/deployment_guide/s2-proc-uptime
 * @returns uptime of system in seconds.
 */
double get_system_uptime()
{
    double uptime;
    std::ifstream file("/proc/uptime");
    
    // Grabs first instance, don't need second number here
    file >> uptime;
    file.close();

    return uptime;
}

/**
 * @returns sum of idle time for each core.
 */
double get_idle_time()
{
    double uptime;
    double idle_time;
    std::ifstream file("/proc/uptime");

    file >> uptime >> idle_time;
    file.close();

    return idle_time;
}

/**
 * TODO: FIX UP
 * 
 * @returns Linux version of system.
 */
std::string get_version()
{
    std::ifstream file("/proc/version");
    std::string version;

    getline(file, version);

    return version;
}