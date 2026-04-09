#include <iostream>
#include "cpu.h"

using namespace std;

// FTXUI : https://arthursonzogni.github.io/FTXUI/

int main()
{
    cout << (int) get_core_count() << " total cores" << endl;
    cout << (int) get_cpu_usage() << "% total usage" << endl;

    return 0;
}