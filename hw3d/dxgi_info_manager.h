
#pragma once
#include <dxgidebug.h>
#include <wrl.h>

#include <string>
#include <vector>

#include "windows_config.h"


namespace hw3d {

class DxgiInfoManager {
 public:
  DxgiInfoManager();
  ~DxgiInfoManager();
  DxgiInfoManager(const DxgiInfoManager&) = delete;
  DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;
  void Set() noexcept;
  std::vector<std::string> GetMessages() const;

  bool IsAvailable() const noexcept { return info_queue_ != nullptr; }

 private:
  SIZE_T next_ = 0u;
  HMODULE h_dxgi_debug_ = nullptr;
  Microsoft::WRL::ComPtr<IDXGIInfoQueue> info_queue_;
};

}  // namespace hw3d