/*
 * OpenMPTL - C++ Microprocessor Template Library
 *
 * Copyright 2013 Axel Burri <axel@tty0.ch>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef REGISTER_MANIP_HPP_INCLUDED
#define REGISTER_MANIP_HPP_INCLUDED

#include <compiler.h>

template<typename T>
class RegisterManip
{
public:
  using value_type = typename T::value_type;

  RegisterManip() : reg(T::load()) { }
  RegisterManip(value_type val) : reg(val) { }

  RegisterManip<T> operator|(const T & rhs) { return RegisterManip<T>(reg | rhs); }

  RegisterManip<T> & operator=(const RegisterManip<T> & rhs) { return *this; }
  RegisterManip<T> & operator|=(value_type rhs) { this->reg |= rhs; return *this; }
  RegisterManip<T> & operator&=(value_type rhs) { this->reg &= rhs; return *this; }

  constexpr operator value_type() { return reg; }

  void __always_inline load(void) { reg = T::load(); }
  void __always_inline store(void) const { T::store(reg); }

  void __always_inline set(value_type const set_mask) { reg |= set_mask; }
  void __always_inline set(value_type const set_mask, value_type const clear_mask) { reg = (reg & ~clear_mask) | set_mask; }
  void __always_inline clear(value_type const value) { reg &= ~value; }

  template<typename... Rm>
  void __always_inline clear(void) {
    clear(T::template combined_mask<Rm...>::type::clear_mask);
  }

  template<typename... Rm>
  void __always_inline set(void) {
    using combined_type = typename T::template combined_mask<Rm...>::type;
    set( combined_type::set_mask, combined_type::cropped_clear_mask );
  }

private:
  value_type reg;
};

#endif // REGISTER_MANIP_HPP_INCLUDED
