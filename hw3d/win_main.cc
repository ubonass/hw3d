#include <sstream>
#include "app.h"

int CALLBACK WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow) {
  // MessageBox(NULL, L"Hello, World!", L"My First Windows App", MB_OK);

  try {
    return App{}.Loop();
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