/*
 * CppCore - C++ microprocessor core library
 *
 * Copyright 2012 Axel Burri <axel@tty0.ch>
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

#ifndef COMMON_ARM_CORTEX_SCB_HPP_INCLUDED
#define COMMON_ARM_CORTEX_SCB_HPP_INCLUDED

#include "reg/scb.hpp"

template<unsigned priority_bits>
class Scb
{
  using SCB = reg::SCB;

public:

  static void set_priority_group(uint32_t group) {
    //TODO: assert(group == (group & 0x07));

    auto reg  =  SCB::AIRCR::load();
    reg &= ~(SCB::AIRCR::VECTKEY::value | SCB::AIRCR::PRIGROUP::value);
    reg |= (0x5FA << 16) | (group << 8);
    SCB::AIRCR::store(reg);
  }

  static uint32_t get_priority_group(void) {
    return SCB::AIRCR::PRIGROUP::test_and_shift();
  }

  template<int irqn>
  static void set_priority(uint32_t priority) {
    static_assert(irqn < 0 && irqn > -13, "illegal core exception interrupt number");
    SCB::SHPR<((uint32_t)irqn & 0xf)-4>::store((priority << (8 - priority_bits)) & 0xff);
  }

  template<int irqn>
  static uint32_t get_priority(void) {
    static_assert(irqn < 0 && irqn > -13, "illegal core exception interrupt number");
    return((uint32_t)(SCB::SHPR<((uint32_t)irqn & 0xf)-4>::load() >> (8 - priority_bits)));
  }
};


#endif // COMMON_ARM_CORTEX_SCB_HPP_INCLUDED
