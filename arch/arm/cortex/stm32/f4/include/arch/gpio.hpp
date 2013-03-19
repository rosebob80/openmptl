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

#ifndef GPIO_HPP_INCLUDED
#define GPIO_HPP_INCLUDED

#include <resource.hpp>
#include <arch/rcc.hpp>
#include <arch/reg/gpio.hpp>

enum class GpioResistorConfig {
  floating,       //< Floating input
  pull_up,        //< Input with pull-up
  pull_down       //< Input with pull-down
};

enum class GpioOutputType {
  push_pull,      //< General purpose output push-pull (e.g. LED's)
  open_drain,     //< General purpose output open-drain
};

enum class GpioActiveState {
  low,            //< pin is low-active
  high            //< pin is high-active
};

enum class GpioMode {
  input,
  output,
  alternate_function,
  analog
};


////////////////////  Gpio  ////////////////////


template<char port,
         unsigned pin_no,
         GpioMode moder_cnf = GpioMode::input,
         GpioOutputType otype_cnf = GpioOutputType::push_pull,
         freq_t speed = 2_mhz,
         GpioResistorConfig resistor_cnf = GpioResistorConfig::floating,
         unsigned alt_func_num = 0>
class Gpio
{
  static_assert((port >= 'A') && (port <= 'I'), "Invalid GPIO port");
  static_assert(pin_no < 16, "Invalid GPIO pin-no");

  static_assert(alt_func_num < 16, "illegal alternate function number");

  static_assert((speed == 2_mhz) ||
                (speed == 25_mhz) ||
                (speed == 50_mhz) ||
                (speed == 100_mhz),
                "Illegal frequency for GpioOutput speed (allowed: 2_mhz, 25_mhz, 50_mhz, 100_mhz)");
protected:
  static constexpr uint32_t pin_mask         = (uint32_t)0x1 << pin_no;
  static constexpr uint32_t pin_mask_double  = (uint32_t)0x3 << (pin_no * 2);

  static constexpr uint32_t moder_value = (moder_cnf == GpioMode::output ? 1 :
                                           moder_cnf == GpioMode::alternate_function ? 2 :
                                           moder_cnf == GpioMode::analog ? 3 :
                                           0) << (pin_no * 2);

  static constexpr uint32_t otyper_value = (otype_cnf == GpioOutputType::open_drain) ? pin_mask : 0;

  static constexpr uint32_t ospeedr_value =  (speed == 25_mhz  ? 1 :
                                              speed == 50_mhz  ? 2 :
                                              speed == 100_mhz ? 3 :
                                              0) << (pin_no * 2) ;

  static constexpr uint32_t pupdr_value =  (resistor_cnf == GpioResistorConfig::pull_up   ? 1 :
                                            resistor_cnf == GpioResistorConfig::pull_down ? 2 :
                                            0x00) << (pin_no * 2) ;

  static constexpr uint32_t afrl_value = pin_no <  8 ? alt_func_num << ((pin_no % 8) * 4) : 0;
  static constexpr uint32_t afrl_mask  = pin_no <  8 ? 0xf          << ((pin_no % 8) * 4) : 0;
  static constexpr uint32_t afrh_value = pin_no >= 8 ? alt_func_num << ((pin_no % 8) * 4) : 0;
  static constexpr uint32_t afrh_mask  = pin_no >= 8 ? 0xf          << ((pin_no % 8) * 4) : 0;

  typedef reg::GPIO<port> GPIOx;

public:

  typedef Gpio<port, pin_no, moder_cnf, otype_cnf, speed, resistor_cnf, alt_func_num> type;

  using resources = ResourceList<
    Rcc_gpio_clock_resources<port>,
    UniqueResource< Gpio<port, pin_no> >,

    // TODO: make sure the registers are only set if they differ from reset value. does this make sense?
    SharedRegister< typename reg::GPIO<port>::MODER,   moder_value,   pin_mask_double >,
    SharedRegister< typename reg::GPIO<port>::OTYPER,  otyper_value,  pin_mask        >,
    SharedRegister< typename reg::GPIO<port>::OSPEEDR, ospeedr_value, pin_mask_double >,
    SharedRegister< typename reg::GPIO<port>::PUPDR,   pupdr_value,   pin_mask_double >,
    SharedRegister< typename reg::GPIO<port>::AFRL,    afrl_value,    afrl_mask       >,
    SharedRegister< typename reg::GPIO<port>::AFRH,    afrh_value,    afrh_mask       >
    >;

