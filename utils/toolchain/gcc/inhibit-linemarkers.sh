#!/bin/sh

code_file=`mktemp --suffix=.c`
pre_file=`mktemp --suffix=.i`
trap "rm -f $code_file $pre_file" EXIT

#code
cat <<'CODEEND' > $code_file
#include <stdio.h>

int main(int argc, void* argv)
{
	printf("Hello World\n");
}
CODEEND

gcc -E -P $code_file -o $pre_file

cat $pre_file
