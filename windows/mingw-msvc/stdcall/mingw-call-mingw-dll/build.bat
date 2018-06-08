@echo off
::http://blog.csdn.net/zuishikonghuan/article/details/51918076

i686-w64-mingw32-g++ -mwindows -Wall -static -shared dll.cpp -o dll.dll -Wl,--output-def,dll.def -Wl,--out-implib,dll.lib -Wl,--export-all-symbols
i686-w64-mingw32-g++ -mwindows -static main.cpp -o main -ldll -L.


::i686-w64-mingw32-g++ -mwindows -Wall -static -shared dll.cpp -o dll2.dll -Wl,--output-def,dll2.def,--out-implib,dll2.lib,--export-all-symbols,-kill-at

i686-w64-mingw32-g++ -mwindows -Wall -static -shared dll.cpp -o dll2.dll -Wl,--output-def,dll2.def
i686-w64-mingw32-g++ -mwindows -Wall -static -shared dll.cpp -o dll2.dll -Wl,-kill-at
:: use first dll2.def and second dll2.dll export libdll2.a
i686-w64-mingw32-dlltool --kill-at -d dll2.def --dllname dll2.dll -l libdll2.a
i686-w64-mingw32-g++ -mwindows -static main.cpp -o main2 -ldll2 -L.