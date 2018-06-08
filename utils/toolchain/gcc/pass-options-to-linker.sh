#!/bin/sh

code_file=`mktemp --suffix=.c`
exe_file=`mktemp --suffix=.exe`
map_file=`mktemp --suffix=.map`
trap "rm -f $code_file $exe_file $map_file" EXIT

#code
cat <<'CODEEND' > $code_file
#include <stdio.h>

int main (void)
{
  puts ("Hello world!");
  return 0;
}
CODEEND

# -Wl,option
#Pass option as an option to the linker. If option contains commas, it is split into multiple options at the commas. You can use this syntax to pass an argument to the option. For example, -Wl,-Map,output.map passes -Map output.map to the linker. When using the GNU linker, you can also get the same effect with -Wl,-Map=output.map. 

gcc -Wl,-Map=$map_file -g $code_file -o $exe_file
#i686-pc-mingw32-gcc -Wl,-Map=$map_file -g $code_file -o $exe_file
cat $map_file