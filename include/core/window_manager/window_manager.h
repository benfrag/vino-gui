#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <windows.h>
#include <functional>

#include "core/renderer/renderer.h"

class WindowManager
{
public:
    WindowManager();
    ~WindowManager();

    void init(std::string label, WindowConfig new_config, std::function<void(std::string)> window_destroy_callback);

    bool create_window();
    bool process_messages();

    void render(float dt);
    void update(float dt);

    void shutdown();

    bool window_destroyed;

private:
    std::string label;
    HWND hwnd;
    HINSTANCE hInstance;

    WindowConfig window_config;
    Renderer primitive_renderer;

    static LRESULT CALLBACK _WindowProc(HWND, UINT, WPARAM, LPARAM);
    LRESULT window_proc(UINT umsg, WPARAM wparam, LPARAM lparam);
    std::function<void(std::string)> window_destroy_callback;

};

#endif
