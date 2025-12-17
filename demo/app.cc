#include "App.h"

#include <iomanip>
#include <sstream>
// use embedded resource id
#include "resource.h"

App::App() : wnd_(800, 600, "The Donkey Fart Box") {
  wnd_.SetIconFromResource(IDI_HW3D);
}

int App::Loop() {
  while (true) {
    // process all messages pending, but to not block for new messages
    if (const auto ecode = hw3d::Window::ProcessMessages()) {
      // if return optional has value, means we're quitting so return exit
      // code
      return *ecode;
    }
    DoFrame();
  }
}

void App::DoFrame() {
  // const float t = timer_.Peek();
  // std::ostringstream oss;
  // oss << "Time elapsed: " << std::setprecision(1) << std::fixed << t << "s";
  // wnd_.SetTitle(oss.str());

  const float c = sin(timer_.Peek()) / 2.0f + 0.5f;
  wnd_.graphics().ClearBuffer(c, c, 1.0f);
  wnd_.graphics().EndFrame();
}