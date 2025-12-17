#pragma once

#include <exception>
#include <string>

namespace hw3d {

class Hw3dException : public std::exception {
 public:
  Hw3dException(int line, const char* file) noexcept;

  virtual const char* what() const noexcept override;

  virtual const char* GetType() const noexcept;

  int GetLine() const noexcept;

  const std::string& GetFile() const noexcept;

  std::string GetOriginString() const noexcept;

 protected:
  mutable std::string what_buffer_;

 private:
  int line_;
  std::string file_;
};

}  // namespace hw3d