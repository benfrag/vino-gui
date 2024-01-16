#ifndef GUI_H
#define GUI_H

#include <windows.h>
#include <chrono>
#include "core/window_manager/window_manager.h"

class GuiCore
{
public:
    GuiCore();
    ~GuiCore();

    void init(WindowConfig new_config);
    void run();
    void shutdown();
    
    void window_callback(WindowConfig new_config);

private:
    WindowConfig window_config;
    WindowManager window_manager;
    bool is_running;
    std::chrono::high_resolution_clock::time_point last_time;

    void render(float dt);
    void update(float dt);
    void process_input();
};

#endif
