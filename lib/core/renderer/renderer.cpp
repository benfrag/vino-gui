#include "core/renderer/renderer.h"

Renderer::Renderer() : device(nullptr), context(nullptr), swap_chain(nullptr), back_buffer_rtv(nullptr) {}

Renderer::~Renderer()
{

}

void Renderer::shutdown()
{
    for (auto& pair : brushCache) {
        if (pair.second) pair.second->Release();
    }

    if (d2d_render_target) d2d_render_target->Release();
    if (d2d_factory) d2d_factory->Release();

    if (back_buffer_rtv) back_buffer_rtv->Release();
    if (swap_chain) swap_chain->Release();
    if (context) context->Release();
    if (device) device->Release();
}

ID2D1SolidColorBrush* Renderer::get_or_create_brush(clr col) {
    std::tuple<int, int, int, int> colorKey = std::make_tuple(col.r, col.g, col.b, col.a);

    auto it = brushCache.find(colorKey);
    if (it != brushCache.end()) {
        // Brush already exists for this color
        return it->second;
    } else {
        // Create a new brush for this color
        ID2D1SolidColorBrush* newBrush;
        d2d_render_target->CreateSolidColorBrush(
            D2D1::ColorF(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f, col.a / 255.0f),
            &newBrush
        );
        brushCache[colorKey] = newBrush;
        return newBrush;
    }
}

bool Renderer::init(HWND hwnd, WindowConfig new_config)
{
    this->hwnd = hwnd;
    window_config = new_config;

    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // Common format for Direct2D compatibility
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.BufferCount = 2;
    scd.OutputWindow = hwnd;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, NULL, 0, D3D11_SDK_VERSION, &scd, &swap_chain, &device, NULL, &context);
    if (FAILED(hr)) {
        return false;
    }

    ID3D11Texture2D* back_buffer = nullptr;
    hr = swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
    if (FAILED(hr)) {
        return false;
    }

    hr = device->CreateRenderTargetView(back_buffer, NULL, &back_buffer_rtv);
    if (FAILED(hr)) {
        back_buffer->Release();
        return false;
    }
    back_buffer->Release(); // release after creating the render target view

    context->OMSetRenderTargets(1, &back_buffer_rtv, NULL);

    // Initialize Direct2D
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d_factory);
    if (FAILED(hr)) {
        return false;
    }

    IDXGISurface* dxgiSurface = nullptr;
    hr = swap_chain->GetBuffer(0, IID_PPV_ARGS(&dxgiSurface));
    if (FAILED(hr)) {
        return false;
    }

    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
    );

    hr = d2d_factory->CreateDxgiSurfaceRenderTarget(dxgiSurface, &props, &d2d_render_target);
    dxgiSurface->Release(); // Release DXGI surface after creating the render target
    if (FAILED(hr)) {
        return false;
    }

 // Initialize the brush
    hr = d2d_render_target->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::White), &solid_brush
    );
    if (FAILED(hr)) {
        // Handle the error appropriately
        return false;
    }

    hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&dwrite_factory)
        );
        if (FAILED(hr)) {
            return false;
        }


    return true;
}

void Renderer::update_window(WindowConfig new_config)
{

}

void Renderer::cycle_start()
{
    float clearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; // RGBA
    context->ClearRenderTargetView(back_buffer_rtv, clearColor);

    d2d_render_target->BeginDraw();
    d2d_render_target->SetTransform(D2D1::Matrix3x2F::Identity());
}

void Renderer::cycle_end()
{
    d2d_render_target->EndDraw();
    swap_chain->Present(1, 0);
}

void Renderer::draw_rect(int x, int y, int w, int h, clr col)
{
    ID2D1SolidColorBrush* brush = get_or_create_brush(col);

    // Fill the rectangle with the brush
    d2d_render_target->FillRectangle(
        D2D1::RectF(static_cast<float>(x), static_cast<float>(y), 
                    static_cast<float>(x + w), static_cast<float>(y + h)), 
        brush
    );
}
