//
//  vector.hpp: must be included for using std::vector
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

#ifndef jsonserial_vector_hpp
#define jsonserial_vector_hpp

#include <vector>

namespace jsonserial {
  
  template <class T, class Alloc>
  struct is_std_vector<std::vector<T,Alloc>> : std::true_type {};
  
  template <class Alloc>
  struct is_std_vector<std::vector<bool,Alloc>> : std::true_type {};
  
  // add element to a vector or a deque.
  template<class T>
  struct JsonArrayImpl<T, typename std::enable_if<is_std_vector<T>::value>::type> : public JsonArray {
    T& cont_;
    struct Ptr {size_t index; ObjectPtr* jsp;};
    std::vector<Ptr> pointers_;
    
    JsonArrayImpl(T& cont) : cont_(cont) {cont_.clear();}
    
    void add(JsonSerial& js, MetaClass::Creator* cr, const std::string& s) override {
      cont_.resize(cont_.size()+1);
      ObjectPtr* objptr{nullptr};
      readArrayValue(js, cont_.back(), objptr, cr, s);
      
      // TO BE COMPLETED : shared pointers not fixed when elements in this vector point
      // to an element that was created in this vector
      if (objptr && objptr->init_) {
        pointers_.push_back(Ptr{cont_.size()-1, objptr});
      }
    }
    
    void end(JsonSerial&) override {
      cont_.shrink_to_fit();
      if (pointers_.size() > 1) fixPointers<T>(cont_, pointers_);
    }
    
    template<class C>
    static void fixPointers(typename std::enable_if<!is_smart_ptr<typename C::value_type>::value,C>::type &,
                            std::vector<Ptr> &) {}
    
    template<class C>
    static void fixPointers(typename std::enable_if<is_smart_ptr<typename C::value_type>::value,C>::type & cont,
                            std::vector<Ptr> & pointers) {
      for (auto& it : pointers) {
        it.jsp->shared_ = &cont[it.index];
      }
    }
  };
  
}

#endif
