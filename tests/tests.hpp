//
//  serial_tests.hpp
//  Created by elc.
//  Copyright © 2017 elc. All rights reserved.
//

#include <string>
#include <memory>
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <vector>
using std::string;
using std::shared_ptr;
using std::unique_ptr;

// comment for using raw pointers
#define SHARED 1

// - - - - - - - - - - - - - - - -

class Contact;
class Contacts;
class PhotoContact;
class Notes;
class Names;

#if SHARED
using ContactPtr = shared_ptr<Contact>;
using PhotoContactPtr = shared_ptr<PhotoContact>;
using ContactsPtr = shared_ptr<Contacts>;
#else
using ContactPtr = Contact*;
using PhotoContactPtr = PhotoContact*;
using ContactsPtr = Contacts*;
#endif

// - - - - - - - - - - - - - - - -

// NOT friend of MyClasses, accessors must be used
// No standard constructor: a creator function will be needed
class PhoneNumber {
public:
  PhoneNumber(const std::string& type, const std::string& number) : type(type), number(number) {}
  
  const std::string& getType() const {return type;}
  void setType(const std::string& s) {type = s;}
  
  const std::string& getNumber() const {return number;}
  void setNumber(const std::string& s) {number = s;}
  
private:
  std::string type, number;
};

// - - - - - - - - - - - - - - - -

class Contact {
public:
  enum struct Gender {Unknown, Male, Female};
  
  struct Address {
    std::string street, city, state, postcode;
  };

  // virtual need to make class polymorphic!
  virtual ~Contact();

  Contact() {}
  Contact(const std::string& firstname, const std::string& lastname, unsigned short age, Gender);
  
  void addAddress(Address* address, shared_ptr<Address> saddress);
  void addPhone(const std::string& type, const std::string& number);
  void addPhone(PhoneNumber* phone, shared_ptr<PhoneNumber> sphone);

  void addPartner(ContactPtr c) {partner = c;}
  void addChild(ContactPtr c) {children.push_back(c);}
  void addMother(ContactPtr c) {mother = c;}
  void addFather(ContactPtr c) {father = c;}

private:
  friend class MyClasses;
  friend class Contacts;
  
  static long static_var;
  
  std::string firstname1, lastname1;
  std::string *firstname2{nullptr}, *lastname2{nullptr};
  shared_ptr<std::string> firstname3, lastname3;
  unique_ptr<std::string> firstname4, lastname4;
  char *firstname5{nullptr}, *lastname5{nullptr};
  const char *firstname6{nullptr}, *lastname6{nullptr};
  
  Gender gender{Gender::Unknown};
  char sex{'U'};
  bool isalive{true};
  
  unsigned short age1{0};
  unsigned short* age2{new unsigned short(0)};
  shared_ptr<unsigned short> age3{new unsigned short(0)};
  unique_ptr<unsigned short> age4{new unsigned short(0)};
  
  Address address1;
  Address* address2;
  shared_ptr<Address> address3;
  unique_ptr<Address> address4;
  
  int phonecount{0};
  PhoneNumber* phonenumbers1[3]{nullptr};
  shared_ptr<PhoneNumber> phonenumbers2[3]{nullptr};
  std::vector<PhoneNumber*> phonenumbers3;
  std::vector<shared_ptr<PhoneNumber>> phonenumbers4;
  
  Names* names{nullptr};
  Notes* notes{nullptr};

  std::list<ContactPtr> children;;
  //std::vector<ContactPtr> children;;
  ContactPtr partner{nullptr};
  ContactPtr father{nullptr};
  ContactPtr mother{nullptr};
};

// - - - - - - - - - - - - - - - -

class Contacts {
public:
  Contacts() {}
  Contacts(int count, bool cycling_graph);
private:
  friend class MyClasses;
  std::list<ContactPtr> contacts;
  //std::vector<ContactPtr> contacts;
  ContactPtr makeFamily(const std::string& family_name, bool cycling_graph);
};

// - - - - - - - - - - - - - - - -

class Photo {
public:
  virtual void doit() = 0; // abstract class
  
  void setImage(const string& image_, unsigned int width_, unsigned int height_) {
    image = image_; width = width_; height = height_;
  }
  
private:
  friend class MyClasses;
  string image;
  unsigned int width{0}, height{0};
};

// - - - - - - - - - - - - - - - -

class PhotoContact : public Contact, public Photo {  // multiple inheritance
  friend class MyClasses;
public:
  PhotoContact() {}
  
  PhotoContact(const string& firstname, const string& lastname, unsigned short age, Gender gender)
  : Contact(firstname, lastname, age, gender) {}
  
  void doit() override {}
};

// - - - - - - - - - - - - - - - -

class Names {
public:
  Names(bool init = false);
private:
  friend class MyClasses;
  std::string names1[3];
  std::array<std::string,3> names2;
  std::deque<std::string> names3;
  std::forward_list<std::string> names4;
  std::list<std::string> names5;
  std::set<std::string> names6;
  std::unordered_set<std::string> names7;
  std::vector<std::string> names8;
  std::map<std::string,std::string> names9;
  std::unordered_map<std::string,std::string> names10;
  
  char* cnames1[3];
  std::array<char*,3> cnames2;
  std::deque<char*> cnames3;
  std::forward_list<char*> cnames4;
  std::list<char*> cnames5;
  std::set<char*> cnames6;
  std::unordered_set<char*> cnames7;
  std::vector<char*> cnames8;
  std::map<string, char*> cnames9;
  std::unordered_map<string, char*> cnames10;
  
  const char* ccnames1[3];
  std::array<const char*,3> ccnames2;
  std::deque<const char*> ccnames3;
  std::forward_list<const char*> ccnames4;
  std::list<const char*> ccnames5;
  std::set<const char*> ccnames6;
  std::unordered_set<const char*> ccnames7;
  std::vector<const char*> ccnames8;
  std::map<string, const char*> ccnames9;
  std::unordered_map<string, const char*> ccnames10;
};

// - - - - - - - - - - - - - - - -

class Note {
  friend class MyClasses;
  int num{1};
  std::string name1{"xxx"};
  std::string* name2{new std::string("yyy")};
  char *value1{nullptr};
  char *value2{strdup("aaa")};
  const char *value3{nullptr};
  const char *value4{"bbb"};
};

class Notes {
public:
  Notes(bool init = false);
private:
  friend class MyClasses;
  Note notes1[3];
  Note* notes2[3];
  shared_ptr<Note> notes3[3];
  unique_ptr<Note> notes4[3];
  
  std::vector<Note> notes5;
  std::vector<Note*> notes6;
  std::vector<shared_ptr<Note>> notes7;
  std::vector<unique_ptr<Note>> notes8;

  std::vector<Note>* notes9;
  std::vector<Note*>* notes10;
  std::vector<shared_ptr<Note>>* notes11;
  std::vector<unique_ptr<Note>>* notes12;
  
  shared_ptr<std::vector<Note>> notes13;
  shared_ptr<std::vector<Note*>> notes14;
  shared_ptr<std::vector<shared_ptr<Note>>> notes15;
  shared_ptr<std::vector<unique_ptr<Note>>> notes16;
  
  unique_ptr<std::vector<Note>> notes17;
  unique_ptr<std::vector<Note*>> notes18;
  unique_ptr<std::vector<shared_ptr<Note>>> notes19;
  unique_ptr<std::vector<unique_ptr<Note>>> notes20;
};
