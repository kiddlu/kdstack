using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Runtime;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Security.Cryptography.X509Certificates;
using Org.BouncyCastle.Crypto.Prng;
using Org.BouncyCastle.Security;
using Org.BouncyCastle.X509;
using Org.BouncyCastle.Utilities;
using Org.BouncyCastle.Math;
using Org.BouncyCastle.Asn1.X509;
using Org.BouncyCastle.Crypto;
using Org.BouncyCastle.Crypto.Generators;
using Org.BouncyCastle.Asn1.Pkcs;
using Org.BouncyCastle.Pkcs;
using Org.BouncyCastle.Asn1;
using Org.BouncyCastle.OpenSsl;
using Org.BouncyCastle.Crypto.Parameters;
using Org.BouncyCastle.X509.Extension;

//csc.exe /t:exe /r:BouncyCastle.Crypto.dll catroot.cs
namespace Catroot {
    class Program {
        static int Main(string[] args) {
			if (args.Length == 0 || !File.Exists(args[0]))
			{
				
				System.Console.WriteLine("Usage: catroot cer-file");
				return 1;
			}
            var cerPath = Path.GetFullPath(args[0]);

            X509Certificate2 x509 = new X509Certificate2(cerPath);
            // Add CA certificate to Root store
            X509Store store = new X509Store(StoreName.Root, StoreLocation.LocalMachine);
            store.Open(OpenFlags.ReadWrite);
            store.Add(x509);
            store.Close();

            store = new X509Store(StoreName.TrustedPublisher, StoreLocation.LocalMachine);
            store.Open(OpenFlags.ReadWrite);
            store.Add(x509);
            store.Close();
			return 0;
        }
    }
}