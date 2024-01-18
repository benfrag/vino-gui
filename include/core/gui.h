#ifndef GUI_H
#define GUI_H

#include <windows.h>
#include <chrono>
#include <string>
#include <unordered_map>
#include "core/window_manager/window_manager.h"

class GuiCore
{
public:
    GuiCore();
    ~GuiCore();

    void init();
    void run();
    void shutdown();
    void create_window(std::string label, WindowConfig window_config);

    void window_destroy_callback(std::string label);
    
//    void window_callback(WindowConfig new_config);

private:
    std::unordered_map<std::string, WindowManager*> windows;
    //WindowConfig window_config;
    //WindowManager window_manager;
    bool is_running;
    std::chrono::high_resolution_clock::time_point last_time;

    void render(float dt);
    void update(float dt);
    void process_input();
};

#endif
