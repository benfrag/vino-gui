#ifndef RENDERER_H
#define RENDERER_H


#include <unordered_map>
#include <map>
#include <tuple>

#include <d3d11.h>
#include <dxgi.h>
#include <D3Dcompiler.h>

#include <d2d1.h>
#include <dwrite.h>

#include <string>

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

struct clr
{
    int r, g, b, a;
};

struct TupleHash {
    size_t operator()(const std::tuple<int, int, int, int>& key) const {
        auto [r, g, b, a] = key;
        return std::hash<int>()(r) ^ std::hash<int>()(g) ^ std::hash<int>()(b) ^ std::hash<int>()(a);
    }
};



class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool init(HWND hwnd, WindowConfig new_config);
    void update_window(WindowConfig new_config);
    void shutdown();
    
    void cycle_start();
    void cycle_end();

    void draw_rect(int x, int y, int w, int h, clr col);

private:
    HWND hwnd;
    WindowConfig window_config; // filled by data from window_manager, but not a ref as we have to update when it changes.
    
    ID3D11Device* device;
    ID3D11DeviceContext* context;
    IDXGISwapChain* swap_chain;
    ID3D11RenderTargetView* back_buffer_rtv;

    ID2D1Factory* d2d_factory;
    ID2D1RenderTarget* d2d_render_target;

    IDWriteFactory* dwrite_factory; // Add a DirectWrite factory member
    std::map<std::wstring, IDWriteTextFormat*> text_format_cache;

    ID2D1SolidColorBrush* solid_brush;
    std::unordered_map<std::tuple<int, int, int, int>, ID2D1SolidColorBrush*, TupleHash> brushCache;

    ID2D1SolidColorBrush* get_or_create_brush(clr col);


};

#endif
