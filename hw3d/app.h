#pragma once
#include "Window.h"

class App {
 public:
  App();
  // master frame / message loop
  int Loop();

 private:
  void DoFrame();

 private:
  hw3d::Window wnd;
};