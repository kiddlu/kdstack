/*
	diag cli tool for Smartisan Debug Bridge Daemon(sdbd)
*/

using System;
using System.Threading;
using System.Text.RegularExpressions;

namespace diag
{
    class Program
    {
		static UInt32 g_QLIB_Connection = 0;
		
        static bool ConnectPhone()
        {
            QMSLPhone.QLIB_TerminateQPSTServer();
     
            QMSLPhone.QLIB_SetLibraryMode(0);

            int waitSecond = 0;
            UInt32 ret;

            while (waitSecond++ < 5)
            {
                g_QLIB_Connection = QMSLPhone.QLIB_ConnectServerWithWait(unchecked((uint)(-1)), 1000);

                if (g_QLIB_Connection != 0)
                {
                    return true;
                }
            }
            return false;
        }

		static bool DisconnectPhone()
        {
            if (g_QLIB_Connection != 0)
            {
                QMSLPhone.QLIB_DisconnectServer(g_QLIB_Connection);
                g_QLIB_Connection = 0;
            }
            return true;
        }

		static byte[] CombineTwoBytes(byte[] first, byte[] second)
		{
			byte[] result = new byte[first.Length + second.Length];
			first.CopyTo(result, 0); 
			second.CopyTo(result, first.Length);
			return result;
		}

        static void Main(string[] args)
        {
			if (args.Length == 0)
			{
				System.Console.WriteLine("Usage: diag unlock");
				return;
			}

			bool bRet=true;

			bRet = ConnectPhone();
			if (!bRet) {
				Console.WriteLine("ConnectPhone... failed");
				return; 
			}

			string strDiagCommand = "unlock";
            short payloadSize = (short)strDiagCommand.Length;
 			byte[] payloadBytes = BitConverter.GetBytes(payloadSize);
	
			byte[] subSysCode = new byte[] {0x4b, 0xfe, 0x00, 0x00};
            byte[] cmdCode = System.Text.Encoding.ASCII.GetBytes(strDiagCommand);

			byte[] requestBytes = CombineTwoBytes(CombineTwoBytes(subSysCode, payloadBytes), cmdCode);

			//Console.WriteLine(System.Text.Encoding.ASCII.GetString(requestBytes));

            short requestSize = (short)requestBytes.Length;
            short responseSize = 1024;
            byte[] responseBytes = new byte[1024];
            byte result = 0;
            int retry = 0;

            while ((result == 0) && (retry < 3))
            {
                if (retry > 0)
					Thread.Sleep(500);
				result = QMSLPhone.QLIB_SendSync(g_QLIB_Connection, requestSize, requestBytes, ref responseSize, responseBytes, 10000);
                retry++;
            }
            if (result == 0)
            {
				Console.WriteLine("QLIB_SendSync... failed");
                return;
            }

		    ushort value = BitConverter.ToUInt16( responseBytes, 4);

            byte[] retBytes = new byte[value];
			for(int i=0; i<value; i++){
				retBytes[i]=responseBytes[i+6];
			}

			//Console.WriteLine(System.Text.Encoding.ASCII.GetString(requestBytes));

			Console.WriteLine(System.Text.Encoding.ASCII.GetString(retBytes));

			bRet = DisconnectPhone();
			return;
        }
    }
}