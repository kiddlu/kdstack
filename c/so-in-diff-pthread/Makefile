all: libprint.dll main.exe

main.exe: main.c libprint.dll
	gcc -L. -lprint -o $@ $<

libprint.dll: print.c
	gcc -fPIC -shared -Wall -o $@ $^

clean:
	rm libprint.dll main.exe
