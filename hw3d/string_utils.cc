#include "string_utils.h"

#include <stdexcept>
#include <vector>

namespace hw3d {

std::wstring MultiByteToWide(const std::string& s, UINT codePage) {
  if (s.empty())
    return std::wstring();

  int required = ::MultiByteToWideChar(codePage, 0, s.c_str(), -1, nullptr, 0);
  if (required == 0) {
    throw std::runtime_error("MultiByteToWideChar failed: " +
                             std::to_string(::GetLastError()));
  }

  std::wstring out;
  out.resize(required);
  int res =
      ::MultiByteToWideChar(codePage, 0, s.c_str(), -1, &out[0], required);
  if (res == 0) {
    throw std::runtime_error("MultiByteToWideChar failed: " +
                             std::to_string(::GetLastError()));
  }
  // remove terminating null added by API
  if (!out.empty() && out.back() == L'\0')
    out.pop_back();
  return out;
}

}  // namespace hw3d
