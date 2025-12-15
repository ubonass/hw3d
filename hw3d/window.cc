#include "window.h"

#include <sstream>
#include <stdexcept>

#include "resource.h"
#include "windows_message_map.h"

namespace hw3d {

// WindowException Stuff
WindowException::WindowException(int line,
                                 const char* file,
                                 HRESULT hr) noexcept
    : Exception(line, file), hr_(hr) {}

const char* WindowException::what() const noexcept {
  std::ostringstream oss;
  oss << GetType() << std::endl
      << "[Error Code] " << GetErrorCode() << std::endl
      << "[Description] " << GetErrorString() << std::endl
      << GetOriginString();
  what_buffer_ = oss.str();
  return what_buffer_.c_str();
}

const char* WindowException::GetType() const noexcept {
  return "hw3d Window Exception";
}

std::string WindowException::TranslateErrorCode(HRESULT hr) noexcept {
  char* pMsgBuf = nullptr;
  // windows will allocate memory for err string and make our pointer point to
  // it
  DWORD nMsgLen = FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
          FORMAT_MESSAGE_IGNORE_INSERTS,
      nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);
  // 0 string length returned indicates a failure
  if (nMsgLen == 0) {
    return "Unidentified error code";
  }
  std::string errorString = pMsgBuf;
  // free windows buffer
  LocalFree(pMsgBuf);
  return errorString;
}

HRESULT WindowException::GetErrorCode() const noexcept {
  return hr_;
}

std::string WindowException::GetErrorString() const noexcept {
  return TranslateErrorCode(hr_);
}

// Window Stuff
// Define the static WindowClass instance
Window::WindowClass Window::WindowClass::wndClass;

Window::Window(int width, int height, const char* name)
    : width_(width), height_(height) {
  /*
    Window construction sequence (purpose of each step):

    1) Store client area size (width/height): these represent the desired
       drawable client-area dimensions for the application logic.

    2) Compute a window rectangle `wr` in screen coordinates that includes
       window non-client areas (frame, title bar) so that after creating the
       window the client area matches the requested `width` x `height`.
       We choose an initial position (left/top) of 100,100 for a visible
       starting location — this is arbitrary and can be changed later.

    3) Call `AdjustWindowRect` with the chosen window style flags to expand
       the client-area rectangle to the full window rectangle required by the
       window manager. This ensures when we pass the computed width/height to
       `CreateWindowEx`, the resulting client area will be `width` x
       `height`.

    4) Create the actual Win32 window with `CreateWindowEx`:
       - `dwExStyle` (first arg): extended window styles (0 for default).
       - `lpClassName`: the registered window class name returned by
         `WindowClass::GetName()`.
       - `lpWindowName`: the window title text passed in `name`.
       - `dwStyle`: window style flags (caption, minimize box, system menu).
       - `x`,`y`,`nWidth`,`nHeight`: position and size. We use
         `CW_USEDEFAULT` for an OS-chosen position and the adjusted
         `wr.right-wr.left` / `wr.bottom-wr.top` for size so the client area
         matches the requested dimensions.
       - `hWndParent`: `nullptr` for a top-level window.
       - `hMenu`: `nullptr` (no menu).
       - `hInstance`: application instance obtained from
         `WindowClass::GetInstance()`.
       - `lpParam`: we pass `this` so the pointer to this `Window` instance
         is available in `WM_NCCREATE`/`WM_CREATE` via the `CREATESTRUCT`.

    5) The `this` pointer passed as `lpParam` is used in the window setup
       procedure (`HandleMsgSetup`) to associate the `HWND` with this C++
       object (stored in `GWLP_USERDATA`) and switch the window procedure to
       the instance message thunk. That binding allows subsequent messages to
       be forwarded to the `Window` instance methods.
  */

  RECT wr;
  wr.left = 100;
  wr.right = wr.left + width;
  wr.top = 100;
  wr.bottom = wr.top + height;

  // Expand the rectangle to include non-client area (borders, title bar)
  if ((AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
                        FALSE)) == 0) {
    throw CHWND_LAST_EXCEPTION();
  }

  // Create the Win32 window. `this` is passed as the creation parameter so
  // the setup WndProc can bind the HWND to this instance.
  hwnd_ = CreateWindowEx(0,                       // Optional window styles.
                         WindowClass::GetName(),  // Registered class name
                         name,                    // Window title
                         WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
                         // Window style

                         // Position (use OS default) and adjusted size
                         CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left,
                         wr.bottom - wr.top,

                         nullptr,                     // Parent window
                         nullptr,                     // Menu
                         WindowClass::GetInstance(),  // Instance handle
                         this  // lpParam -> pointer to this Window instance
  );

  // check for error
  if (hwnd_ == nullptr) {
    throw CHWND_LAST_EXCEPTION();
  }

  ShowWindow(hwnd_, SW_SHOWDEFAULT);
}

Window::~Window() noexcept {
  DestroyWindow(hwnd_);
}

