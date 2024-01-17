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

void GuiCore::init(WindowConfig new_config)
{
    window_config = new_config;
    window_manager.init(std::bind(&GuiCore::window_callback, this, std::placeholders::_1));
    window_manager.create_window(&window_config);


    is_running = true;
}

void GuiCore::window_callback(WindowConfig new_config)
{
    //update window config renderer etc
    //should we scale up when window size changes? or should we occupy the new space

}

void GuiCore::process_input()
{
    window_manager.process_messages();
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

}

void GuiCore::update(float dt)
{

}
