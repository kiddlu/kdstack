#include <windows.h>

extern "C" void WINAPI showMessageA();
extern "C" void WINAPI showMessageB();

int main() {
    showMessageA();
}