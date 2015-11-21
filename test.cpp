#include <iostream>
#include "jsmn.hpp"

using namespace std;
using namespace jsmn;

void test_serialize() {
  cout << "Test Serialize" << endl;
  Object o;
  o["a"] = 1.;
  o["b"] = "Hello, Json";
  Array a;
  a.Push(1.);
  a.Push(2);
  a.Push("yo");
  o["c"] = a;
  o["self"] = o;

  cout << serialize(o) << endl;
}

void test_parse() {
  cout << "Test Parse" << endl;
  Object o = parse("../test.json");
  cout << serialize(o) << endl;
}

int main(int argc, char* argv[]) {
  test_parse();
  test_serialize();
}
