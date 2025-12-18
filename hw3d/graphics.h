
#pragma once

#include <d3d11.h>
#include <wrl.h>

#include <string>
#include <vector>

#include "dxgi_Info_manager.h"
#include "exception.h"
#include "windows_config.h"

namespace hw3d {

class Graphics {
 public:
  // Exception class for DirectX HRESULT errors
  class HrException : public Hw3dException {
   public:
    HrException(int line,
                const char* file,
                HRESULT hr,
                const std::vector<std::string>& infoMsgs = {}) noexcept;
    const char* what() const noexcept override;
    const char* GetType() const noexcept override;
    HRESULT GetErrorCode() const noexcept;
    std::string GetErrorString() const noexcept;
    std::string GetErrorDescription() const noexcept;
    std::string GetErrorInfo() const noexcept;

   private:
    HRESULT hr;
    std::string info;
  };
  // Exception class for device removed errors
  class DeviceRemovedException : public HrException {
    using HrException::HrException;

   public:
    const char* GetType() const noexcept override;
  };

 public:
  Graphics(HWND hWnd);
  Graphics(const Graphics&) = delete;
  Graphics& operator=(const Graphics&) = delete;
  ~Graphics();

  void Present();
  void ClearBuffer(float red, float green, float blue);

 private:
#ifndef NDEBUG
  DxgiInfoManager info_manager_;
#endif
  Microsoft::WRL::ComPtr<ID3D11Device> device_;
  Microsoft::WRL::ComPtr<IDXGISwapChain> swap_chain_;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext> context_;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target_;
};

// #define GFX_THROW_FAILED(hrcall)                           \
//   {                                                        \
//     HRESULT hr = (hrcall);                                 \
//     if (FAILED(hr)) {                                      \
//       throw Graphics::HrException(__LINE__, __FILE__, hr); \
//     }                                                      \
//   }

// #define GFX_DEVICE_REMOVED_EXCEPTION(hr) \
//   Graphics::DeviceRemovedException(__LINE__, __FILE__, (hr))

}  // namespace hw3d