
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

  // gain access to texture subresource in swap chain (back buffer)
  ID3D11Resource* pBackBuffer = nullptr;
  swap_chain_->GetBuffer(0, __uuidof(ID3D11Resource),
                         reinterpret_cast<void**>(&pBackBuffer));
  device_->CreateRenderTargetView(pBackBuffer, nullptr, &target_);
  pBackBuffer->Release();

  // swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D),
  //                        reinterpret_cast<void**>(&target_));
}

Graphics::~Graphics() {

  if (target_ != nullptr) {
    target_->Release();
  } 

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

void Graphics::ClearBuffer(float red, float green, float blue) {
  const float color[] = {red, green, blue, 1.0f};
  context_->ClearRenderTargetView(target_, color);
}

}  // namespace hw3d