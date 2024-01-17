#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <windows.h>
#include <functional>

struct WindowConfig
{
    std::wstring window_name{L"default_window"};
    std::wstring class_name{L"default_class"};
    int width{1280};
    int height{720};
    int window_width{1280};
    int window_height{720};
    int x = CW_USEDEFAULT;
    int y = CW_USEDEFAULT;
};

class WindowManager
{
public:
    WindowManager();
    ~WindowManager();

    void init(std::function<void(WindowConfig)> update_callback);

    bool create_window(WindowConfig* window_config);
    bool process_messages();

private:
    HWND hwnd;
    HINSTANCE hInstance;

    static LRESULT CALLBACK _WindowProc(HWND, UINT, WPARAM, LPARAM);
    LRESULT window_proc(UINT umsg, WPARAM wparam, LPARAM lparam);
    std::function<void(const WindowConfig&)> update_callback;

};

#endif
