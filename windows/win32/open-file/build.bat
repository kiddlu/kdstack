gcc main.c -o main -D UNICODE -D _UNICODE -lcomdlg32

::g++ main.cpp psnr.cpp -o main -D UNICODE -D _UNICODE -lcomdlg32 ^
::-lopencv_core248 -lopencv_highgui248 ^
::-I../../../../3rdparty/opencv/include -L../../../../3rdparty/opencv/lib/Release