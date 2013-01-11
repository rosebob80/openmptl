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

#ifndef RESOURCE_HPP_INCLUDED
#define RESOURCE_HPP_INCLUDED

#include "resource_mpl.hpp"
#include "core.hpp"
#include "arch/rcc.hpp"

// NOTE: SharedXXX<0> is used as a tag for the values in SharedXXX
template<uint32_t val> struct SharedAPB1ENR : SharedResourceOR, SharedResource<SharedAPB1ENR<0>, val> { };
template<uint32_t val> struct SharedAPB2ENR : SharedResourceOR, SharedResource<SharedAPB2ENR<0>, val> { };

template<char port, uint32_t val> struct SharedCRL      : SharedResourceOR, SharedResource<SharedCRL<port, 0>, val>      { };
template<char port, uint32_t val> struct SharedCRL_Mask : SharedResourceOR, SharedResource<SharedCRL_Mask<port, 0>, val> { };
template<char port, uint32_t val> struct SharedCRH      : SharedResourceOR, SharedResource<SharedCRH<port, 0>, val>      { };
template<char port, uint32_t val> struct SharedCRH_Mask : SharedResourceOR, SharedResource<SharedCRH_Mask<port, 0>, val> { };

template<char port, int pin_no>   struct UniqueGPIO     : CountedResource,  SharedResource<UniqueGPIO<port, pin_no>, 1>  { };



template<typename T, typename... Args>
void ResourceList<T, Args...>::configure(void)
{
  Core::RCC::APB1ENR::store( value< SharedAPB1ENR<0> >() );
  Core::RCC::APB2ENR::store( value< SharedAPB2ENR<0> >() );

  // TODO: loop tuple
  if(value< SharedCRL_Mask<'A', 0> >()) Core::GPIO<'A'>::CRL::store((Core::GPIO<'A'>::CRL::load() & ~value< SharedCRL_Mask<'A', 0> >()) | value< SharedCRL<'A', 0> >());
  if(value< SharedCRH_Mask<'A', 0> >()) Core::GPIO<'A'>::CRH::store((Core::GPIO<'A'>::CRH::load() & ~value< SharedCRH_Mask<'A', 0> >()) | value< SharedCRH<'A', 0> >());

  if(value< SharedCRL_Mask<'B', 0> >()) Core::GPIO<'B'>::CRL::store((Core::GPIO<'B'>::CRL::load() & ~value< SharedCRL_Mask<'B', 0> >()) | value< SharedCRL<'B', 0> >());
  if(value< SharedCRH_Mask<'B', 0> >()) Core::GPIO<'B'>::CRH::store((Core::GPIO<'B'>::CRH::load() & ~value< SharedCRH_Mask<'B', 0> >()) | value< SharedCRH<'B', 0> >());

  if(value< SharedCRL_Mask<'C', 0> >()) Core::GPIO<'C'>::CRL::store((Core::GPIO<'C'>::CRL::load() & ~value< SharedCRL_Mask<'C', 0> >()) | value< SharedCRL<'C', 0> >());
  if(value< SharedCRH_Mask<'C', 0> >()) Core::GPIO<'C'>::CRH::store((Core::GPIO<'C'>::CRH::load() & ~value< SharedCRH_Mask<'C', 0> >()) | value< SharedCRH<'C', 0> >());

  if(value< SharedCRL_Mask<'D', 0> >()) Core::GPIO<'D'>::CRL::store((Core::GPIO<'D'>::CRL::load() & ~value< SharedCRL_Mask<'D', 0> >()) | value< SharedCRL<'D', 0> >());
  if(value< SharedCRH_Mask<'D', 0> >()) Core::GPIO<'D'>::CRH::store((Core::GPIO<'D'>::CRH::load() & ~value< SharedCRH_Mask<'D', 0> >()) | value< SharedCRH<'D', 0> >());

  if(value< SharedCRL_Mask<'E', 0> >()) Core::GPIO<'E'>::CRL::store((Core::GPIO<'E'>::CRL::load() & ~value< SharedCRL_Mask<'E', 0> >()) | value< SharedCRL<'E', 0> >());
  if(value< SharedCRH_Mask<'E', 0> >()) Core::GPIO<'E'>::CRH::store((Core::GPIO<'E'>::CRH::load() & ~value< SharedCRH_Mask<'E', 0> >()) | value< SharedCRH<'E', 0> >());

  if(value< SharedCRL_Mask<'F', 0> >()) Core::GPIO<'F'>::CRL::store((Core::GPIO<'F'>::CRL::load() & ~value< SharedCRL_Mask<'F', 0> >()) | value< SharedCRL<'F', 0> >());
  if(value< SharedCRH_Mask<'F', 0> >()) Core::GPIO<'F'>::CRH::store((Core::GPIO<'F'>::CRH::load() & ~value< SharedCRH_Mask<'F', 0> >()) | value< SharedCRH<'F', 0> >());

  if(value< SharedCRL_Mask<'G', 0> >()) Core::GPIO<'G'>::CRL::store((Core::GPIO<'G'>::CRL::load() & ~value< SharedCRL_Mask<'G', 0> >()) | value< SharedCRL<'G', 0> >());
  if(value< SharedCRH_Mask<'G', 0> >()) Core::GPIO<'G'>::CRH::store((Core::GPIO<'G'>::CRH::load() & ~value< SharedCRH_Mask<'G', 0> >()) | value< SharedCRH<'G', 0> >());
}


#ifndef CORE_SIMULATION
// recursively assert count<=1 for UniqueGPIO
template<typename resources, char port, int pin_no>
struct UniqueGPIO_Assert {
  static_assert(resources::template value<UniqueGPIO<port, pin_no> >() <= 1, "UniqueGPIO is not unique");
  UniqueGPIO_Assert<resources, port, pin_no + 1> recursion;
};
template<typename resources, char port>
struct UniqueGPIO_Assert<resources, port, 15> {
  static_assert(resources::template value<UniqueGPIO<port, 0> >() <= 1, "UniqueGPIO is not unique");
  UniqueGPIO_Assert<resources, port + 1, 0> recursion;
};
template<typename resources>
struct UniqueGPIO_Assert<resources, 'G', 15> {
  static_assert(resources::template value<UniqueGPIO<'G', 15> >() <= 1, "UniqueGPIO is not unique");
};

template<typename T, typename... Args>
void ResourceList<T, Args...>::assert(void)
{
  UniqueGPIO_Assert<ResourceList<T, Args...>, 'A', 0> assert;
}
#endif



#endif // RESOURCE_HPP_INCLUDED