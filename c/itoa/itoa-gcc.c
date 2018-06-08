/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.

 */
//http://www.strudel.org.uk/itoa/
char* itoa(int value, char* result, int base) {
  // check that the base if valid
  if (base < 2 || base > 36) { *result = '\0'; return result; }

  char* ptr = result, *ptr1 = result, tmp_char;
  int tmp_value;

  do {
    tmp_value = value;
    value /= base;
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
  } while ( value );

  // Apply negative sign
  if (tmp_value < 0) *ptr++ = '-';
  *ptr-- = '\0';
  while(ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr--= *ptr1;
    *ptr1++ = tmp_char;
  }
  return result;
}

//https://stackoverflow.com/questions/190229/where-is-the-itoa-function-in-linux
//https://stackoverflow.com/questions/8257714/how-to-convert-an-int-to-string-in-c

/* itoa example */
#include <stdio.h>
#include <stdlib.h>

#define ENOUGH ((CHAR_BIT * sizeof(int) - 1) / 3 + 2)

int main ()
{
  int i=-175099;
  char buffer [255];
  itoa (i, buffer, 10);
  printf ("decimal: %s\n", buffer);
  itoa (i, buffer, 16);
  printf ("hexadecimal: %s\n", buffer);
  itoa (i, buffer, 2);
  printf ("binary: %s\n", buffer);

  //char str[ENOUGH];
  //printf ("ENOUGH is %d\n", ENOUGH);
  int length = snprintf( NULL, 0, "%d", i);
  char* str = (char *)malloc( length + 1 );
  snprintf( str, length + 1, "%d", i );
  printf("\nsnprintf is %s\n", str);
  free(str);

  return 0;
}
