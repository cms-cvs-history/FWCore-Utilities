#include "FWCore/FWUtilities/interface/Exception.h"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>

int main()
{
  cms::Exception e("BadJuju");

  char array[] = "blue";
  char* ptr = array;

  e << "arghargharghargharghargh " << 3 << ' ' << 1.5 << std::endl << std::string(" boo") << ' ' << array << ' ' << ptr;


  std::string expected = 
    "---- BadJuju BEGIN\n"
    "arghargharghargharghargh 3 1.5\n boo blue blue\n"
    "---- BadJuju END\n";

  std::string result(e.what());
  assert (result == expected);

  // Check to see that the following items compile; their modification
  // of the output is tested elsewhere.
  e << std::setprecision(3);
  e << std::scientific;      
}
