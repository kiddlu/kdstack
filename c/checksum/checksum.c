#include <stdio.h>

unsigned char checksum (unsigned char *ptr, size_t sz) {
  unsigned char chk = 0;
  while (sz-- != 0)
    chk -= *ptr++;
  return chk;
}

int main(int argc, char* argv[])
{
  unsigned char x[] = "Hello_";
  unsigned char y = checksum (x, 5);
  printf ("Checksum is 0x%02x\n", y);
  x[5] = y;
  y = checksum (x, 6);
  printf ("Checksum test is 0x%02x\n", y);
  return 0;
}
