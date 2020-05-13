//
//  contact_special_cases.cpp: JsonSerial Example
//
//  JsonSerial: C++ Object Serialization in JSON.
//  (C) Eric Lecolinet 2018 - https://www.telecom-paristech.fr/~elc
//
// NOTES:
// - raw pointers must be properly initialized otherwise JsonSerial will crash
// - std::shared_ptr could be used instead of raw pointers
// - std::unique_ptr can't be used here because objects are pointed by multiple pointers
//

#include <cstdlib>
#include "jsonserial/jsonserial.hpp"  // main JsonSerial header
#include "jsonserial/list.hpp"        // needed for serializing std::list
#include "jsonserial/vector.hpp"      // needed for serializing std::vector
#include "contact_special_cases.hpp"
using namespace std;
using namespace jsonserial;           // JsonSerial namespace

// - - - DECLARE SERIALIZED CLASSES - - - - - - - - - - - - - - - - - - - - - -

// class for declaring serialized classes
class MyClasses : public JsonClasses {
public:
  // this static variable will hold all declarations.
  static const MyClasses instance;
  
  MyClasses() {
    // Phone does NOT have a no-argument constructor => provide a creator function
    defclass<Phone>("Phone", []{return new Phone("","");})
    // MyClasses does NOT have access to Phone variables => use setters/getters
    .member("type", &Phone::setType, &Phone::getType)
    .member("number", &Phone::setNumber, &Phone::getNumber);
    
    // Address is a nested class of Contact => must be fully qualified
    defclass<Contact::Address>("Contact::Address")
    .member("street", &Contact::Address::street)
    .member("city", &Contact::Address::city)
    .member("state", &Contact::Address::state)
    .member("postcode", &Contact::Address::postcode);
    
    defclass<Contact>("Contact")
    .member("firstname", &Contact::firstname)
    .member("lastname", &Contact::lastname)
    .member("gender", &Contact::gender)
    .member("isalive", &Contact::isalive)
    .member("age", &Contact::age)
    .member("address", &Contact::address)
    .member("phones", &Contact::phones)
    .member("mother", &Contact::mother)
    .member("father", &Contact::father)
    .member("partner", &Contact::partner)
    .member("children", &Contact::children);
    
    // abstract class => second argument of defclass() must be nullptr
    defclass<Photo>("Photo", nullptr)
    .member("image", &Photo::image)
    .member("width", &Photo::width)
    .member("height", &Photo::height);
    
    // multiple inheritage => extends<>() for each superclass
    defclass<PhotoContact>("PhotoContact")
    .extends<Contact>()
    .extends<Photo>();
  }
};

// this static variable holds all declarations.
const MyClasses MyClasses::instance;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int main(int argc, char** argv)
{
  // - - - CREATE OBJECTS - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Contact::Address* address =
  new Contact::Address{"21 2nd Street","New York", "NY", "10021-3100"};
  
  Phone* phone = new Phone("home","123 456-7890");
  
  Contact* bob = new Contact("Bob", "Smith", 37, Contact::Male);
  bob->addAddress(address);
  bob->addPhone(phone);
  
  Contact* alice = new Contact("Alice", "Smith", 38, Contact::Female);
  alice->addAddress(address);
  alice->addPhone(phone);
  alice->addPhone(new Phone("office","703 221-2121"));
  
  Contact* laura = new Contact("Laura", "Smith", 10, Contact::Female);
  laura->addAddress(address);
  laura->addPhone(phone);
  laura->addPhone(new Phone("mobile","211 1234-2121"));
  
  Contact* franck = new Contact("Franck", "Smith", 6, Contact::Male);
  franck->addAddress(address);
  franck->addPhone(phone);

  // - - - CREATE SERIALIZER - - - - - - - - - - - - - - - - - - - - - - - - - -

  // js is a serializer for all the classes declared in MyClasses::instance
  JsonSerial js(MyClasses::instance);
  
  string pwd = ::getenv("PWD") + string("/");
  cout << "Files created in directory: " << pwd << endl;
  
  // - - - WRITE ACYCLIC GRAPH - - - - - - - - - - - - - - - - - - - - - - - -
  
  // add links, the graph is ACYCLIC, all nodes can be reached from bob
  bob->addPartner(alice);
  bob->addChild(laura);
  bob->addChild(franck);
  alice->addChild(laura);
  alice->addChild(franck);
  
  // writes bob and related objects
  // - shared objects are duplicated in the JSON file
  if (!js.write(bob, pwd+"bob-simple.json")) return 1;
  
  // - - - POLYMORPHIC OBJECTS - - - - - - - - - - - - - - - - - - - - - - - -
  
  // brad is a PhotoContact, which derives from Contact and Photo
  PhotoContact* brad = new PhotoContact("Brad", "Smith", 12, Contact::Male);
  brad->setImage("brad.png", 75, 50);
  
  bob->addChild(brad);
  alice->addChild(brad);
  
  // writes bob and related objects
  // - the class of brad is stored in the JSON file
  if (!js.write(bob, pwd+"bob-poly.json")) return 1;
  
  // - - - WRITE CYCLIC GRAPH - - - - - - - - - - - - - - - - - - - - - - - - -

  // add links, the graph is CYCLIC, all nodes can be reached from any node
  alice->addPartner(bob);
  franck->addFather(bob);
  franck->addMother(alice);
  laura->addFather(bob);
  laura->addMother(alice);
  brad->addFather(bob);
  brad->addMother(alice);
  
  // CAUTION: needed because the graph is cyclic (infinite loop otherwise)
  // - all objects will have an ID, shared objects won't be duplicated
  js.setSharing(true);
  
  // writes bob and related objects
  // - shared objects are NOT duplicated in the JSON file
  if (!js.write(bob, pwd+"bob-cyclic.json")) return 1;

  // - - - READ AND CREATE COPIES - - - - - - - - - - - - - - - - - - - - - - -

  // don't forget to set pointers to null!
  Contact* bobcopy_simple{nullptr};
  Contact* bobcopy_poly{nullptr};
  Contact* bobcopy_cyclic{nullptr};

  // reads copies from previously saved files
  if (!js.read(bobcopy_simple, pwd+"bob-simple.json")) return 1;
  if (!js.read(bobcopy_poly,   pwd+"bob-poly.json")) return 1;
  if (!js.read(bobcopy_cyclic, pwd+"bob-cyclic.json")) return 1;

  // - - - WRITE THE COPIES AND COMPARE - - - - - - - - - - - - - - - - - - - -

  js.setSharing(false);  // acyclic graphs
  if (!js.write(bobcopy_simple, pwd+"bob-simple2.json")) return 1;
  if (!js.write(bobcopy_poly,   pwd+"bob-poly2.json")) return 1;

  js.setSharing(true);   // cyclic graph
  if (!js.write(bobcopy_cyclic, pwd+"bob-cyclic2.json")) return 1;

  // compare files
  // - files should be identical (except for elements in unordered containers)
  cout << endl;
  system(("diff -q -s " + pwd+"bob-simple.json " + pwd+"bob-simple2.json").c_str());
  cout << endl;
  system(("diff -q -s " + pwd+"bob-poly.json "   + pwd+"bob-poly2.json").c_str());
  cout << endl;
  system(("diff -q -s " + pwd+"bob-cyclic.json " + pwd+"bob-cyclic2.json").c_str());
  cout << endl;

  return 0;
}
