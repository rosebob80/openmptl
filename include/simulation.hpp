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

#ifndef SIMULATION_HPP_INCLUDED
#define SIMULATION_HPP_INCLUDED

#ifdef OPENMPTL_SIMULATION
#  include <iostream>
#  include <thread>
#  define SIM_DEBUG(msg) std::clog << msg << std::endl
#  define SIM_TRACE(msg) std::clog << msg << " - " << __PRETTY_FUNCTION__ << std::endl
#  define SIM_RELAX      std::this_thread::sleep_for( std::chrono::milliseconds( 20 ) )
#else
#  define SIM_DEBUG(msg)
#  define SIM_TRACE(msg)
#  define SIM_RELAX
#endif // OPENMPTL_SIMULATION


#endif // SIMULATION_HPP_INCLUDED
