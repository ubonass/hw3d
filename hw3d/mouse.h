/******************************************************************************************
 *	Chili DirectX Framework Version 16.07.20
 ** Mouse.h
 ** Copyright 2016 PlanetChili <http://www.planetchili.net>
 **
 *																						  *
 *	This file is part of The Chili DirectX Framework.
 **
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it
 *and/or modify	  * it under the terms of the GNU General Public License as
 *published by				  * the Free Software Foundation, either
 *version 3 of the License, or					  * (at your
 *option) any later version.
 **
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be
 *useful,		  * but WITHOUT ANY WARRANTY; without even the implied
 *warranty of						  * MERCHANTABILITY or
 *FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 *																						  *
 *	You should have received a copy of the GNU General Public License
 ** along with The Chili DirectX Framework.  If not, see
 *<http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once
#include <queue>
namespace hw3d {

class Mouse {
  friend class Window;

 public:
  class Event {
   public:
    enum class Type {
      LPress,
      LRelease,
      RPress,
      RRelease,
      WheelUp,
      WheelDown,
      Move,
      Enter,
      Leave,
      Invalid
    };

   private:
    Type type;
    bool left_is_pressed_;
    bool right_is_pressed_;
    int x;
    int y;

   public:
    Event() noexcept
        : type(Type::Invalid),
          left_is_pressed_(false),
          right_is_pressed_(false),
          x(0),
          y(0) {}
    Event(Type type, const Mouse& parent) noexcept
        : type(type),
          left_is_pressed_(parent.left_is_pressed_),
          right_is_pressed_(parent.right_is_pressed_),
          x(parent.x),
          y(parent.y) {}
    bool IsValid() const noexcept { return type != Type::Invalid; }
    Type GetType() const noexcept { return type; }
    std::pair<int, int> GetPos() const noexcept { return {x, y}; }
    int GetPosX() const noexcept { return x; }
    int GetPosY() const noexcept { return y; }
    bool LeftIsPressed() const noexcept { return left_is_pressed_; }
    bool RightIsPressed() const noexcept { return right_is_pressed_; }
  };

 public:
  Mouse() = default;
  Mouse(const Mouse&) = delete;
  Mouse& operator=(const Mouse&) = delete;
  std::pair<int, int> GetPos() const noexcept;
  int GetPosX() const noexcept;
  int GetPosY() const noexcept;
  bool IsInWindow() const noexcept;
  bool LeftIsPressed() const noexcept;
  bool RightIsPressed() const noexcept;
  Mouse::Event Read() noexcept;
  bool IsEmpty() const noexcept { return buffer_.empty(); }
  void Flush() noexcept;

 private:
  void OnMouseMove(int x, int y) noexcept;
  void OnMouseLeave() noexcept;
  void OnMouseEnter() noexcept;
  void OnLeftPressed(int x, int y) noexcept;
  void OnLeftReleased(int x, int y) noexcept;
  void OnRightPressed(int x, int y) noexcept;
  void OnRightReleased(int x, int y) noexcept;
  void OnWheelUp(int x, int y) noexcept;
  void OnWheelDown(int x, int y) noexcept;
  void TrimBuffer() noexcept;
  void OnWheelDelta(int x, int y, int delta) noexcept;

 private:
  static constexpr unsigned int bufferSize = 16u;
  int x;
  int y;
  bool left_is_pressed_ = false;
  bool right_is_pressed_ = false;
  bool is_in_window_ = false;
  int wheel_delta_carry_ = 0;
  std::queue<Event> buffer_;
};
}  // namespace hw3d