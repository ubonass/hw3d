
#pragma once
#include <chrono>

namespace hw3d {
// Timer class for measuring elapsed time intervals.
class Timer {
 public:
  // Constructor. Initializes the timer and sets the starting point.
  Timer();

  // Returns the time elapsed (in seconds) since the last call to Mark().
  // Also resets the starting point to now.
  float Mark();

  // Returns the time elapsed (in seconds) since the last call to Mark() or
  // since construction, but does not reset the starting point.
  float Peek() const;

 private:
  // Stores the last time point when Mark() was called or when the timer was
  // constructed.
  std::chrono::steady_clock::time_point last_;
};

}  // namespace hw3d