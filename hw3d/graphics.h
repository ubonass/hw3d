
#pragma once

#include "windows_config.h"
#include <d3d11.h>

namespace hw3d {

class Graphics {
 public:
  Graphics(HWND hWnd);
  Graphics(const Graphics&) = delete;
  Graphics& operator=(const Graphics&) = delete;
  ~Graphics();

  void EndFrame();

  void ClearBuffer(float red, float green, float blue);

 private:
  ID3D11Device* device_ = nullptr;
  IDXGISwapChain* swap_chain_ = nullptr;
  ID3D11DeviceContext* context_ = nullptr;
  ID3D11RenderTargetView* target_ = nullptr;
};

}  // namespace hw3d