#!/bin/sh

code_file=`mktemp --suffix=.c`
obj_file=`mktemp --suffix=.o`
trap "rm -f $code_file $obj_file" EXIT

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

# -Wa,option
#Pass option as an option to the assembler. If option contains commas, it is split into multiple options at the commas. 

#Include Local Symbols: -L
#Symbols beginning with system-specific local label prefixes, typically `.L' for ELF systems or `L' for traditional a.out systems, are called local symbols. See Symbol Names. Normally you do not see such symbols when debugging, because they are intended for the use of programs (like compilers) that compose assembler programs, not for your notice. Normally both as and ld discard such symbols, so you do not normally debug with them.
#This option tells as to retain those local symbols in the object file. Usually if you do this you also tell the linker ld to preserve those symbols.

gcc -Wa,-L -c $code_file -o $obj_file
objdump -d $obj_file