#pragma once

#include <optional>

#include "exception.h"
#include "keyboard.h"
#include "mouse.h"
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

  Keyboard& kbd() noexcept { return kbd_; }
  Mouse& mouse() noexcept { return mouse_; }

  // Sets the window title shown in the window's title bar.
  // The provided `title` string will be applied to the associated
  // HWND (platform window). Call this to update the displayed title
  // at runtime.
  void SetTitle(const std::string& title);

  static std::optional<int> ProcessMessages();
  
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
};

// error exception helper macro
#define CHWND_EXCEPTION(hr) WindowException(__LINE__, __FILE__, hr)

#define CHWND_LAST_EXCEPTION() \
  WindowException(__LINE__, __FILE__, GetLastError())

}  // namespace hw3d