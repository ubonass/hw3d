#pragma once

#include <memory>
#include <optional>
#include <string>

#include "exception.h"
#include "graphics.h"
#include "keyboard.h"
#include "mouse.h"
#include "windows_config.h"

namespace hw3d {

class Window {
 public:
  // Exception class for window-related errors
  class Exception : public Hw3dException {
    using Hw3dException::Hw3dException;

   public:
    static std::string TranslateErrorCode(HRESULT hr) noexcept;
  };
  // Exception class for HRESULT errors
  class HrException : public Exception {
   public:
    HrException(int line, const char* file, HRESULT hr) noexcept;
    const char* what() const noexcept override;
    const char* GetType() const noexcept override;
    HRESULT GetErrorCode() const noexcept;
    std::string GetErrorDescription() const noexcept;

   private:
    HRESULT hr;
  };

  // Exception class for no graphics device errors
  class NoGfxException : public Exception {
   public:
    using Exception::Exception;
    const char* GetType() const noexcept override;
  };

 public:
  Window(int width, int height, const char* name);
  ~Window() noexcept;
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  Keyboard& kbd() noexcept { return kbd_; }
  Mouse& mouse() noexcept { return mouse_; }
  Graphics& graphics() noexcept { return *graphics_; }

  // Sets the window title shown in the window's title bar.
  // The provided `title` string will be applied to the associated
  // HWND (platform window). Call this to update the displayed title
  // at runtime.
  void SetTitle(const std::string& title);
  // Load an icon from an external .ico file (multibyte string path) and set
  // it as the window's large and small icon. Throws `WindowException` on
  // failure.
  void SetIconFromFile(const std::string& path,
                       int width = 32,
                       int height = 32);

  void SetIconFromResource(int resourceID, int width = 32, int height = 32);

  static std::optional<int> ProcessMessages() noexcept;

 private:
  class WindowClass {
   public:
    static const char* GetName() noexcept;
    static HINSTANCE GetInstance() noexcept;

   private:
    WindowClass() noexcept;
    ~WindowClass();
    WindowClass(const WindowClass&) = delete;
    WindowClass& operator=(const WindowClass&) = delete;
    static constexpr const char* wndClassName = "Direct3D Engine Window";
    static WindowClass wndClass;
    HINSTANCE hInst;
  };

 private:
  static LRESULT CALLBACK HandleMsgSetup(HWND hWnd,
                                         UINT msg,
                                         WPARAM wParam,
                                         LPARAM lParam) noexcept;

  static LRESULT CALLBACK HandleMsgThunk(HWND hWnd,
                                         UINT msg,
                                         WPARAM wParam,
                                         LPARAM lParam) noexcept;

  LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

 private:
  Keyboard kbd_;
  Mouse mouse_;

 private:
  int width_;
  int height_;
  HWND hwnd_;
  std::unique_ptr<Graphics> graphics_;
};

// error exception helper macro
#define CHWND_EXCEPTION(hr) Window::HrException(__LINE__, __FILE__, (hr))
#define CHWND_LAST_EXCEPTION() \
  Window::HrException(__LINE__, __FILE__, GetLastError())
#define CHWND_NOGFX_EXCEPTION() Window::NoGfxException(__LINE__, __FILE__)

}  // namespace hw3d