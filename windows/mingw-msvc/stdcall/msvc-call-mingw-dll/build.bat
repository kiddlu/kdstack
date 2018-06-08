@echo off
::http://blog.csdn.net/zuishikonghuan/article/details/51918076
::with @ dll
i686-w64-mingw32-g++ -mwindows -Wall -static -shared dll.cpp -o dll.dll -Wl,--output-def,dll.def,--export-all-symbols

::must use msvc-lib and def to export dll.lib for link
lib /machine:i386 /def:dll.def
cl /MT /c main.cpp
link main.obj dll.lib


::without @ dll
::with @ dll
i686-w64-mingw32-g++ -mwindows -Wall -static -shared dll.cpp -o dll2.dll -Wl,--output-def,dll2.def,--export-all-symbols
i686-w64-mingw32-g++ -mwindows -Wall -static -shared dll.cpp -o dll2.dll -Wl,-kill-at

::must use msvc-lib and def to export dll.lib for link
lib /machine:i386 /def:dll2.def
cl /MT /c main.cpp /Fomain2.obj
link main2.obj dll2.lib


::one word, def must with @, dll can without @