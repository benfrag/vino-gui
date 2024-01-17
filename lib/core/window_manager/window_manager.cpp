#include "core/window_manager/window_manager.h"
#include <iostream>

WindowManager::WindowManager() : hwnd(NULL) {}

WindowManager::~WindowManager() {}

void WindowManager::init(std::function<void(WindowConfig)> update_callback)
{
    this->update_callback = update_callback;
}

bool WindowManager::create_window(WindowConfig* window_config)
{
    hInstance = GetModuleHandle(nullptr);

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowManager::_WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = window_config->class_name.c_str();
    RegisterClass(&wc);

    hwnd = CreateWindowEx(0, window_config->class_name.c_str(), window_config->window_name.c_str(), WS_OVERLAPPEDWINDOW, window_config->x, window_config->y, window_config->window_width, window_config->window_height, nullptr, nullptr, hInstance, this);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    
    return true;
}

LRESULT CALLBACK WindowManager::_WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WindowManager* pthis = nullptr;

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* create = reinterpret_cast<CREATESTRUCT*>(lParam);
        pthis = static_cast<WindowManager*>(create->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pthis));
        pthis->hwnd = hwnd;
    }
    else
    {
        pthis = reinterpret_cast<WindowManager*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pthis)
        return pthis->window_proc(uMsg, wParam, lParam);

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT WindowManager::window_proc(UINT umsg, WPARAM wparam, LPARAM lparam)
{
    switch (umsg)
    {
        /*case WM_KEYDOWN:
            input_manager->set_key_down(wparam);
            break;
        case WM_KEYUP:
            input_manager->set_key_up(wparam);
            break;
        case WM_MOUSEMOVE:
            input_manager->set_mouse_position(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            break;
        case WM_LBUTTONDOWN:
            input_manager->set_mouse_button_down(MOUSE_LEFT);
            break;
        case WM_LBUTTONUP:
            input_manager->set_mouse_button_up(MOUSE_LEFT);
            break;
        case WM_RBUTTONDOWN:
            input_manager->set_mouse_button_down(MOUSE_LEFT);
            break;
        case WM_RBUTTONUP:
            input_manager->set_mouse_button_up(MOUSE_LEFT);
            break;*/
    }

    return DefWindowProc(hwnd, umsg, wparam, lparam);
}

bool WindowManager::process_messages()
{
    MSG msg;
    bool should_close = false;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            should_close = true;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return !should_close;
}
