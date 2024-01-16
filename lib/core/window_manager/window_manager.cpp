#include "core/window_manager/window_manager.h"
#include <iostream>

WindowManager::WindowManager() {}

WindowManager::~WindowManager() {}

void WindowManager::init(std::function<void(WindowConfig)> update_callback)
{
    this->update_callback = update_callback;
}

bool WindowManager::create_window(WindowConfig* window_config)
{
    return true;
}
