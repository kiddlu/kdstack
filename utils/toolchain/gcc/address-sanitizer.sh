#!/bin/sh

code_file=`mktemp --suffix=.c`
exe_file=`mktemp --suffix=.exe`
trap "rm -f $code_file $exe_file" EXIT

#code
cat <<'CODEEND' > $code_file
#include <stdio.h>

int main(void)
{
	int a[3] = {0};
	a[3] = 1;
	
	printf("%d\n", a[3]);
	return 0;
}
CODEEND

# -fsanitize=address
#Enable AddressSanitizer, a fast memory error detector. Memory access instructions will be instrumented to detect out-of-bounds and use-after-free bugs. See http://code.google.com/p/address-sanitizer/ for more details. The run-time behavior can be influenced using the ASAN_OPTIONS environment variable; see https://code.google.com/p/address-sanitizer/wiki/Flags#Run-time_flags for a list of supported options. 
# -fsanitize=kernel-address
#Enable AddressSanitizer for Linux kernel. See http://code.google.com/p/address-sanitizer/wiki/AddressSanitizerForKernel for more details. 

gcc -fsanitize=address -g $code_file -o $exe_file
exec $exe_file