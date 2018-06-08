//from https://github.com/ada3000/DriverStorageCleaner/blob/master/DriverStorageCleaner/Program.cs
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Security.Principal;
using System.Text;

namespace DiskCleaner
{
    class Program
    {
        static void Main(string[] args)
        {
            bool isAdmin = IsUserAdmin();

            if (isAdmin)
            {
                List<string> drvToRemove = GetOemDrivers();

                for (int i = 0; i < drvToRemove.Count; i++)
                {
                    string drv = drvToRemove[i];
                    Console.WriteLine("[ " + (i + 1) + "/" + drvToRemove.Count + " ] Remove driver ... " + drv);

                    RemoveDrv(drv);
                }
            }
            else
                Console.WriteLine("[err] You must be admin!");

            Console.WriteLine("[inf] Press any key to continue ...");

            Console.ReadKey();
        }

        private static void RemoveDrv(string drv)
        {
            ProcessStartInfo procStart = new ProcessStartInfo("pnputil.exe", "-d " + drv)
            {
                CreateNoWindow = true,
                UseShellExecute = false,
                //RedirectStandardOutput = true,
                WindowStyle = ProcessWindowStyle.Hidden
            };

            Process proc = Process.Start(procStart);

            proc.WaitForExit();

            //Console.WriteLine("Remove driver end");
        }

        private static List<string> GetOemDrivers()
        {
            List<string> result = new List<string>();

            Console.WriteLine("Collect third party drivers ...");

            MemoryStream output = new MemoryStream();

            ProcessStartInfo procStart = new ProcessStartInfo("pnputil.exe", "-e")
            {
                CreateNoWindow = true,
                RedirectStandardOutput = true,
                UseShellExecute = false,
                WindowStyle = ProcessWindowStyle.Hidden
            };

            Process util = Process.Start(procStart);

            while (!util.StandardOutput.EndOfStream)
            {
                string data = util.StandardOutput.ReadLine();
                if (data.IndexOf(".inf") > -1)
                    result.Add(data);
            }

            util.WaitForExit();
            Console.WriteLine("Collect third party drivers end, found drivers: " + result.Count);

            return result;
        }

        public static bool IsUserAdmin()
        {
            bool isAdmin;
            try
            {
                WindowsIdentity user = WindowsIdentity.GetCurrent();
                WindowsPrincipal principal = new WindowsPrincipal(user);
                isAdmin = principal.IsInRole(WindowsBuiltInRole.Administrator);
            }
            catch (UnauthorizedAccessException ex)
            {
                isAdmin = false;
            }
            catch (Exception ex)
            {
                isAdmin = false;
            }
            return isAdmin;
        }
    }
}