#pragma once
#include "Window.h"
#include "Timer.h"
class App {
 public:
  App();
  // master frame / message loop
  int Loop();

 private:
  void DoFrame();

 private:
  hw3d::Window wnd_;
  hw3d::Timer timer_;
};