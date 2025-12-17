
#include "graphics.h"

#pragma comment(lib, "d3d11.lib")

namespace hw3d {

Graphics::Graphics(HWND hwnd) {
  DXGI_SWAP_CHAIN_DESC sd = {};
  sd.BufferDesc.Width = 0;
  sd.BufferDesc.Height = 0;
  sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 0;
  sd.BufferDesc.RefreshRate.Denominator = 0;
  sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.BufferCount = 1;
  sd.OutputWindow = hwnd;
  sd.Windowed = TRUE;
  sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  sd.Flags = 0;

  // create device and front/back buffers, and swap chain and rendering context
  D3D11CreateDeviceAndSwapChain(nullptr,                   //
                                D3D_DRIVER_TYPE_HARDWARE,  //
                                nullptr,                   //
                                0,                         //
                                nullptr,                   //
                                0,                         //
                                D3D11_SDK_VERSION,         //
                                &sd,                       //
                                &swap_chain_,              //
                                &device_,                  //
                                nullptr,                   //
                                &context_);
}

Graphics::~Graphics() {
  if (context_ != nullptr) {
    context_->Release();
  }
  if (swap_chain_ != nullptr) {
    swap_chain_->Release();
  }
  if (device_ != nullptr) {
    device_->Release();
  }
}

void Graphics::EndFrame() {
  // wait for vertical blanking interval before presenting
  swap_chain_->Present(1u, 0u);
}

}  // namespace hw3d