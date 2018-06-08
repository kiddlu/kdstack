#!/bin/sh

code_file=`mktemp --suffix=.c`
exe_file=`mktemp --suffix=.exe`
trap "rm -f $code_file $exe_file" EXIT

#code
cat <<'CODEEND' > $code_file
#include <stdio.h>

int main (void)
{
  int i, sum;

  for (i = 1, sum = 0; i <= 10; i++)
    {
      sum += i;
    #ifdef DEBUG
      printf ("sum += %d is %d\n", i, sum);
    #endif
    }
  printf ("total sum is %d\n", sum);

  return 0;
}
CODEEND

# -D name
#Predefine name as a macro, with definition 1. 
# -D name=definition
#The contents of definition are tokenized and processed as if they appeared during translation phase three in a ‘#define’ directive. In particular, the definition will be truncated by embedded newline characters.
#If you are invoking the preprocessor from a shell or shell-like program you may need to use the shell's quoting syntax to protect characters such as spaces that have a meaning in the shell syntax.
# -U name
#Cancel any previous definition of name, either built in or provided with a -D option. 
# -undef
#Do not predefine any system-specific or GCC-specific macros. The standard predefined macros remain defined.

#gcc -DDEBUG $code_file -o $exe_file
gcc -D DEBUG $code_file -o $exe_file

exec $exe_file
