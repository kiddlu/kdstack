using System; // Console
using System.Runtime.InteropServices; // DllImport

class App
{
    [DllImport("lib.dll", CallingConvention = CallingConvention.Cdecl)]
    extern static int next(int n);

    static void Main()
    {
        Console.WriteLine(next(0));
    }
}