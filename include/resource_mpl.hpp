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

#ifndef RESOURCE_MPL_HPP_INCLUDED
#define RESOURCE_MPL_HPP_INCLUDED

#include <type_traits>
#include <cstdint>
#include <isr.hpp>  // isr_t

namespace mpl
{
  ////////////////////  resource  ////////////////////


  /**
   * Template arguments:
   *
   * - T (group_type): resource group type. Acts as filter for resource_type_list. This type (T)
   *                   MUST provide the configure() and assert_type() functions.
   *
   * - R (type): resource type, MUST provide combine<> type and configure() function.
   */
  template<typename T, typename R = T>
  struct resource {
    using group_type = T;
    using type = R;

    /* Append resource R to the filtered_list (Tf) if Filter matches our type (T). */
    template<typename Tf, typename Filter>
    using append_filtered_list = typename std::conditional<
      std::is_same<Filter, group_type>::value,
      typename Tf::template append<type>,
      Tf>::type;

    /* Append type T to the resource_type_list (Tu) if resource_type_list does not yet hold our type (T). */
    template<typename Tu>
    using append_resource_type_list = typename std::conditional<
      Tu::template contains<group_type>::value,
      Tu,
      typename Tu::template append<group_type>
      >::type;

    void configure() { }

    template<typename U>
    struct combine {
      /* combine asserts by default */
      using type = void;
      static_assert(std::is_void<U>::value,
                    "ResourceList contains a resource group which does not implement the combine functor.");
    };
  };


  ////////////////////  unique_resource  ////////////////////


  template<typename T, typename R = T>
  struct unique_resource : resource< T, R > {
    template<typename U>
    struct combine {
      using type = void;
      static_assert(std::is_void<U>::value,  /* always assert. combining unique resources means that two same types exist */
                    "ResourceList contains a resource derived from unique_resource which is not unique. (see compile message \"mpl::filtered_list<MyResource< XXX >, MyResource< XXX >, ... >\" above.)");
    };

    template<typename Rl>
    static constexpr bool assert_type() {
      /* We check for "Rl::combined_type<type>::type here", and assert    */
      /* in "combine" above. The reason for this is to get nicer          */
      /* compiler messages than if we were just asserting for             */
      /* "Rl::resource_filtered_list<type>::size <= 1" (which is also     */
      /* correct).                                                        */
      return !std::is_void<typename Rl::template combined_type<T>::type >::value;
    }
  };


  ////////////////////  resource_group  ////////////////////


  struct resource_group
  {
    template<typename Rl> static constexpr bool assert_type() { return true; }
  };


  ////////////////////  filtered_list  ////////////////////


  template<typename... Args>
  struct filtered_list_impl;

  template<typename Head, typename... Args>
  struct filtered_list_impl<Head, Args...>  {
    using combined_type = typename Head::template combine<typename filtered_list_impl<Args...>::combined_type>::type;
  };
  template<typename Head>
  struct filtered_list_impl<Head> {
    using combined_type = typename Head::type;
  };
  template<>
  struct filtered_list_impl<> {
    /* combined_type is void for empty filtered list */
    using combined_type = void ;
  };

  template<typename... Args>
  struct filtered_list : filtered_list_impl<Args...> {
    template<typename T>
    using append = filtered_list<Args..., T>;
    static constexpr std::size_t size = sizeof...(Args);
  };


  /* Creates a filtered_list<...>: list of resource class types,               */
  /* providing combined_type (e.g. or'ed  register values for SharedRegister). */
  template<typename T, typename Filter, typename... Args>
  struct make_filtered_list;

  template<typename T, typename Filter, typename Head, typename... Args>
  struct make_filtered_list<T, Filter, Head, Args...> {
    using type = typename make_filtered_list<typename Head::template append_filtered_list<T, Filter>, Filter, Args...>::type;
  };

  template<typename T, typename Filter>
  struct make_filtered_list<T, Filter> {
    using type = T;
  };


  ////////////////////  resource_type_list  ////////////////////


  template<typename... Args>
  struct resource_type_list_impl;

  template<typename Head, typename... Args>
  struct resource_type_list_impl<Head, Args...>  {
    template<typename T>
    struct contains {
      static constexpr bool value = ( std::is_same<Head, T>::value || 
                                      resource_type_list_impl<Args...>::template contains<T>::value );
    };

    template<typename Rl>
    static void configure() {
      typename Rl::template resource_filtered_list<Head>::combined_type().configure();
      resource_type_list_impl<Args...>::template configure<Rl>();
    }

    template<typename Rl>
    static constexpr bool assert_type() {
      return (Head::template assert_type<Rl>() &&
              resource_type_list_impl<Args...>::template assert_type<Rl>() );
    }
  };
  template<>
  struct resource_type_list_impl<> {
    template<typename T>
    using contains = std::false_type;

    template<typename Rl> static void configure() { }
    template<typename Rl> static constexpr bool assert_type() { return true; }
  };

  template<typename... Args>
  struct resource_type_list : resource_type_list_impl<Args...> {
    template<typename T>
    using append = resource_type_list<Args..., T>;

    static constexpr std::size_t size = sizeof...(Args);
  };


  /* Creates a resource_type_list<...>: list of unique resource types */
  template<typename T, typename... Args>
  struct make_resource_type_list;

  template<typename T, typename Head, typename... Args>
  struct make_resource_type_list<T, Head, Args...> {
    using type = typename make_resource_type_list<typename Head::template append_resource_type_list<T>, Args...>::type;
  };

  template<typename T>
  struct make_resource_type_list<T> {
    using type = T;
  };
} // namespace mpl


#endif // RESOURCE_MPL_HPP_INCLUDED
