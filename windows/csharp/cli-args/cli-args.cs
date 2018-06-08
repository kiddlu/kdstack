// Sample for the Environment.GetCommandLineArgs method
using System;

class Sample 
{
    public static void Main() 
    {
    Console.WriteLine();
//  Invoke this sample with an arbitrary set of command line arguments.
    String[] arguments = Environment.GetCommandLineArgs();
    Console.WriteLine("GetCommandLineArgs: {0}", String.Join(", ", arguments));
    }
}