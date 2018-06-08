#!/bin/sh

code_file=`mktemp --suffix=.c`
asm_file=`mktemp --suffix=.s`
trap "rm -f $code_file $asm_file" EXIT

#code
cat <<'CODEEND' > $code_file
#include <stdio.h>

int main(void)
{
	int i;

	for (i = 0; i < 10; i++)
		printf("%d ", i);
	putchar ('\n');

	return 0;
}
CODEEND

# -fverbose-asm
#Put extra commentary information in the generated assembly code to make it more readable.

gcc -fverbose-asm -S $code_file -o $asm_file
cat $asm_file