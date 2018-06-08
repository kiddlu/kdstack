#include <iostream>
#include <string>

#include "hello.h"

int main()
{
  Hello hello;
  hello.classFunc();

  test::base::nspFunc();
  return 0;
}
