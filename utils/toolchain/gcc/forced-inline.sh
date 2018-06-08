#!/bin/sh

code_file=`mktemp --suffix=.c`
obj_file=`mktemp --suffix=.o`
trap "rm -f $code_file $obj_file" EXIT

#code
cat <<'CODEEND' > $code_file
#include <stdio.h>

#if defined(__GNUC__)
#define FORCEDINLINE  __attribute__((always_inline))
#else 
#define FORCEDINLINE
#endif

static inline int add(int a,int b)
{
	return a+b;
}

int main()
{
	int a=9, b=1;
	int sum=add(a, b);
	printf("sum is %d", sum);
}
CODEEND

gcc -c $code_file -o $obj_file
objdump  -d $obj_file
