#include <stdio.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int main()
{
  int a=9;
  int b=MAX(25, 9);
  printf("b is %d\n", b);
}
