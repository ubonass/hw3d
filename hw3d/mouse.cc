/******************************************************************************************
 *	Chili DirectX Framework Version 16.07.20
 ** Mouse.cpp
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
#include "mouse.h"

namespace hw3d {

std::pair<int, int> Mouse::GetPos() const noexcept {
  return {x, y};
}

int Mouse::GetPosX() const noexcept {
  return x;
}

int Mouse::GetPosY() const noexcept {
  return y;
}

bool Mouse::IsInWindow() const noexcept {
  return is_in_window_;
}

bool Mouse::LeftIsPressed() const noexcept {
  return left_is_pressed_;
}

bool Mouse::RightIsPressed() const noexcept {
  return right_is_pressed_;
}

Mouse::Event Mouse::Read() noexcept {
  if (buffer_.size() > 0u) {
    Mouse::Event e = buffer_.front();
    buffer_.pop();
    return e;
  } else {
    return Mouse::Event();
  }
}

void Mouse::Flush() noexcept {
  buffer_ = std::queue<Event>();
}

void Mouse::OnMouseMove(int newx, int newy) noexcept {
  x = newx;
  y = newy;

  buffer_.push(Mouse::Event(Mouse::Event::Type::Move, *this));
  TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept {
  is_in_window_ = false;
  buffer_.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
  TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept {
  is_in_window_ = true;
  buffer_.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
  TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y) noexcept {
  left_is_pressed_ = true;

  buffer_.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
  TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept {
  left_is_pressed_ = false;

  buffer_.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
  TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept {
  right_is_pressed_ = true;

  buffer_.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
  TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept {
  right_is_pressed_ = false;

  buffer_.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
  TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept {
  buffer_.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
  TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept {
  buffer_.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
  TrimBuffer();
}

void Mouse::TrimBuffer() noexcept {
  while (buffer_.size() > bufferSize) {
    buffer_.pop();
  }
}

}  // namespace hw3d