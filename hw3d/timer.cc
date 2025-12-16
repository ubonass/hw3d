#include "Timer.h"

namespace hw3d {

Timer::Timer() {
  last_ = std::chrono::steady_clock::now();
}

float Timer::Mark() {
  const auto old = last_;
  last_ = std::chrono::steady_clock::now();
  const std::chrono::duration<float> frameTime = last_ - old;
  return frameTime.count();
}

float Timer::Peek() const {
  return std::chrono::duration<float>(std::chrono::steady_clock::now() - last_)
      .count();
}

}  // namespace hw3d