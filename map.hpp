//
//  map.hpp: must be included for using std::map
//
//  JsonSerial: C++ Object Serialization in JSON.
//  See: https://www.telecom-paris.fr/~elc/software/jsonserial.html
//  (C) Eric Lecolinet 2017/2018 - https://www.telecom-paristech.fr/~elc
//
//  JsonSerial is free software; you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  JsonSerial is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
//  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
//  License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, see https://www.gnu.org/licenses/lgpl-3.0.html.
//

#ifndef jsonserial_map_hpp
#define jsonserial_map_hpp

#include <map>

namespace jsonserial {
  
  template <class Key, class T, class Comp, class Alloc>
  struct is_std_map<std::map<Key,T,Comp,Alloc>> : std::true_type {};

  template <class Key, class T, class Comp, class Alloc>
  struct is_std_map<std::multimap<Key,T,Comp,Alloc>> : std::true_type {};
  
}
#endif
