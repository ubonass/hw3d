/******************************************************************************************
 *	Chili Direct3D Engine
 ** Copyright 2018 PlanetChili <http://www.planetchili.net>
 **
 *																						  *
 *	This file is part of Chili Direct3D Engine.
 **
 *																						  *
 *	Chili Direct3D Engine is free software: you can redistribute it and/or
 *modify		  * it under the terms of the GNU General Public License
 *as published by				  * the Free Software
 *Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 *																						  *
 *	The Chili Direct3D Engine is distributed in the hope that it will be
 *useful,		  * but WITHOUT ANY WARRANTY; without even the implied
 *warranty of						  * MERCHANTABILITY or
 *FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 *																						  *
 *	You should have received a copy of the GNU General Public License
 ** along with The Chili Direct3D Engine.  If not, see
 *<http://www.gnu.org/licenses/>.    *
 ******************************************************************************************/
#include "keyboard.h"

namespace hw3d {

bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept {
  return key_states_[keycode];
}

Keyboard::Event Keyboard::ReadKey() noexcept {
  if (key_buffer_.size() > 0u) {
    Keyboard::Event e = key_buffer_.front();
    key_buffer_.pop();
    return e;
  } else {
    return Keyboard::Event();
  }
}

bool Keyboard::KeyIsEmpty() const noexcept {
  return key_buffer_.empty();
}

char Keyboard::ReadChar() noexcept {
  if (char_buffer_.size() > 0u) {
    unsigned char charcode = char_buffer_.front();
    char_buffer_.pop();
    return charcode;
  } else {
    return 0;
  }
}

bool Keyboard::CharIsEmpty() const noexcept {
  return char_buffer_.empty();
}

void Keyboard::FlushKey() noexcept {
  key_buffer_ = std::queue<Event>();
}

void Keyboard::FlushChar() noexcept {
  char_buffer_ = std::queue<char>();
}

void Keyboard::Flush() noexcept {
  FlushKey();
  FlushChar();
}

void Keyboard::EnableAutorepeat() noexcept {
  autorepeat_enabled_ = true;
}

void Keyboard::DisableAutorepeat() noexcept {
  autorepeat_enabled_ = false;
}

bool Keyboard::AutorepeatIsEnabled() const noexcept {
  return autorepeat_enabled_;
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept {
  key_states_[keycode] = true;
  key_buffer_.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
  TrimBuffer(key_buffer_);
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept {
  key_states_[keycode] = false;
  key_buffer_.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
  TrimBuffer(key_buffer_);
}

void Keyboard::OnChar(char character) noexcept {
  char_buffer_.push(character);
  TrimBuffer(char_buffer_);
}

void Keyboard::ClearState() noexcept {
  key_states_.reset();
}

template <typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept {
  while (buffer.size() > bufferSize) {
    buffer.pop();
  }
}

}  // namespace hw3d
