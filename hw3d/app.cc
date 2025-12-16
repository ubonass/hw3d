#include "App.h"
#include <sstream>
#include <iomanip>

App::App() : wnd_(800, 600, "The Donkey Fart Box") {}

int App::Loop() {
  MSG msg;
  BOOL gResult;
  while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
    // TranslateMessage will post auxilliary WM_CHAR messages from key msgs
    TranslateMessage(&msg);
    DispatchMessage(&msg);

    DoFrame();
  }

  // check if GetMessage call itself borked
  if (gResult == -1) {
    throw hw3d::CHWND_LAST_EXCEPTION();
  }

  // wParam here is the value passed to PostQuitMessage
  return msg.wParam;
}

void App::DoFrame() {
  const float t = timer_.Peek();
  std::ostringstream oss;
  oss << "Time elapsed: " << std::setprecision(1) << std::fixed << t << "s";
  wnd_.SetTitle(oss.str());
}