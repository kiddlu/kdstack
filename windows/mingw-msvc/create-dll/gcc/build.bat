@echo off
::Windows use lib to link but dll to run

i686-w64-mingw32-gcc -Wall -shared dll.c -o dll.dll -Wl,--output-def,dll.def -Wl,--out-implib,dll.lib -Wl,--export-all-symbols
i686-w64-mingw32-gcc main.c dll.lib -o main