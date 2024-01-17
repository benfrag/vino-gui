#include <windows.h>
#include <iostream>

#include "core/gui.h"

int main()
{
    std::cout << "in main" << std::endl;

    GuiCore gui;

    // need to change nvim keybind from enter to autofill to tab to autofill

    // need wrapper over gui ecs system, this wrapper needs to allow
    // for integration with functionality
    // so we need events etc to call events in functionality.

    try
    {
//        WindowConfig window_config;
//        gui.init(window_config);
        gui.init();
        gui.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "An exception occured: " << e.what() << std::endl;
    }

    //engine shutsdown in deconstructor.
    return 0;
};