  static void init() {
  }

  static void set() {
    GPIOx::BSRR::store(pin_mask);
  }
  static void reset() {
    GPIOx::BSRR::store(pin_mask << 16);
  }

  static uint32_t read_input_bit() {
    return GPIOx::IDR::test(pin_mask);
  }

  static uint32_t read_output_bit() {
    return GPIOx::ODR::test(pin_mask);
  }
};


////////////////////  GpioInput  ////////////////////


template<char port,
         unsigned pin_no,
         GpioResistorConfig resistor_cnf,
         GpioActiveState active_state = GpioActiveState::low>
class GpioInput
: public Gpio< port,
               pin_no,
               GpioMode::input,
               GpioOutputType::push_pull,
               2_mhz,
               resistor_cnf >
{
  typedef GpioInput<port, pin_no, resistor_cnf, active_state> type;
public:
  static bool active(void) {
    bool input = type::read_input_bit();
    return active_state == GpioActiveState::low ? !input : input;
  }
};


////////////////////  GpioInputAF  ////////////////////


template<char port,
         unsigned pin_no,
         unsigned alt_func_num,
         GpioResistorConfig resistor_cnf = GpioResistorConfig::floating
         >
class GpioInputAF
: public Gpio< port,
               pin_no,
               GpioMode::alternate_function,
               GpioOutputType::push_pull,
               2_mhz,
               resistor_cnf,
               alt_func_num >
{ };


////////////////////  GpioOutput  ////////////////////


template<char port,
         unsigned pin_no,
         GpioOutputType otype_cnf,
         GpioResistorConfig resistor_cnf = GpioResistorConfig::floating,
         freq_t speed = 50_mhz,
         GpioActiveState active_state = GpioActiveState::low>
class GpioOutput
: public Gpio< port,
               pin_no,
               GpioMode::output,
               otype_cnf,
               speed,
               resistor_cnf >
{
public:
  typedef GpioOutput<port, pin_no, otype_cnf, resistor_cnf, speed, active_state> type;

  static void enable() {
    if(active_state == GpioActiveState::low) {
      type::reset();
    } else {
      type::set();
    }
  }

  static void disable() {
    if(active_state == GpioActiveState::low) {
      type::set();
    } else {
      type::reset();
    }
  }

  static bool active() {
    bool input = type::read_input_bit();
    return active_state == GpioActiveState::low ? !input : input;
  }

  static void toggle() {
    if(type::read_input_bit()) {
      type::reset();
    }
    else {
      type::set();
    }
  }

  static bool latched() {
    bool output = type::read_output_bit();
    return active_state == GpioActiveState::low ? !output : output;
  }
};


////////////////////  GpioOutputAF  ////////////////////


template<char port,
         unsigned pin_no,
         unsigned alt_func_num,
         GpioOutputType otype_cnf = GpioOutputType::open_drain,
         GpioResistorConfig resistor_cnf = GpioResistorConfig::floating,
         freq_t speed = 50_mhz>
class GpioOutputAF
: public Gpio< port,
               pin_no,
               GpioMode::alternate_function,
               otype_cnf,
               speed,
               resistor_cnf,
               alt_func_num >
{ };


////////////////////  GpioAnalogIO  ////////////////////


template<char port,
         unsigned pin_no>
class GpioAnalogIO
: public Gpio< port,
               pin_no,
               GpioMode::analog,
               GpioOutputType::push_pull,
               2_mhz,
               GpioResistorConfig::floating >
{
  // TODO: get/set analog value
};


////////////////////  GpioLed  ////////////////////


template<char port,
         unsigned pin_no,
         GpioOutputType otype_cnf = GpioOutputType::push_pull,
         GpioResistorConfig resistor_cnf = GpioResistorConfig::floating,
         freq_t speed = 50_mhz,
         GpioActiveState active_state = GpioActiveState::high>
class GpioLed : public GpioOutput<port, pin_no, otype_cnf, resistor_cnf, speed, active_state> {
public:
  typedef GpioLed<port, pin_no, otype_cnf, resistor_cnf, speed, active_state> type;

  static void on() {
    type::enable();
  }
  static void off() {
    type::disable();
  }
};

#endif // GPIO_HPP_INCLUDED


