#include "exception.h"

#include <sstream>

namespace hw3d {

Exception::Exception(int line, const char* file) noexcept
    : line_(line), file_(file) {}

const char* Exception::what() const noexcept {
  std::ostringstream oss;
  oss << GetType() << std::endl << GetOriginString();
  what_buffer_ = oss.str();
  return what_buffer_.c_str();
}

const char* Exception::GetType() const noexcept {
  return "hw3d Exception";
}

int Exception::GetLine() const noexcept {
  return line_;
}

const std::string& Exception::GetFile() const noexcept {
  return file_;
}

std::string Exception::GetOriginString() const noexcept {
  std::ostringstream oss;
  oss << "[File] " << file_ << std::endl << "[Line] " << line_;
  return oss.str();
}

}  // namespace hw3d