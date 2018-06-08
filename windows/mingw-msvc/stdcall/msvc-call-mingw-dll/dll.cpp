#include <windows.h>

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,
    DWORD     fdwReason,
    LPVOID    lpvReserved
)
{
    return TRUE;
}

extern "C" __declspec(dllexport) void WINAPI showMessageA()
{
    MessageBoxA(0, "showMessageA", 0, 0);
}

extern "C" __declspec(dllexport) void WINAPI showMessageB()
{
    MessageBoxA(0, "showMessageB", 0, 0);
}