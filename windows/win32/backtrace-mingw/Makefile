CC = $(CROSS_COMPILE)gcc

.PHONY: all clean

all : backtrace.dll test.exe

backtrace.dll : backtrace.c
	$(CC) -O2 -shared -Wall -o $@ $^ -lbfd -lintl -liberty -limagehlp

test.exe : test.c
	$(CC) -g -Wall -o $@ $^

clean :
	rm -rf backtrace.dll test.exe
