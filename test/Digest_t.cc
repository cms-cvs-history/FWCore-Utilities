#include <cassert>
#include <iostream>
#include "FWCore/Utilities/interface/Digest.h"

using namespace std;
using cms::Digest;
using cms::MD5Result;

void testGivenString(std::string const& s)
{
  Digest dig1(s);
  MD5Result r1 = dig1.digest();

  Digest dig2;
  dig2.append(s);
  MD5Result r2 = dig2.digest();
  assert(r1 == r2);

  // The result should be valid *iff* s is non-empty.
  assert(r1.isValid() != s.empty() );
  assert(r1.toString().size() == 32);
  assert(r1.compactForm().size() == 16);
}


void testEmptyString()
{
  std::string e;
  testGivenString(e);

  Digest dig1;
  MD5Result r1 = dig1.digest();

  MD5Result r2;
  assert(r1 == r2);

  assert(! r1.isValid());
}


int main()
{
  Digest dig1;
  dig1.append("hello");
  Digest dig2("hello");

  MD5Result r1 = dig1.digest();
  MD5Result r2 = dig2.digest();

  assert(r1 == r2);
  assert(! (r1 < r2));
  assert(! (r2 < r1));

  assert(r1.toString().size() == 32);

  testGivenString("a");
  testGivenString("{ }");
  testGivenString("abc 123 abc");
  testEmptyString();
  
}
