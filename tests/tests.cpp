//
//  serial_tests.cpp
//  Created by elc.
//  Copyright © 2017 elc. All rights reserved.
//

#include <chrono>
#include "tests.hpp"
#include "jsonserial/jsonserial.hpp"
#include "jsonserial/array.hpp"
#include "jsonserial/deque.hpp"
#include "jsonserial/forward_list.hpp"
#include "jsonserial/list.hpp"
#include "jsonserial/map.hpp"
#include "jsonserial/unordered_set.hpp"
#include "jsonserial/set.hpp"
#include "jsonserial/unordered_map.hpp"
#include "jsonserial/vector.hpp"
using namespace std;
using namespace jsonserial;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Class for serializing classes
class MyClasses : public JsonClasses {
public:
  static MyClasses instance;
  
  MyClasses();
  
  static PhoneNumber* createPhoneNumber();
  static PhoneNumber* createPhoneNumberMember(Contact&);
  static void contactWasRead(Contact&);
  static void contactWasWritten(const Contact&);
  static void readAge(Contact&, JsonSerial&, const string& value);
  static void writeAge(const Contact&, JsonSerial&);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class Clock {
public:
  void start() {
    t1 = chrono::high_resolution_clock::now();
  }
  
  void stop() {
    t2 = chrono::high_resolution_clock::now();
    chrono::duration<double> time_span(t2 - t1);
    std::cout << "Time: " << time_span.count() << " seconds."<< std::endl;
  }
  
  chrono::steady_clock::time_point t1, t2;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

MyClasses MyClasses::instance;

bool test(const string& filename, const string& filename2,
          int count, bool cyclic_graph, bool show)
{
  cout << "\n*** Test: " << filename << endl;
  Clock clock;
  
  JsonSerial js(MyClasses::instance);

  // plain objects and object pointers are supported.
  Contacts contacts(count, cyclic_graph);
  //ContactsPtr contacts{new Contacts(count, cyclic_graph)};

  // cyclic_graph = will share objects pointed by multiple pointers
  // note that is mode is needed if the graph is cyclic (infinite loop otherwise!)
  js.setSharing(cyclic_graph);
  
  cout << "\n- Writing: " << filename << endl;
  clock.start();
  if (!js.write(contacts, filename)) return false;
  clock.stop();
  
  // show file
  if (show) system(("open " + filename).c_str());

  //Contacts copy;
  ContactsPtr copy;
  
  cout << "\n- Reading: " << filename << endl;
  clock.start();
  if (!js.read(copy, filename)) return false;
  clock.stop();

  cout << "\n- Writing: " << filename2 << endl;
  clock.start();
  if (!js.write(copy, filename2)) return false;
  clock.stop();
  
  // show file
  if (show) system(("open " + filename2).c_str());

  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int main(int argc, char** argv) {  
  string dir = (argc > 1) ? argv[1] : "/tmp/";
  
  int count = 100;

  // test without sharing objects
  test(dir+"contacts.json", dir+"contacts-copy.json", count, false, false);
  
  // test with shared objects + cyclic graph
  test(dir+"contacts-shared.json", dir+"contacts-shared-copy.json", count, true, false);
  return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Myclasses class

static int global_var{55}; // static global variable.

MyClasses::MyClasses()
{
  /* 1) a creator function is needed because PhoneNumber doesn't have a default constructor
   * 2) accessors must be used because MyClasses is not a friend of PhoneNumber
   */
  defclass<PhoneNumber>("PhoneNumber", createPhoneNumber)
  .member("type", &PhoneNumber::setType, &PhoneNumber::getType)
  .member("number", &PhoneNumber::setNumber, &PhoneNumber::getNumber);
  
  defclass<Note>("Note")
  .member("num", &Note::num)
  .member("name1", &Note::name1)
  .member("name2", &Note::name2)
  .member("value1", &Note::value1)
  .member("value2", &Note::value2)
  .member("value3", &Note::value3)
  .member("value4", &Note::value4);

  defclass<Contact::Address>("Contact::Address")
  .member("street", &Contact::Address::street)
  .member("city", &Contact::Address::city)
  .member("state", &Contact::Address::state)
  .member("postcode", &Contact::Address::postcode);
  
  defclass<Contact>("Contact")
  .member("global_var", global_var)           // static global variable (no & symbol)
  .member("static_var", Contact::static_var)  // static class variable (no & symbol)
  
  .member("firstname1", &Contact::firstname1)
  .member("lastname1", &Contact::lastname1)
  .member("firstname2", &Contact::firstname2)
  .member("lastname2", &Contact::lastname2)
  .member("firstname3", &Contact::firstname3)
  .member("lastname3", &Contact::lastname3)
  .member("firstname4", &Contact::firstname4)
  .member("lastname4", &Contact::lastname4)
  .member("firstname5", &Contact::firstname5)
  .member("lastname5", &Contact::lastname5)
  .member("firstname6", &Contact::firstname6)
  .member("lastname6", &Contact::lastname6)
  
  .member("gender", &Contact::gender)
  .member("sex", &Contact::sex)
  .member("isalive", &Contact::isalive)

  //.member("age1", &Contact::age1)
  .member("age1", readAge, writeAge)    // custom read/write functions
  .member("age2", &Contact::age2)
  .member("age3", &Contact::age3)
  .member("age4", &Contact::age4)
  
  .member("address1", &Contact::address1)
  .member("address2", &Contact::address2)
  .member("address3", &Contact::address3)
  .member("address4", &Contact::address4)

  .member("phonenumbers1", &Contact::phonenumbers1)
  .member("phonenumbers2", &Contact::phonenumbers2)
  // with member creator function
  .member("phonenumbers3", &Contact::phonenumbers3, createPhoneNumberMember)
  // with member creator lambda
  .member("phonenumbers4", &Contact::phonenumbers4,
          [](Contact& c) {return new PhoneNumber("","");})
  
  .member("names", &Contact::names)
  .member("notes", &Contact::notes)
  
  .member("mother", &Contact::mother)
  .member("father", &Contact::father)
  .member("partner", &Contact::partner)
  .member("children", &Contact::children)
  
  // called after reading the object
  // can be a function or a lambda or a method of Contact (without first parameter)
  .postread(contactWasRead)
  
  // called after writting the object
  // can be a function or a lambda or a method of Contact (without first parameter)
  .postwrite(contactWasWritten);
  
  defclass<Contacts>("Contacts")
  .member("contacts", &Contacts::contacts);

  // this class is abstract => nullptr as a second argument
  defclass<Photo>("Photo", nullptr)
  .member("image", &Photo::image)
  .member("width", &Photo::width)
  .member("height", &Photo::height);

  // this class has 2 superclasses
  defclass<PhotoContact>("PhotoContact")
  .extends<Contact>()
  .extends<Photo>();
  
  defclass<Names>("Names")
  .member("names1", &Names::names1)
  .member("names2", &Names::names2)
  .member("names3", &Names::names3)
  .member("names4", &Names::names4)
  .member("names5", &Names::names5)
  .member("names6", &Names::names6)
  .member("names7", &Names::names7)
  .member("names8", &Names::names8)
  .member("names9", &Names::names9)
  
  .member("names10", &Names::names10)
  .member("cnames1", &Names::cnames1)
  .member("cnames2", &Names::cnames2)
  .member("cnames3", &Names::cnames3)
  .member("cnames4", &Names::cnames4)
  .member("cnames5", &Names::cnames5)
  .member("cnames6", &Names::cnames6)
  .member("cnames7", &Names::cnames7)
  .member("cnames8", &Names::cnames8)
  .member("cnames9", &Names::cnames9)
  .member("cnames10", &Names::cnames10)
  
  //.member("ccnames1", &Names::ccnames1)
  .member("ccnames2", &Names::ccnames2)
  .member("ccnames3", &Names::ccnames3)
  .member("ccnames4", &Names::ccnames4)
  .member("ccnames5", &Names::ccnames5)
  .member("ccnames6", &Names::ccnames6)
  .member("ccnames7", &Names::ccnames7)
  .member("ccnames8", &Names::ccnames8)
  .member("ccnames9", &Names::ccnames9)
  .member("ccnames10", &Names::ccnames10);
  
  defclass<Notes>("Notes")
  .member("notes1", &Notes::notes1)
  .member("notes2", &Notes::notes2)
  .member("notes3", &Notes::notes3)
  .member("notes4", &Notes::notes4)
  .member("notes5", &Notes::notes5)
  .member("notes6", &Notes::notes6)
  .member("notes7", &Notes::notes7)
  .member("notes8", &Notes::notes8)
  .member("notes9", &Notes::notes9)
  .member("notes10", &Notes::notes10)
  .member("notes11", &Notes::notes11)
  .member("notes12", &Notes::notes12)
  .member("notes13", &Notes::notes13)
  .member("notes14", &Notes::notes14)
  .member("notes15", &Notes::notes15)
  .member("notes16", &Notes::notes16)
  .member("notes17", &Notes::notes17)
  .member("notes18", &Notes::notes18);
}

PhoneNumber * MyClasses::createPhoneNumber() {
  //cout << "createPhoneNumber" << endl;
  return new PhoneNumber("","");
}

PhoneNumber * MyClasses::createPhoneNumberMember(Contact& c) {
  //cout << "createPhoneNumberMember" << endl;
  return new PhoneNumber("","");
}

void MyClasses::contactWasRead(Contact& c) {
  //cout << c.firstname1 << " " << c.lastname1 << " was read" << endl;
}

void MyClasses::contactWasWritten(const Contact& c) {
  //cout << c.firstname1 << " " << c.lastname1 << " was written" << endl;
}

void MyClasses::readAge(Contact& c, JsonSerial& js, const string& val) {
  //cout << "read age: " << val << endl;
  c.age1 = stoi(val);
}

void MyClasses::writeAge(const Contact& c, JsonSerial& js) {
  //cout << "write age: " << c.age1 << endl;
  js.writeMember(c.age1);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Contact class

long Contact::static_var{100};   // static class variable.

Contact::Contact(const string& firstname, const string& lastname,
                 unsigned short age, Gender gender) :
firstname1(firstname), lastname1(lastname),
firstname2(new string(firstname)), lastname2(new string(lastname)),
firstname3(new string(firstname)), lastname3(new string(lastname)),
firstname4(new string(firstname)), lastname4(new string(lastname)),
firstname5(strdup(firstname.c_str())), lastname5(strdup(lastname.c_str())),
firstname6(strdup(firstname.c_str())), lastname6(strdup(lastname.c_str())),
gender(gender),
sex(gender == Gender::Male ? 'M' : 'F'),
age1(age),
age2(new unsigned short(age)),
age3(new unsigned short(age)),
age4(new unsigned short(age)) {
}

Contact::~Contact() {

}

void Contact::addPhone(PhoneNumber* phone, shared_ptr<PhoneNumber> sphone) {
  phonenumbers1[phonecount] = phone;
  phonenumbers2[phonecount] = shared_ptr<PhoneNumber>(sphone);
  phonenumbers3.push_back(phone);
  phonenumbers4.push_back(shared_ptr<PhoneNumber>(sphone));
  phonecount++;
}

void Contact::addPhone(const std::string& type, const std::string& number) {
  addPhone(new PhoneNumber(type, number),
           shared_ptr<PhoneNumber>(new PhoneNumber(type, number)));
}

void Contact::addAddress(Address* address, shared_ptr<Address> saddress) {
  if (address) address1 = *address;
  address2 = address;
  address3 = saddress;
  if (address) address4.reset(new Address(*address));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Contacts::Contacts(int count, bool cycling_graph) {
#if SHARED
  for (int i = 0; i < count; ++i) {
    contacts.push_back(shared_ptr<Contact>(makeFamily("Smith", cycling_graph)));
  }
#else
  for (int i = 0; i < count; ++i) {
    contacts.push_back(makeFamily("Smith", cycling_graph));
  }
#endif
}

ContactPtr Contacts::makeFamily(const string& family_name, bool cycling_graph)
{
  Contact::Address* address{
    new Contact::Address{"21 2nd Street","New York", "NY", "10021-3100"}};
  shared_ptr<Contact::Address> saddress{
    new Contact::Address{"21 2nd Street", "New York", "NY", "10021-3100"}};
  PhoneNumber* homephone{new PhoneNumber("home","212 555-1234")};
  shared_ptr<PhoneNumber> shomephone{new PhoneNumber("home","212 555-1234")};

  ContactPtr laura{new Contact("Laura", family_name, 6, Contact::Gender::Female)};
  laura->addAddress(address, saddress);
  laura->addPhone(homephone, shomephone);
                   
  ContactPtr franck{new Contact("Franck", family_name, 10, Contact::Gender::Male)};
  franck->addAddress(address, saddress);
  franck->addPhone(homephone, shomephone);
  franck->addPhone("mobile","211 1234-2121");
  
  PhotoContactPtr john{new PhotoContact("John", family_name, 38, Contact::Gender::Male)};
  john->addAddress(address, saddress);
  john->addPhone(homephone, shomephone);
  john->addPhone("mobile","123 456-7890");
  john->setImage("john.png", 75, 50);
  john->addChild(franck);
  john->addChild(laura);
  
  PhotoContactPtr bessie{new PhotoContact("Bessie", family_name, 40, Contact::Gender::Female)};
  bessie->addAddress(address, saddress);
  bessie->addPhone(homephone, shomephone);
  bessie->addPhone("office","703 221-2121");
  bessie->addPhone("mobile","456 455-7733");
  bessie->setImage("bessie.png", 50, 50);
  bessie->addChild(franck);
  bessie->addChild(laura);
  bessie->addPartner(john);
  
  bessie->names = new Names(true);
  bessie->notes = new Notes(true);
  
  if (cycling_graph) {
    john->addPartner(bessie);
    franck->addFather(john);
    franck->addMother(bessie);
    laura->addFather(john);
    laura->addMother(bessie);
  }
  return bessie;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Names

Names::Names(bool init) {
  if (init) {
    names1[0] = "aaa";
    names1[1] = "bbb";
    names1[2] = "ccc";
    names2 = {"aaa","bbb","ccc"};
    names3 = {"aaa","bbb","ccc"};
    names4 = {"aaa","bbb","ccc"};
    names5 = {"aaa","bbb","ccc"};
    names6 = {"aaa","bbb","ccc"};
    names7 = {"aaa","bbb","ccc"};
    names8 = {"aaa","bbb","ccc"};
    names9 = {{"xxx","aaa"}, {"yyy","bbb"}, {"zzz","ccc"}};
    names10 = {{"xxx","aaa"}, {"yyy","bbb"}, {"zzz","ccc"}};
    
    cnames1[0] = strdup("aaa");
    cnames1[1] = strdup("bbb");
    cnames1[2] = nullptr;
    cnames2 = {strdup("aaa"), strdup("bbb"), strdup("ccc")};
    cnames3 = {strdup("aaa"), strdup("bbb"), strdup("ccc")};
    cnames4 = {strdup("aaa"), strdup("bbb"), strdup("ccc")};
    cnames5 = {strdup("aaa"), strdup("bbb"), strdup("ccc")};
    cnames6 = {strdup("aaa"), strdup("bbb"), strdup("ccc")};
    cnames7 = {strdup("aaa"), strdup("bbb"), strdup("ccc")};
    cnames8 = {strdup("aaa"), strdup("bbb"), strdup("ccc")};
    cnames9 = {{"xxx",strdup("aaa")}, {"yyy",strdup("bbb")}, {"zzz",strdup("ccc")}};
    cnames10 = {{"xxx",strdup("aaa")}, {"yyy",strdup("bbb")}, {"zzz",strdup("ccc")}};
    
    ccnames1[0] = "aaa";
    ccnames1[1] = "bbb";
    ccnames1[2] = nullptr;
    ccnames2 = {"aaa", "bbb", "ccc"};
    ccnames3 = {"aaa", "bbb", "ccc"};
    ccnames4 = {"aaa", "bbb", "ccc"};
    ccnames5 = {"aaa", "bbb", "ccc"};
    ccnames6 = {"aaa", "bbb", "ccc"};
    ccnames7 = {"aaa", "bbb", "ccc"};
    ccnames8 = {"aaa", "bbb", "ccc"};
    ccnames9 = {{"xxx","aaa"}, {"yyy","bbb"}, {"zzz","ccc"}};
    ccnames10 = {{"xxx","aaa"}, {"yyy","bbb"}, {"zzz","ccc"}};
  }
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Notes

Notes::Notes(bool init) {
  if (init) {
    notes2[0] = new Note();
    notes2[1] = new Note();
    notes2[2] = nullptr;
    
    notes3[0] = shared_ptr<Note>(new Note());
    notes3[1] = shared_ptr<Note>(new Note());

    notes4[0] = unique_ptr<Note>(new Note());
    notes4[1] = unique_ptr<Note>(new Note());
    
    notes5.push_back(Note());
    notes5.push_back(Note());

    notes6.push_back(new Note());
    notes6.push_back(new Note());

    notes7.push_back(make_shared<Note>());
    notes7.push_back(make_shared<Note>());

    notes8.push_back(unique_ptr<Note>(new Note()));
    notes8.push_back(unique_ptr<Note>(new Note()));
    
    notes9 = new std::vector<Note>();
    notes9->push_back(Note());
    notes9->push_back(Note());

    notes10 = new std::vector<Note*>();
    notes10->push_back(new Note());
    notes10->push_back(new Note());
    
    notes11 = new std::vector<shared_ptr<Note>>();
    notes11->push_back(make_shared<Note>());
    notes11->push_back(make_shared<Note>());
    
    notes12 = new std::vector<unique_ptr<Note>>();
    notes12->push_back(unique_ptr<Note>(new Note()));
    notes12->push_back(unique_ptr<Note>(new Note()));
    
    notes13 = make_shared<std::vector<Note>>();
    notes13->push_back(Note());
    notes13->push_back(Note());
    
    notes14 = make_shared<std::vector<Note*>>();
    notes14->push_back(new Note());
    notes14->push_back(new Note());
    
    notes15 = make_shared<std::vector<shared_ptr<Note>>>();
    notes15->push_back(make_shared<Note>());
    notes15->push_back(make_shared<Note>());
    
    notes16 = make_shared<std::vector<unique_ptr<Note>>>();
    notes16->push_back(unique_ptr<Note>(new Note()));
    notes16->push_back(unique_ptr<Note>(new Note()));
    
    notes17 = unique_ptr<std::vector<Note>>(new std::vector<Note>);
    notes17->push_back(Note());
    notes17->push_back(Note());
    
    notes18 = unique_ptr<std::vector<Note*>>(new std::vector<Note*>);
    notes18->push_back(new Note());
    notes18->push_back(new Note());
    
    notes19 = unique_ptr<std::vector<shared_ptr<Note>>>(new std::vector<shared_ptr<Note>>);
    notes19->push_back(make_shared<Note>());
    notes19->push_back(make_shared<Note>());
    
    notes20 = unique_ptr<std::vector<unique_ptr<Note>>>(new std::vector<unique_ptr<Note>>);
    notes20->push_back(unique_ptr<Note>(new Note()));
    notes20->push_back(unique_ptr<Note>(new Note()));
  }
}
