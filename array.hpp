//
//  array.hpp: must be included for using std::array
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

#ifndef jsonserial_array_hpp
#define jsonserial_array_hpp

#include <array>

namespace jsonserial {
  
  template <class T, size_t N>
  struct is_std_array<std::array<T,N>> : std::true_type {};

  // add element to a std array (dont confuse with std::is_array for C arrays)
  template<class T>
  struct JsonArrayImpl<T, typename std::enable_if<is_std_array<T>::value>::type> : public JsonArray {
    T& array_;
    size_t index_;
    
    JsonArrayImpl(T& array) : array_(array), index_(0) {}
    
    void add(JsonSerial& js, MetaClass::Creator* cr, const std::string& s) override {
      ObjectPtr* objptr{nullptr};
      if (index_ >= array_.size()) js.error(JsonError::CantAddToArray);
      else readArrayValue(js, array_[index_++], objptr, cr, s);
    }
  };
  
}

#endif
