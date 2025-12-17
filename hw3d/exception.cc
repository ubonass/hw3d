#include "exception.h"

#include <sstream>

namespace hw3d {

Hw3dException::Hw3dException(int line, const char* file) noexcept
    : line_(line), file_(file) {}

const char* Hw3dException::what() const noexcept {
  std::ostringstream oss;
  oss << GetType() << std::endl << GetOriginString();
  what_buffer_ = oss.str();
  return what_buffer_.c_str();
}

const char* Hw3dException::GetType() const noexcept {
  return "hw3d Exception";
}

int Hw3dException::GetLine() const noexcept {
  return line_;
}

const std::string& Hw3dException::GetFile() const noexcept {
  return file_;
}

std::string Hw3dException::GetOriginString() const noexcept {
  std::ostringstream oss;
  oss << "[File] " << file_ << std::endl << "[Line] " << line_;
  return oss.str();
}

}  // namespace hw3d