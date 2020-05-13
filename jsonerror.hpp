//
//  jsonerror.hpp (included by jsonserial.hpp)
//  Error management.
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

#ifndef jsonerror_hpp
#define jsonerror_hpp

#include <stdexcept>

namespace jsonserial {
  
  /** @brief JsonSerial error.
   *  @see class JsonSerial.
   */
  class JsonError : public std::runtime_error {
  public:
    enum Type {
      OK, CantReadFile, CantWriteFile, NoData, PrematureEOF, InvalidCharacter,
      ExpectingComma, ExpectingDelimiter, ExpectingBrace, ExpectingBracket,
      ExpectingPairOrBrace, ExpectingValueOrBracket, ExpectingString,
      UnknownClass, UnknownSuperclass, RedefinedClass, RedefinedSuperclass,
      UnknownMember, RedefinedMember, AbstractClass, CantCreateObject, CantAddToArray,
      InvalidValue, InvalidID, WrongKeyword, ErrorCount
    };
    
    /// Returns the corresponding error message.
    static const char* error(Type type) {
      constexpr static const char* _errors[ErrorCount] {
        "OK",
        "can't read file (not found or not readable)",
        "can't write file",
        "no data",
        "premature end of file",
        "invalid character in string: ",
        "expecting comma",
        "expecting , or } or ]",
        "expecting {",
        "expecting [",
        "expecting } or name:value pair",
        "expecting ] or value",
        "expecting a quoted name:",
        "unknown class",
        "unknown superclass",
        "class is already declared",
        "already declared as a superclass",
        "unknown member",
        "class member is already defined",
        "can't create instance of abstract class",
        "could not create object",
        "C-style array is too small to add value",
        "invalid value:",
        "ID number expected after @",
        "expecting @id or @class before",
      };
      if (type >= ErrorCount) return "Unknown error";
      else return _errors[type];
    }
    
    Type type;
    bool fatal;
    std::string where, arg, streamname;
    size_t line;

    /// JsonError Error handler.
    using Handler = std::function<void(const JsonError&)>;
    
    JsonError() : runtime_error("JsonSerial Error"){}
    
    void set(Type type_, bool fatal_, const std::string where_, const std::string arg_,
             const std::string& streamname_, size_t line_, Handler errhandler_) {
      type = type_;
      fatal = fatal_;
      where = where_;
      arg = arg_;
      streamname = streamname_;
      line = line_;
      if (errhandler_) (errhandler_)(*this); else print(std::cerr);
    }
    
    /// Prints an error on _out_.
    void print(std::ostream& out) const {
      out << "Error ";
      if (where=="read") out << "while reading file ";
      else if (where=="write") out << "while writing file ";
      else out << "in " << where;
      if (line > 0) out << "at or before line " << line;
      if (!streamname.empty()) out << " in '" << streamname << "'";
      out << ":\n- "<< error(type); if (!arg.empty()) out << " " << arg;
      out << std::endl;
    }
  };
  
}

#endif
