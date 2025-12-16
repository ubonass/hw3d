#include "App.h"

#include <iomanip>
#include <sstream>

App::App() : wnd_(800, 600, "The Donkey Fart Box") {}

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
  const float t = timer_.Peek();
  std::ostringstream oss;
  oss << "Time elapsed: " << std::setprecision(1) << std::fixed << t << "s";
  wnd_.SetTitle(oss.str());
}