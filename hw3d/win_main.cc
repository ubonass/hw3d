#include <sstream>

#include "windows_config.h"
#include "windows_message_map.h"

LRESULT CALLBACK WindowProc(_In_ HWND hWnd,
                            _In_ UINT Msg,
                            _In_ WPARAM wParam,
                            _In_ LPARAM lParam) {
  static WindowsMessageMap messageMap;
  OutputDebugString(messageMap(Msg, lParam, wParam).c_str());

  switch (Msg) {
      // case WM_DESTROY:
      //   PostQuitMessage(0);
      // break;

    case WM_CLOSE:
      PostQuitMessage(69);
      break;

    case WM_KEYDOWN:
      if (wParam == 'F') {
        SetWindowText(hWnd, "F Key Pressed");
      }
      break;

    case WM_KEYUP:
      if (wParam == 'F') {
        SetWindowText(hWnd, "F Key Released");
      }
      break;

    case WM_CHAR: {
      static std::string title = "Char Pressed: ";
      title.push_back(static_cast<char>(wParam));
      SetWindowText(hWnd, title.c_str());
    } break;

    case WM_LBUTTONDOWN: {
      // SetWindowText(hWnd, "Left Mouse Button Pressed");
      const POINTS pt = MAKEPOINTS(lParam);
      std::ostringstream oss;
      oss << "（" << pt.x << "," << pt.y << ")";

      SetWindowText(hWnd, oss.str().c_str());
    }

    break;

    default:
      break;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow) {
  // MessageBox(NULL, L"Hello, World!", L"My First Windows App", MB_OK);

  const auto pClassName = "hw3d";

  // register a window class
  WNDCLASSEX wc = {0};

  wc.cbSize = sizeof(wc);
  wc.style = CS_OWNDC;
  wc.lpfnWndProc = WindowProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hIcon = nullptr;
  wc.hCursor = nullptr;
  wc.hbrBackground = nullptr;
  wc.lpszClassName = pClassName;
  wc.hIconSm = nullptr;

  RegisterClassEx(&wc);

  // create windows instance
  HWND hwnd = CreateWindowEx(
      0,                    // dwExStyle: Extended window style (0 for default)
      pClassName,           // lpClassName: Name of the window class
      "Happy Hard Window",  // lpWindowName: Window title
      WS_CAPTION | WS_MINIMIZEBOX |
          WS_SYSMENU,  // dwStyle: Window style (caption, minimize box, system
                       // menu)
      200,             // X: Initial horizontal position
      200,             // Y: Initial vertical position
      640,             // nWidth: Window width
      480,             // nHeight: Window height
      nullptr,    // hWndParent: Handle to parent window (NULL for top-level
                  // window)
      nullptr,    // hMenu: Handle to menu (NULL if no menu)
      hInstance,  // hInstance: Handle to application instance
      nullptr     // lpParam: Additional application data (usually NULL)
  );

  ShowWindow(hwnd, SW_SHOW);

  // msg loop

  MSG msg;

  BOOL gResult;

  while ((gResult = GetMessage(&msg, nullptr, 0, 0) > 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  if (gResult == -1) {
    return -1;
  } else {
    return msg.wParam;
  }
}