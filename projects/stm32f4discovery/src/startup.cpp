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

#include <arch/core.hpp>
#include "kernel.hpp"

/* Reset exception: triggered on system startup (system entry point). */
void Kernel::reset_isr(void) {
  mptl::core::startup< sysclk, early_cfg >();

  Kernel::init();
  Kernel::run();
}

#ifndef OPENMPTL_SIMULATION

#include <arch/vector_table.hpp>

extern const uint32_t _stack_top;  /* provided by linker script */

/* Build the vector table:
 * - use irq handler from irq_handler<> traits in Kernel::resources
 * - use Kernel::error_isr as default isr
 */
// TODO: declare Kernel::default_isr in kernel.hpp and use it here:
mptl::vector_table<&_stack_top, Kernel::resources, Kernel::error_isr> vector_table;


#else // OPENMPTL_SIMULATION


#include <iostream>

//int main(int argc, char *argv[])
int main(void)
{
  std::cout << "*** stm32f4discovery demo: starting simulation..." << std::endl;

  Kernel::reset_isr();
}

#endif // OPENMPTL_SIMULATION
