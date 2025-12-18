#include "dxgi_info_manager.h"

#include <memory>

#include "graphics.h"
#include "window.h"

#pragma comment(lib, "dxguid.lib")

namespace wrl = Microsoft::WRL;

namespace hw3d {

#define GFX_THROW_NOINFO(hrcall)                           \
  {                                                        \
    HRESULT hr;                                            \
    if (FAILED(hr = (hrcall)))                             \
      throw Graphics::HrException(__LINE__, __FILE__, hr); \
  }

DxgiInfoManager::DxgiInfoManager() {
  // define function signature of DXGIGetDebugInterface
  typedef HRESULT(WINAPI * DXGIGetDebugInterface)(REFIID, void**);

  // load the dll that contains the function DXGIGetDebugInterface
  // Try to load the dxgidebug DLL and obtain the debug interface. If not
  // available (release builds or missing Graphics Tools), treat as absent
  // and keep the manager usable as a no-op.
  h_dxgi_debug_ =
      LoadLibraryExA("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
  if (h_dxgi_debug_ == nullptr) {
    // Debug layer not present - leave info_queue_ null and return
    return;
  }

  // get address of DXGIGetDebugInterface in dll
  const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
      GetProcAddress(h_dxgi_debug_, "DXGIGetDebugInterface"));
  if (DxgiGetDebugInterface == nullptr) {
    // Unexpected: free library and continue without debug info
    FreeLibrary(h_dxgi_debug_);
    h_dxgi_debug_ = nullptr;
    return;
  }

  if (FAILED(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &info_queue_))) {
    // couldn't get the interface - clean up and continue silently
    FreeLibrary(h_dxgi_debug_);
    h_dxgi_debug_ = nullptr;
    info_queue_ = nullptr;
  }
}

DxgiInfoManager::~DxgiInfoManager() {
  if (h_dxgi_debug_ != nullptr) {
    FreeLibrary(h_dxgi_debug_);
    h_dxgi_debug_ = nullptr;
  }
}

void DxgiInfoManager::Set() noexcept {
  // set the index (next) so that the next call to GetMessages()
  // will only get errors generated after this call
  if (!info_queue_) {
    return;
  }
  next_ = info_queue_->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DxgiInfoManager::GetMessages() const {
  std::vector<std::string> messages;
  if (!info_queue_) {
    return messages;
  }
  const auto end = info_queue_->GetNumStoredMessages(DXGI_DEBUG_ALL);
  for (auto i = next_; i < end; i++) {
    HRESULT hr;
    SIZE_T messageLength = 0;
    // get the size of message i in bytes
    if (FAILED(info_queue_->GetMessage(DXGI_DEBUG_ALL, i, nullptr,
                                       &messageLength))) {
      continue;
    }
    // allocate a buffer and read the message
    std::vector<char> bytes(messageLength);
    auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.data());
    if (FAILED(info_queue_->GetMessage(DXGI_DEBUG_ALL, i, pMessage,
                                       &messageLength))) {
      continue;
    }
    if (pMessage->pDescription) {
      messages.emplace_back(pMessage->pDescription);
    }
  }
  return messages;
}

}  // namespace hw3d