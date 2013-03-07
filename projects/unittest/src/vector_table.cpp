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

#include <resource_mpl.hpp>
#include <arch/vector_table.hpp>
#include <cassert>
#include "unittest_static_assert.hpp"

static unsigned int stack_top = 0;
static int isr_test = 0;

static void default_isr(void) { isr_test = 0; }
static void isr_42(void) { isr_test = 42; }

/* Handler for irq number = 42 */
typedef IrqResource< 42, isr_42 > irq42;


typedef ResourceList < irq42 > resource_list;
typedef ResourceList < irq42, irq42 > resource_fail_list;

int main()
{
  VectorTable<&stack_top, resource_list, default_isr> vector_table;

  UNITTEST_STATIC_ASSERT( VectorTable<&stack_top, default_isr, resource_fail_list> vector_table_fail; )

  resource_list::check();

  isr_t *vt = vector_table.vector_table_p;

  assert((unsigned long)vt[0] == (unsigned long)&stack_top);
  assert((unsigned long)vt[41] == (unsigned long)default_isr);
  assert((unsigned long)vt[42 + Irq::numof_core_exceptions + 1] == (unsigned long)isr_42);


  std::cout << "vector table size = " << vector_table.size << " (" <<
    "1 stack_top_ptr, " <<
    Irq::numof_core_exceptions << " core_exceptions, " <<
    Irq::numof_interrupt_channels << " irq_channels)" << std::endl;

  std::cout << std::endl;
  std::cout << "vector table dump" << std::endl;
  std::cout << "-----------------" << std::endl;

  for(unsigned i = 0; i < vector_table.size; i++)
    std::cout << std::dec << std::setw(3) << i << ": 0x" << std::hex << (unsigned long)vt[i] << std::endl;

  return 0;
}