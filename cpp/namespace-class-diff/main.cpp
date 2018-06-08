//g++ -Wall -o main main.cpp
#include <iostream>

class Base
{
 public:
  void baseFunc()
  {
    std::cout << "Class" << std::endl;
  }
};

namespace Base2 {
  void baseFunc()
  {
    std::cout << "Namespace" << std::endl;
  }
}

//class And namespace do not have the same type name
int main()
{
  Base base;
  base.baseFunc();

  Base2::baseFunc();

  return 0;
}
