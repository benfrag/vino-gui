#include "core/gui.h"
#include <iostream>

GuiCore::GuiCore() : is_running(false) {}

GuiCore::~GuiCore()
{
    shutdown();
}

void GuiCore::shutdown()
{

}

void GuiCore::init()
{
//    window_config = new_config;
//    window_manager.init(std::bind(&GuiCore::window_callback, this, std::placeholders::_1));
//    window_manager.create_window(&window_config);

    is_running = true;
}

void GuiCore::create_window(std::string label, WindowConfig window_config)
{
    if (windows.find(label) != windows.end())
        throw std::runtime_error("Window Label already exists"); // could just add identifier number to end of window and return the new label?


    windows[label] = new WindowManager();
    WindowManager* window = windows[label];
    window->init(label, window_config, std::bind(&GuiCore::window_destroy_callback, this, std::placeholders::_1));
    window->create_window();
}

void GuiCore::window_destroy_callback(std::string label)
{
    windows[label]->shutdown();
    delete windows[label];
    windows.erase(label);
}

void GuiCore::process_input()
{
    for (auto it = windows.begin(); it != windows.end(); )
    {
        it->second->process_messages();
        if (it->second->window_destroyed)
        {
            it->second->shutdown();
            WindowManager* curr = it->second;
            delete curr;
            it = windows.erase(it);
        }
        else
            it++;
    }
    
    // if no more windows destroy set is_runnig false
    // postquitmessage?
};

void GuiCore::run()
{
    last_time = std::chrono::high_resolution_clock::now();
    while (is_running)
    {
        auto current_time = std::chrono::high_resolution_clock::now();
        float delta = std::chrono::duration<float, std::chrono::seconds::period>(current_time - last_time).count();
        last_time = current_time;


        process_input();
        update(delta);
        render(delta);
    }
}

void GuiCore::render(float dt)
{
    for (const auto& [key, value] : windows)
    {
        value->render(dt);
    }
}

void GuiCore::update(float dt)
{
    for (const auto& [key, value] : windows)
    {
        value->update(dt);
    }
}
