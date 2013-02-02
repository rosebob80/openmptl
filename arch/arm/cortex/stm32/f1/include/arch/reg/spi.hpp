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

#ifndef REG_SPI_HPP_INCLUDED
#define REG_SPI_HPP_INCLUDED

#include "../../../../common/reg/spi.hpp"

namespace reg
{
  template<std::size_t spi_no>
  class SPI
  {
    static_assert((spi_no >= 1) && (spi_no <= 3), "unsupported SPI number"); // TODO: depends on cpu sub-arch
  };

  template<> class SPI<1> : public __SPI_COMMON< 0x40013000 > { };
  template<> class SPI<2> : public __SPI_COMMON< 0x40003800 > { };
  template<> class SPI<3> : public __SPI_COMMON< 0x40003C00 > { };
}

#endif // REG_SPI_HPP_INCLUDED
