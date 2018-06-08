#include <string.h>
#include <stdio.h>

int is_palindrome(char *str)
{
  int length = strlen(str);
  if(str == NULL || length == '0') {
    return 0;
  }

  for(int i = 0; i < length / 2; i++) {
    if(str[i] != str[length - i - 1])
      return 0;
  }
  return 1;
}

int main()
{
  int ret;
  char *str;

  str="aabcdcbaa";
  ret = is_palindrome(str);
  printf("%s is %d\n", str, ret);

  str = "aabbc";
  ret = is_palindrome(str);
  printf("%s is %d\n", str, ret);
}
