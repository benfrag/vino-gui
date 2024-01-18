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
        WindowConfig main_window;
        gui.create_window("main", main_window);
        WindowConfig secondary_window;
        secondary_window.window_width = 400;
        secondary_window.window_height = 700;
        gui.create_window("secondary", secondary_window);
        WindowConfig third_window;
        gui.create_window("third", third_window);

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
