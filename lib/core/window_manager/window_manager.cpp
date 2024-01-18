#include "core/window_manager/window_manager.h"
#include <iostream>

WindowManager::WindowManager() : hwnd(NULL), window_destroyed(false) {}

WindowManager::~WindowManager() {}

void WindowManager::init(std::string label, WindowConfig new_config, std::function<void(std::string)> window_destroy_callback)
{
    this->window_destroy_callback = window_destroy_callback;
    this->label = label;
    window_config = new_config;
}

void WindowManager::shutdown()
{
    std::cout << "shutting down " << label << std::endl;
    primitive_renderer.shutdown();
}

bool WindowManager::create_window()
{
    hInstance = GetModuleHandle(nullptr);

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowManager::_WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = window_config.class_name.c_str();
    RegisterClass(&wc);

    hwnd = CreateWindowEx(0, window_config.class_name.c_str(), window_config.window_name.c_str(), WS_OVERLAPPEDWINDOW, window_config.x, window_config.y, window_config.window_width, window_config.window_height, nullptr, nullptr, hInstance, this);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    bool init_renderer = primitive_renderer.init(hwnd, window_config);
    if (!init_renderer)
        throw std::runtime_error("primitive renderer initialization failed");

    
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
    {
        return pthis->window_proc(uMsg, wParam, lParam);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT WindowManager::window_proc(UINT umsg, WPARAM wparam, LPARAM lparam)
{
    switch (umsg)
    {
        case WM_CLOSE:
            window_destroyed = true;
//          DestroyWindow(hwnd);  // Destroys the window
            break;
        case WM_DESTROY:
            window_destroyed = true;
            // PostQuitMessage(0) if last window. query GuiCore.
            break;
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

void WindowManager::render(float dt)
{
    primitive_renderer.cycle_start();
    primitive_renderer.draw_rect(10, 10, 30, 30, {255, 0, 0, 255});
    //render primitive queue from update? or should we just render them straight?
    //so have render override called here for ecs
    primitive_renderer.cycle_end();

}

void WindowManager::update(float dt)
{

    //ecs.update_systems
}

bool WindowManager::process_messages()
{
    MSG msg;
    bool should_close = false;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        //made redundant
        if (window_destroyed)
            should_close = true;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return !should_close;
}
