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

#ifndef USART_HPP_INCLUDED
#define USART_HPP_INCLUDED

#include <arch/gpio.hpp>
#include "../../../common/usart.hpp"

namespace mptl {

template< unsigned _usart_no, typename _rcc >
class usart : public usart_stm32_common<_usart_no, _rcc>
{
  using base_type = usart_stm32_common<_usart_no, _rcc>;

  template<char port, unsigned pin_no>
  struct gpio_rx_impl
  {
    using gpio_type = gpio< port, pin_no >; // gpio_input_af<port, pin_no>
    using type = typelist<
      typename base_type::enable_rx,
      typename gpio_type::resources,
      typename gpio_type::resistor::floating,
      typename gpio_type::mode::alternate_function,  // implicitely set by alt_func_num, but does not harm
      typename gpio_type::template alt_func_num< (usart::usart_no <= 3) ? 7 : 8 >
      >;
  };

  template<char port, unsigned pin_no>
  struct gpio_tx_impl
  {
    using gpio_type = gpio< port, pin_no >;
    using type = typelist<
      typename base_type::enable_tx,
      typename gpio_type::resources,
      typename gpio_type::output_type::push_pull,
      typename gpio_type::resistor::floating,
      typename gpio_type::template speed< mhz(50) >,
      typename gpio_type::mode::alternate_function,  // implicitely set by alt_func_num, but does not harm
      typename gpio_type::template alt_func_num< (usart::usart_no <= 3) ? 7 : 8 >
        >;
  };
public:

  /** 
   * Provide GPIO port/pin_no for RX (used for configuration of the GPIO registers).
   * NOTE: this implicitely sets the enable_rx option!
   */
  // TODO: provide a matrix for the gpio port/pin_no
  template<char port, unsigned pin_no>
  using gpio_rx = typename gpio_rx_impl<port, pin_no>::type;

  /** 
   * Provide GPIO port/pin_no for TX (used for configuration of the GPIO registers).
   * NOTE: this implicitely sets the enable_tx option!
   */
  template<char port, unsigned pin_no>
  using gpio_tx = typename gpio_tx_impl<port, pin_no>::type;
};

} // namespace mptl


#endif // USART_HPP_INCLUDED