void Window::SetTitle(const std::string& title) {
  if (SetWindowText(hwnd_, title.c_str()) == 0) {
    throw CHWND_LAST_EXCEPTION();
  }
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd,
                                        UINT msg,
                                        WPARAM wParam,
                                        LPARAM lParam) noexcept {
  /*
    Setup window procedure called very early during CreateWindowEx.

    Purpose: bind the freshly created HWND to the C++ `Window` instance
    passed in as the `lpParam` argument to `CreateWindowEx`. This allows
    subsequent window messages to be forwarded to instance methods rather
    than handled by a global/static WndProc.

    Sequence and rationale:
    - We look for `WM_NCCREATE` because it is sent before non-client area
      creation finishes and before most other messages. The `lParam`
      contains a pointer to a `CREATESTRUCT` whose `lpCreateParams` is the
      `lpParam` value that was passed to `CreateWindowEx` (we passed `this`).
    - Extract the pointer to the `Window` instance from `CREATESTRUCT`.
    - Store that pointer in the window's user data area via
      `SetWindowLongPtr(..., GWLP_USERDATA, ...)`. `GWLP_USERDATA` is a
      convenient per-HWND slot used to associate arbitrary data (like a
      `this` pointer) with the HWND.
    - Replace the window's WndProc with the instance thunk (`HandleMsgThunk`)
      by calling `SetWindowLongPtr(..., GWLP_WNDPROC, ...)`. After this
      replacement, future messages go to `HandleMsgThunk`, which reads
      `GWLP_USERDATA` and forwards to the instance `HandleMsg` member.
    - Finally, forward the current message to the instance handler so the
      instance can process any creation-time messages.

    Notes:
    - Returning zero from `WM_NCCREATE` will cancel window creation; if you
      decide creation failed, return `FALSE` to abort. In our case we
      forward to the instance and return its result.
    - If a message arrives before `WM_NCCREATE` (very uncommon for normal
      CreateWindowEx usage), we fall back to `DefWindowProc`.
  */
  if (msg == WM_NCCREATE) {
    const CREATESTRUCTW* const pCreate =
        reinterpret_cast<CREATESTRUCTW*>(lParam);
    Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

    // store pointer to C++ object in the HWND so other procs can retrieve it
    SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

    // switch to the normal thunk WndProc which will forward messages to the
    // instance stored in GWLP_USERDATA
    SetWindowLongPtr(hWnd, GWLP_WNDPROC,
                     reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

    // forward the create message to the instance for any per-instance handling
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
  }

  // fallback for messages arriving before WM_NCCREATE
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd,
                                        UINT msg,
                                        WPARAM wParam,
                                        LPARAM lParam) noexcept {
  // retrieve ptr to window instance
  Window* const pWnd =
      reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
  // forward message to window instance handler
  return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd,
                          UINT msg,
                          WPARAM wParam,
                          LPARAM lParam) noexcept {
  // static WindowsMessageMap messageMap;
  // OutputDebugString(messageMap(msg, lParam, wParam).c_str());

  switch (msg) {
    // we don't want the DefProc to handle this message because
    // we want our destructor to destroy the window, so return 0 instead of
    // break
    case WM_CLOSE:
      PostQuitMessage(0);
      return 0;

    // clear keystate when window loses focus to prevent input getting "stuck"
    case WM_KILLFOCUS:
      kbd_.ClearState();
      break;

    /*********** KEYBOARD MESSAGES ***********/
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
      // Only handle the key press event the first time a key is pressed,
      // preventing repeated triggers when holding down the key, unless
      // auto-repeat is enabled.
      if (!(lParam & 0x40000000) ||
          kbd_.AutorepeatIsEnabled()) {  // filter autorepeat
        kbd_.OnKeyPressed(static_cast<unsigned char>(wParam));
      }
      break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
      kbd_.OnKeyReleased(static_cast<unsigned char>(wParam));
      break;
    case WM_CHAR:
      kbd_.OnChar(static_cast<unsigned char>(wParam));
      break;
      /*********** END KEYBOARD MESSAGES ***********/

      /************* MOUSE MESSAGES ****************/
    case WM_MOUSEMOVE: {
      POINTS pt = MAKEPOINTS(lParam);
      mouse_.OnMouseMove(pt.x, pt.y);
    }
    case WM_LBUTTONDOWN: {
      const POINTS pt = MAKEPOINTS(lParam);
      mouse_.OnLeftPressed(pt.x, pt.y);
      break;
    }
    case WM_RBUTTONDOWN: {
      const POINTS pt = MAKEPOINTS(lParam);
      mouse_.OnRightPressed(pt.x, pt.y);
      break;
    }
    case WM_LBUTTONUP: {
      const POINTS pt = MAKEPOINTS(lParam);
      mouse_.OnLeftReleased(pt.x, pt.y);
      break;
    }
    case WM_RBUTTONUP: {
      const POINTS pt = MAKEPOINTS(lParam);
      mouse_.OnRightReleased(pt.x, pt.y);
      break;
    }
    case WM_MOUSEWHEEL: {
      const POINTS pt = MAKEPOINTS(lParam);
      if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
        mouse_.OnWheelUp(pt.x, pt.y);
      } else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
        mouse_.OnWheelDown(pt.x, pt.y);
      }
      break;
    }
      /************** END MOUSE MESSAGES **************/
  }

  /************** END RAW MOUSE MESSAGES **************/
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr)) {
  WNDCLASSEX wc = {};
  wc.cbSize = sizeof(wc);
  wc.style = CS_OWNDC;
  wc.lpfnWndProc = Window::HandleMsgSetup;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = GetInstance();
  wc.hIcon = static_cast<HICON>(LoadImage(
      GetInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
  wc.hCursor = nullptr;
  wc.hbrBackground = nullptr;
  wc.lpszClassName = GetName();
  wc.hIconSm = static_cast<HICON>(LoadImage(
      GetInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));

  RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() {
  UnregisterClass(wndClassName, hInst);
}

const char* Window::WindowClass::GetName() noexcept {
  return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept {
  return wndClass.hInst;
}
}  // namespace hw3d
