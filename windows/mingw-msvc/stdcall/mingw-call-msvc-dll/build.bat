@echo off
::http://blog.csdn.net/zuishikonghuan/article/details/51918076


cl /MT /c dll.cpp
link /dll dll.obj user32.lib
::eq cl /LD dll.cpp /link user32.lib

::if have a lib for link
i686-w64-mingw32-g++ -mwindows -Wall -static main.cpp dll.lib -o main.exe


::not have a lib, gen one. must modify dll export symble
::dll2def dll.dll > dll.def
::dumpbin dll.dll /exports /out:dll.def
pexports.exe dll.dll  > dll.def


lib /def:dll.def /out:dll.lib
::i686-w64-mingw32-dlltool --dllname dll.dll --def dll.def --output-lib dll.lib

i686-w64-mingw32-g++ -mwindows -Wall -static main.cpp dll.lib -o main.exe