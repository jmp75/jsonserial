# jsonserial

C++ Object Serialization in JSON

This library is a versioned copy of [jsonserial](https://perso.telecom-paristech.fr/elc/software/jsonserial.html) by [Eric Lecolinet](https://perso.telecom-paristech.fr/elc/)

* JsonSerial allows serializing any set of C++ objects (a single object or a collection, a tree or even a cyclic graph of objects).
* JsonSerial is not intrusive in that sense it does not require writing read/write functions in the C++ classes. Instead, classes and variables are  serialized by calling Jsonserial functions outside C++ classes.
* Variables can be arithmetic values, enums, C and C++ strings, C++ objects (plain objects or pointers, including shared ptr and unique ptr), C++ containers (vector, list, map, unordered map) and 1-dimensional arrays with brackets. Pointees are automatically created when reading a JSON file.
* The names in the JSON file can be the names of the C++ variables or whatever desired. This allows managing human-readable files such as resource or  configuration files.
* JsonSerial supports multiple inheritance and polymorphism. If a C++ pointer points to an object of a derived class, its class name is stored in the JSON  file so that the object can be created properly when reading the file.
* JsonSerial optionally supports shared objects. Objects pointed by several pointers are not duplicated when writing files and reading them again. This also allows serializing a cyclic graph of objects.
* JsonSerial allows relaxing the JSON syntax in various ways (comments are supported, quotes and commas can optionally be omitted, etc.)
* JsonSerial consists of header file and relies on C++ 11 templates.
* JsonSerial requires UTF8. It has been tested on MacOS and Linux Debian but not on Windows.
* Licence: GNU Lesser General Public License (LGPL).

## Documentation

[Manual at the original source web site](https://perso.telecom-paristech.fr/elc/software/jsonserial.pdf)


