#include <fstream>
#include <map>
#include <algorithm>

using namespace std;

/**
 * Reference:
 *   https://docs.redhat.com/en/documentation/red_hat_enterprise_linux/6/html/deployment_guide/s2-proc-uptime
 * 
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
 * @returns Linux version of system.
 */
std::string get_version()
{
    std::ifstream file("/proc/version");
    std::string version;
    getline(file, version);

    size_t linux_start = version.find("Linux version ");
    size_t linux_end = version.find(" (", linux_start);
    return version.substr(linux_start, linux_end - linux_start);
}

/**
 * @returns OS information
 */
std::string get_os_information()
{
    std::ifstream file("/etc/os-release");
    std::string line;
    std::string os_info;

    while (getline(file, line))
    {
        if (line.starts_with("PRETTY_NAME"))
        {
            size_t first_quote = line.find('"');
            size_t last_quote = line.rfind('"');
            if (first_quote != std::string::npos && last_quote != std::string::npos && last_quote > first_quote)
            {
                os_info = line.substr(first_quote + 1, last_quote - first_quote - 1);
            }
            break;
        }
    }

    file.close();

    return os_info;
}

/**
 * @returns linux distro ID - used for colour in UI
 */
std::string get_distro_id()
{
    std::ifstream file("/etc/os-release");
    std::string line;

    while (std::getline(file, line))
    {
        if (line.rfind("ID=", 0) == 0)
        {
            std::string id = line.substr(3);
            // strip surrounding quotes if present, e.g. ID="fedora"
            id.erase(std::remove(id.begin(), id.end(), '"'), id.end());
            return id;
        }
    }

    return "unknown";
}