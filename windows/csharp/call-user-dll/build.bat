@echo off
::https://www.codeproject.com/Articles/552348/C-fCplusinteroppluswithplusDllImport

cl /LD lib.c
::i686-w64-mingw32-gcc -shared -o lib.dll lib.c
csc /platform:x86 main.cs