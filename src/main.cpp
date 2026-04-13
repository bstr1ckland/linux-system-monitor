#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include "cpu.h"

using namespace std;

int main()
{
    // FTXUI Documentation: https://arthursonzogni.github.io/FTXUI/
    using namespace ftxui;
 
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

    cout << get_cpu_usage() << endl;

    return 0;
}