//
//  unordered_set.hpp: must be included for using std::unordered_set
//
//  JsonSerial: C++ Object Serialization in JSON.
//  See: https://www.telecom-paris.fr/~elc/software/jsonserial.html
//  (C) Eric Lecolinet 2017/2019 - https://www.telecom-paris.fr/~elc
//
//  JsonSerial is free software; you can redistribute it and/or modify it
//  under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  JsonSerial is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
//  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
//  License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License along
//  with this program; if not, see https://www.gnu.org/licenses/lgpl-3.0.html.
//

#ifndef jsonserial_unordered_set_hpp
#define jsonserial_unordered_set_hpp

#include <unordered_set>
#include "./set.hpp"

namespace jsonserial {
  
  template <class T, class Comp, class Alloc>
  struct is_std_set<std::unordered_set<T,Comp,Alloc>> : std::true_type {};

  template <class T, class Comp, class Alloc>
  struct is_std_set<std::unordered_multiset<T,Comp,Alloc>> : std::true_type {};
}

#endif
