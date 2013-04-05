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

#ifndef SCREENITEM_HPP_INCLUDED
#define SCREENITEM_HPP_INCLUDED

#include <boost/intrusive/list.hpp>

class ScreenItem;

using ScreenItemList = boost::intrusive::list<
  ScreenItem,
  boost::intrusive::link_mode<boost::intrusive::link_mode_type::normal_link>
  >;

class ScreenItem
: public boost::intrusive::list_base_hook<> /* doubly linked list */
{
  bool inverted = false;

public:
  boost::intrusive::list_member_hook<> member_hook_;

  ScreenItem(ScreenItemList & item_list, bool _inverted = false) : inverted(_inverted) {
    item_list.push_back(*this);
  }

  void set_inverted(bool inv = true) { inverted = inv; }
  bool get_inverted(void) const { return inverted; }

  virtual const char * c_str(void) const { return ""; }
};

class TextRow
: public ScreenItem
{
  const char * text;

public:

  TextRow(ScreenItemList & item_list, bool _inverted = false)
    : ScreenItem(item_list, _inverted) { }

  TextRow(ScreenItemList & item_list, const char * _text, bool _inverted = false)
    : ScreenItem(item_list, _inverted), text(_text) { }

  virtual const char * c_str(void) const { return text; }
};


class DataRow
: public ScreenItem
{
protected:

  const char * name;
  uint32_t value;
  static char buf[]; /* static! NEVER use DataRows non-linear */

public:
  DataRow(ScreenItemList & item_list, const char * _name, bool _inverted = false) : ScreenItem(item_list, _inverted), name(_name) { }
  virtual const char * c_str(void) const;
  void set(uint32_t _value) { value = _value; }
  void operator=(uint32_t _value)  { set(_value); }
};


class DataRowHex
: public DataRow
{
public:
  DataRowHex(ScreenItemList & item_list, const char * _name, bool _inverted = false) : DataRow(item_list, _name, _inverted) { }
  virtual const char * c_str(void) const;
  void operator=(uint32_t _value)  { set(_value); }
};


#endif
