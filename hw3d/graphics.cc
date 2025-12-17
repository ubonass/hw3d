
#include "graphics.h"

#include <sstream>

#include "dxerr.h"

#pragma comment(lib, "d3d11.lib")

namespace hw3d {

// Graphics exception stuff
Graphics::HrException::HrException(int line,
                                   const char* file,
                                   HRESULT hr) noexcept
    : Hw3dException(line, file), hr(hr) {}

const char* Graphics::HrException::what() const noexcept {
  std::ostringstream oss;
  oss << GetType() << std::endl
      << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
      << std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
      << "[Error String] " << GetErrorString() << std::endl
      << "[Description] " << GetErrorDescription() << std::endl
      << GetOriginString();
  what_buffer_ = oss.str();
  return what_buffer_.c_str();
}

const char* Graphics::HrException::GetType() const noexcept {
  return "hw3d Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept {
  return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept {
  return DXGetErrorString(hr);
}

std::string Graphics::HrException::GetErrorDescription() const noexcept {
  char buf[512];
  DXGetErrorDescription(hr, buf, sizeof(buf));
  return buf;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept {
  return "hw3d Graphics Exception [Device Removed] "
         "(DXGI_ERROR_DEVICE_REMOVED)";
}

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

  // for checking results of d3d functions
  // HRESULT hr;
  // create device and front/back buffers, and swap chain and rendering context
  GFX_THROW_FAILED(D3D11CreateDeviceAndSwapChain(nullptr,                   //
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
                                                 &context_));

  // gain access to texture subresource in swap chain (back buffer)
  ID3D11Resource* pBackBuffer = nullptr;
  GFX_THROW_FAILED(swap_chain_->GetBuffer(
      0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer)));

  GFX_THROW_FAILED(
      device_->CreateRenderTargetView(pBackBuffer, nullptr, &target_));

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

void Graphics::Present() {
  // wait for vertical blanking interval before presenting
  HRESULT hr = swap_chain_->Present(1u, 0u);
  if (!FAILED(hr)) {
    return;
  }

  if (hr == DXGI_ERROR_DEVICE_REMOVED) {
    // Device was removed; throw a specific exception with the removal reason
    throw Graphics::DeviceRemovedException(__LINE__, __FILE__,
                                           device_->GetDeviceRemovedReason());
  }
  // For other failures, throw a generic HRESULT exception
  throw Graphics::HrException(__LINE__, __FILE__, hr);
}

void Graphics::ClearBuffer(float red, float green, float blue) {
  const float color[] = {red, green, blue, 1.0f};
  context_->ClearRenderTargetView(target_, color);
}

}  // namespace hw3d