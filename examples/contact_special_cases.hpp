//
//  contact_special_cases.hpp: JsonSerial Example
//
//  JsonSerial: C++ Object Serialization in JSON.
//  (C) Eric Lecolinet 2018 - https://www.telecom-paristech.fr/~elc
//
// NOTES:
// - raw pointers must be properly initialized otherwise JsonSerial will crash
// - std::shared_ptr could be used instead of raw pointers
// - std::unique_ptr can't be used here because objects are pointed by multiple pointers
//

#ifndef CONTACT_HPP
#define CONTACT_HPP

#include <memory>
#include <iostream>
#include <string>
#include <list>
#include <vector>

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class Phone {
public:
  //Phone() = default;   !!! the class does NOT have a no-argument constructor
  
  Phone(const std::string& type, const std::string& number)
  : type(type), number(number) {}
  
  const std::string& getType() const {return type;}
  void setType(const std::string& s) {type = s;}
  
  const std::string& getNumber() const {return number;}
  void setNumber(const std::string& s) {number = s;}
  
private:
  //friend class MyClasses;  !!! MyClasses does NOT have access to instance variables
  std::string type, number;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class Contact {
public:
  enum Gender {Unknown, Male, Female};

  struct Address {
    std::string street, city, state, postcode;
  };
  
  Contact() = default;   // no-argument constructor is needed
  
  Contact(const std::string& firstname,
          const std::string& lastname,
          unsigned short age,
          Gender gender)
  : firstname(firstname), lastname(lastname), gender(gender), age(age) {}
  
  virtual ~Contact() {}   // virtual needed to make the class polymorphic.
  
  void addAddress(Address* a) {address = a;}
  void addPhone(Phone* p)     {phones.push_back(p);}
  
  void addChild(Contact* c)   {children.push_back(c);}
  void addFather(Contact* c)  {father = c;}
  void addMother(Contact* c)  {mother = c;}
  void addPartner(Contact* c) {partner = c;}

private:
  friend class MyClasses;   // MyClasses must have access to instance variables
  std::string firstname, lastname;
  Gender gender{Gender::Unknown};
  bool isalive{true};
  unsigned short age{0};
  Address* address{nullptr};   // !!! raw pointers MUST be initialized
  std::vector<Phone*> phones;
  std::list<Contact*> children;
  Contact* partner{nullptr};
  Contact* father{nullptr};
  Contact* mother{nullptr};
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Photo is an abstract class
class Photo {
public:
  
  virtual void whatever() = 0;  // abstract method

  void setImage(const std::string& image, unsigned int width, unsigned int height) {
    this->image = image; this->width = width; this->height = height;
  }

private:
  friend class MyClasses;  // MyClasses must have access to instance variables
  std::string image;
  unsigned int width{0}, height{0};
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Multiple inheritance
class PhotoContact : public Contact, public Photo {
public:
  PhotoContact() = default;  // no-argument constructor is needed
  
  PhotoContact(const std::string& firstname,
               const std::string& lastname,
               unsigned short age,
               Gender gender)
  : Contact(firstname, lastname, age, gender) {}
  
  void whatever() override {std::cout << "whatever" << std::endl;}
  
  friend class MyClasses;   // MyClasses must have access to instance variables
};

#endif
