#include <iostream>
#include <string>
#include "cat.hh"

using std::cout;
using std::endl;
using std::string;

int main()
{
  string name = "Felix";
  cout<< "Meet my cat, " << name << "!" <<endl;

  Cat kitty(name);
  kitty.speak();
  return 0;
}
