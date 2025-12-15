#include <sstream>

#include "window.h"
#include "windows_config.h"
#include "windows_message_map.h"

LRESULT CALLBACK WindowProc(_In_ HWND hWnd,
                            _In_ UINT Msg,
                            _In_ WPARAM wParam,
                            _In_ LPARAM lParam) {
  static hw3d::WindowsMessageMap messageMap;
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

  try {
    hw3d::Window window(640, 480, "Donkey Fart Box");
    MSG msg;
    BOOL gResult;
    while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
      // TranslateMessage will post auxilliary WM_CHAR messages from key msgs
      TranslateMessage(&msg);
      DispatchMessage(&msg);

      if (window.kbd().KeyIsPressed(VK_SPACE)) {
        MessageBox(nullptr, "Something Happon!", "Space Key Was Pressed",
                   MB_OK | MB_ICONEXCLAMATION);
      }
    }

    // check if GetMessage call itself borked
    if (gResult == -1) {
      return -1;
    }

    // wParam here is the value passed to PostQuitMessage
    return msg.wParam;
  } catch (const hw3d::Exception& e) {
    MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
  } catch (const std::exception& e) {
    MessageBox(nullptr, e.what(), "Standard Exception",
               MB_OK | MB_ICONEXCLAMATION);
  } catch (...) {
    MessageBox(nullptr, "No details available", "Unknown Exception",
               MB_OK | MB_ICONEXCLAMATION);
  }
  return -1;
}