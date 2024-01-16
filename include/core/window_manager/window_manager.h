#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <functional>

struct WindowConfig
{
    int x;
};

class WindowManager
{
public:
    WindowManager();
    ~WindowManager();

    void init(std::function<void(WindowConfig)> update_callback);
    bool create_window(WindowConfig* window_config);

private:
    std::function<void(const WindowConfig&)> update_callback;

};

#endif
