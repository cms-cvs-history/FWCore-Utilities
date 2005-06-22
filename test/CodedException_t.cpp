
#include "FWCore/FWUtilities/interface/CodedException.h"

#include <iostream>
#include <string>
#include <iomanip>

//using namespace cms;
using namespace std;

namespace edmtest
{

  // This is the list of error codes we shall use.
  enum ToyErrorCodes
    {
      Bad=3,
      Worse,
      Horrific
    };


  // This is the kind of exception we shall throw.
  // I was confused at first, and thought we were supposed to use
  //     edm::CodedException<ToyErrorCodes>
  typedef edm::CodedException<ToyErrorCodes> ToyException;

}

// we must write this specialization.  it is somewhat awkward
// because of the edm namespace
namespace edm {
  template <> void edmtest::ToyException::loadTable()
  {
    EDM_MAP_ENTRY(edmtest,Bad);
    EDM_MAP_ENTRY(edmtest,Worse);
    EDM_MAP_ENTRY(edmtest,Horrific);
  }
}

struct Thing
{
  Thing():x() { }
  explicit Thing(int xx):x(xx) { }
  int x;
};

ostream& operator<<(ostream& os, const Thing& t)
{
  os << "Thing(" << t.x << ")";
  return os;
}

void simple()
{
  edmtest::ToyException h(edmtest::Horrific);
}

void func3()
{
  double d = 1.11111;
  float f = 2.22222;
  unsigned long l = std::numeric_limits<unsigned long>::max();
  std::string s("a string");
  char* c1 = "a nonconst pointer";
  char c2[] = "a c-style array";
  Thing thing(4);

  throw edmtest::ToyException(edmtest::Horrific)
    << "This is just a test: \n"
    << "double: " << d << "\n"
    << "float:  " << f << "\n"
    << "ulong:  " << l << "\n"
    << "string: " << s << "\n"
    << "char*:  " << c1 << "\n"
    << "char[]: " << c2 << "\n"
    << "Thing:  " << thing << "\n"
    << endl
    << "double: " << scientific << d << "\n"
    << "float:  " << setprecision(2) << f << "\n"
    << "ulong:  " << l << "\n"
    << "string: " << s << "\n"
    << "char*:  " << setfill('.') << setw(20) << c1 << "\n"
    << "char[]: " << c2 << "\n"
    << "Thing:  " << thing
    << endl;
}

void func2()
{
  func3();
}

void func1()
{
  try 
    {
      func2();
    }
  catch (cms::Exception& e)
    {
      throw cms::Exception("Worse","In func2",e) << "Gave up";
    }
  
}

const char answer[] = 
  "---- Worse BEGIN\n"
  "In func2\n"
  "---- Horrific BEGIN\n"
  "This is just a test: \n" 
  "double: 1.11111\n"
  "float:  2.22222\n"
  "ulong:  4294967295\n"
  "string: a string\n"
  "char*:  a nonconst pointer\n"
  "char[]: a c-style array\n"
  "Thing:  Thing(4)\n"
  "\n"
  "double: 1.111110e+00\n"
  "float:  2.22e+00\n"
  "ulong:  4294967295\n"
  "string: a string\n"
  "char*:  ..a nonconst pointer\n"
  "char[]: a c-style array\n"
  "Thing:  Thing(4)\n"
  "---- Horrific END\n"
  "Gave up\n"
  "---- Worse END\n"
  ;

const char* correct[] = { "Worse","Horrific" };

int main()
{
  try
    {
      func1();
    }
  catch (cms::Exception& e)
    {
      cerr << "*** main caught Exception, output is ***\n"
	   << "(" << e.what() << ")"
	   << "*** After exception output ***"
	   << endl;

      cerr << "\nCategory name list:\n";

      if(e.what() != answer)
	{
	  cerr << "not right answer\n(" << answer << ")\n"
	       << endl;
	  abort();
	}

      cms::Exception::CategoryList::const_iterator i(e.history().begin()),
	b(e.history().end());

      if(e.history().size() !=2) abort();

      for(int j=0;i!=b;++i,++j)
	{
	  cout << "  " << *i << "\n";
	  if(*i != correct[j])
	    { cerr << "bad category " << *i << endl; abort(); }
	}
    }
  return 0;
}
