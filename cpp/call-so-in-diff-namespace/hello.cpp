#include <iostream>

#include "hello.h"

Hello::Hello()
{
  std::cout << "Hello,";
}

Hello::~Hello()
{
  std::cout << "world" << std::endl;
}

void Hello::classFunc()
{
  std::cout << "Hello::func" << std::endl;
}

namespace test {
  namespace base {
    void nspFunc() {
      std::cout << "test::base::nspFunc" << std::endl;
    }
  }
}
