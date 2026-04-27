#include <cmath>
#include <iostream>
#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include "cpu.h"
#include "gpu.h"
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

    cout << "Avg clock speed: " << get_avg_cpu_freq() << endl;
    
    return 0;
}