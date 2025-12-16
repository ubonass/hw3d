#pragma once

#include <windows.h>
#include <string>

// Some older Windows SDKs may not define CP_UTF8; provide a fallback.
#ifndef CP_UTF8
#define CP_UTF8 65001
#endif

namespace hw3d {
// Convert a multibyte string to a wide string using the specified code page.
// Throws std::runtime_error on failure.
std::wstring MultiByteToWide(const std::string& s, UINT codePage = CP_UTF8);

}  // namespace hw3d
