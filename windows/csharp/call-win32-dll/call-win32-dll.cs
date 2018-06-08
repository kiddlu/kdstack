using System;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential)]
public class MySystemTime {
    public ushort wYear; 
    public ushort wMonth;
    public ushort wDayOfWeek; 
    public ushort wDay; 
    public ushort wHour; 
    public ushort wMinute; 
    public ushort wSecond; 
    public ushort wMilliseconds; 
}
class Win32API {
    [DllImport("Kernel32.dll")]
    public static extern void GetSystemTime(MySystemTime st);

    [DllImport("user32.dll", CharSet=CharSet.Auto)]
    public static extern int MessageBox(IntPtr hWnd, string text, string caption, int options);
}

public class TestPlatformInvoke
{
    public static void Main()
    {
        MySystemTime sysTime = new MySystemTime();
        Win32API.GetSystemTime(sysTime);

        string dt;
        dt = "System time is: \n" +
              "Year: " + sysTime.wYear + "\n" +
              "Month: " + sysTime.wMonth + "\n" +
              "DayOfWeek: " + sysTime.wDayOfWeek + "\n" +
              "Day: " + sysTime.wDay;
        Win32API.MessageBox(IntPtr.Zero, dt, "Platform Invoke Sample", 0);
		
		
    }
}