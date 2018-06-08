//g++ -Wall -o main main.cpp
#include <iostream>
#include <string>

#include <stdio.h>

namespace {
  char *str = "Hello World";
}

namespace base {
  char *str = "base::Hello World";
}

int main()
{
  printf("str is %s\n", str);
  printf("base::str is %s\n", base::str);
  return 0;
}
