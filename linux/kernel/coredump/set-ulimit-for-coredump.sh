#!/bin/bash
set -x
ulimit -c
ulimit -c unlimited

src_file=test.c
exe_file=test.exe

cat <<'SRCEND' > $src_file
#include <stdio.h>
int main()
{
    int *ptr=NULL;
    int a = *(ptr);
    return 0;
}
SRCEND

gcc -g $src_file -o $exe_file
./$exe_file
#gdb -c core $exe_file
gdbgui core $exe_file

rm -f core $exe_file $src_file
