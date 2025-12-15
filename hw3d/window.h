#pragma once
#include "Keyboard.h"
#include "exception.h"
#include "windows_config.h"

namespace hw3d {

class WindowException : public Exception {
 public:
  WindowException(int line, const char* file, HRESULT r) noexcept;

  const char* what() const noexcept override;
  const char* GetType() const noexcept;
  static std::string TranslateErrorCode(HRESULT hr) noexcept;
  HRESULT GetErrorCode() const noexcept;
  std::string GetErrorString() const noexcept;

 private:
  HRESULT hr_;
};

class Window {
 public:
  Window(int width, int height, const char* name);
  ~Window() noexcept;
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  const Keyboard& kbd() const noexcept { return kbd_; }

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

 private:
  int width_;
  int height_;
  HWND hwnd_;
};

// error exception helper macro
#define CHWND_EXCEPTION(hr) WindowException(__LINE__, __FILE__, hr)

#define CHWND_LAST_EXCEPTION() \
  WindowException(__LINE__, __FILE__, GetLastError())

}  // namespace hw3d