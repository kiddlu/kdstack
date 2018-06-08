#!/bin/sh

code_file=`mktemp --suffix=.c`
trap "rm -f $code_file" EXIT

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

# -v
#Print (on standard error output) the commands executed to run the stages of compilation. Also print the version number of the compiler driver program and of the preprocessor and the compiler proper. 

# -###
#Like -v except the commands are not executed and arguments are quoted unless they contain only alphanumeric characters or ./-_. This is useful for shell scripts to capture the driver-generated command lines. 

gcc -### $code_file
