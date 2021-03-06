/*
 * OpenMPTL - C++ Microprocessor Template Library
 *
 * Copyright (C) 2012-2017 Axel Burri <axel@tty0.ch>
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

#include "timepoint.hpp"
#include "time.hpp"
#include "kernel.hpp"

std::atomic<systick_t> SystemTime::systick_count;

void SystemTime::systick_isr(void) {
  systick_count.fetch_add(1, std::memory_order_relaxed);
}

void SystemTime::rtc_isr() {
  Kernel::rtc::clear_second_flag();
  Kernel::led::toggle();
}

void SystemTime::nanosleep(unsigned int ns) {
  TimePoint end;
  end.set();
  end.add_ns(ns);
  TimePoint now;
  now.set();
  while(now < end)
    now.set();
}
