#!/bin/sh

code_file=`mktemp --suffix=.c`
exe_file=`mktemp --suffix=.exe`
trap "rm -f $code_file $exe_file" EXIT

#code
cat <<'CODEEND' > $code_file
#include <pthread.h>

int Global;

void *Thread1(void *x) 
{
	Global = 42;
	return x;
}

int main(void) 
{
	pthread_t t;
	pthread_create(&t, NULL, Thread1, NULL);
	Global = 43;
	pthread_join(t, NULL);
	return Global;
}
CODEEND

# -fsanitize=thread
#Enable ThreadSanitizer, a fast data race detector. Memory access instructions will be instrumented to detect data race bugs. See http://code.google.com/p/thread-sanitizer/ for more details. The run-time behavior can be influenced using the TSAN_OPTIONS environment variable; see https://code.google.com/p/thread-sanitizer/wiki/Flags for a list of supported options. 
# -fsanitize=leak
#Enable LeakSanitizer, a memory leak detector. This option only matters for linking of executables and if neither -fsanitize=address nor -fsanitize=thread is used. In that case it will link the executable against a library that overrides malloc and other allocator functions. See https://code.google.com/p/address-sanitizer/wiki/LeakSanitizer for more details. The run-time behavior can be influenced using the LSAN_OPTIONS environment variable. 

gcc -fsanitize=thread -fPIE -pie -g -o $exe_file $code_file -lpthread
exec $exe_file
