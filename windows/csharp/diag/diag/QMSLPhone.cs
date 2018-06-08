using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace diag
{
    class QMSLPhone
    {
//#if (DEBUG)
//        const string qmslDllName = "QMSL_MSVC10D.dll";
//#else
        const string qmslDllName = "QMSL_MSVC10R.dll";
//#endif
        /// <summary>
        /// Following section is for software download methods which are going through the QMSL, and not using the qpst.
        /// </summary>
        /// <param name="handle"></param>
        /// <param name="msgPtr"></param>
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void QPHONEMSCBSWDownloadHandlerCallBack(UInt32 handle, IntPtr msgPtr, UInt16 iMsgSize);//we probably do not need the handle here.

        #region Connection Functions

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern void QLIB_SetLibraryMode(byte useQPST);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern UInt32 QLIB_ConnectServer(uint comPort);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern UInt32 QLIB_ConnectServerWithWait(uint comPort, ulong wait_ms);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        static extern UInt32 QLIB_QPHONEMS_ConnectServer_Sahara(uint ComPort, ref ulong version, ref ulong serialno, ref ulong msmid, StringBuilder pkhash, byte getInfo, int mode, ulong timeout, QPHONEMSCBSWDownloadHandlerCallBack pSWDLCallBack);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_DisconnectServer_Sahara(UInt32 hResourceContext);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern UInt32 QLIB_QPHONEMS_ConnectServer_FireHose(uint ComPort, QPHONEMSCBSWDownloadHandlerCallBack pSWDLCallBack);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern UInt32 QLIB_QPHONEMS_ConnectServerSimple_FireHose(QPHONEMSCBSWDownloadHandlerCallBack pSWDLCallBack);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_GetAvailablePhonesPortList(
            ref ushort NumPorts, ushort[] PortList,
            ushort NumIgnorePorts, ushort[] IgnorePortList);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_GetComPortNumber(UInt32 hResourceContext, ushort[] physicalPort);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_IsPhoneConnected(UInt32 hResourceContext);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern void QLIB_SetTargetType(byte targetType);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_DisconnectServer_FireHose(UInt32 hResourceContext);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_DisconnectServer(UInt32 hResourceContext);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_DisconnectAllServers();

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern void QLIB_TerminateQPSTServer();


        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_DisconnectAllServers(UInt32 hResourceContext);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern UInt32 QLIB_ConnectServer_UserDefinedTransport(
        IntPtr hUserHandle,
        IntPtr pUserDefinedSend,
        IntPtr pUserDefinedReceive,
        IntPtr pUserDefinedFlushRx,
        char bQMSL_HandlesTxHDLC,
        char bQMSL_HandlesRxHDLC);
        #endregion

        #region "Reset"
        public const int _MODE_RESET_F = 2;
        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_DIAG_CONTROL_F(UInt32 hResourceContext, int mode);
        #endregion

        #region QMSL_Functions_Imported

        /// <summary>
        /// Send Sync funtion. 
        /// </summary>
        /// <param name="hResourceContext"></param>
        /// <param name="requestSize">size of request buffer</param>
        /// <param name="requestBytes">byte buffer containing the diag cmd to be sent</param>
        /// <param name="responseSize">size of the response got back from the phone</param>
        /// <param name="responseBytes">byte buffer containing the diag cmd response</param>
        /// <param name="timeout">max time to wait for response from the phone</param>
        /// <returns>True =  diag cmd successfully received and executed by the phone, False = diag cmd execution failure</returns>
        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_SendSync(uint hResourceContext,
            short requestSize,
            byte[] requestBytes,
            ref short responseSize,
            byte[] responseBytes,
            ulong timeout
            );
        #endregion

        #region "Read or Write NV"
        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_DIAG_NV_READ_F(UInt32 hResourceContext, ushort itemID, byte[] itemData, int length, ref ushort status);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_DIAG_NV_READ_EXT_F(UInt32 hResourceContext, ushort iItemID, byte[] pItemData, ushort iContextID, int iLength, ref ushort iStatus);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_DIAG_NV_WRITE_F(UInt32 hResourceContext, ushort itemID, byte[] itemData, int length, ref ushort status);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_DIAG_NV_WRITE_EXT_F(UInt32 hResourceContext, ushort iItemID, byte[] pItemData, ushort iContextID, int iLength, ref ushort iStatus);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_DIAG_NV_READ_ARRAY_F(UInt32 hResourceContext, ushort iItemID, int iArrayIndex, byte[] pItemData, int iLength, ref ushort iStatus);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_DIAG_NV_WRITE_ARRAY_F(UInt32 hResourceContext, ushort iItemID, int iArrayIndex, byte[] pItemData, int iLength, ref ushort iStatus);
        #endregion

        #region Function to write the NVs loaded in memory to mobile, qcn or xml

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_DownloadQcnFile(UInt32 hResourceContext, string sQCN_Path, string sSPC);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_DownloadQcnFile_V2(UInt32 hResourceContext, string sFileName, string sSPC);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_UploadQcnFile(UInt32 hResourceContext, string sQCN_Path, string sSPC,
        bool bAutoRestore, bool bSkipReset, bool bAllowEsnMismatch, bool bIgnorePhoneModel);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_UploadQcnFile_V2(UInt32 hResourceContext, string sFileName, string sSPC, bool bAllowEsnMismatch);

        [DllImport(qmslDllName, CharSet = CharSet.Auto, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_GetLastSoftwareDownloadErrorInfo
        (UInt32 hResourceContext, byte[] bErrorOccurred, sbyte[] sErrorMessage, int iMaxStringLength);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_NV_LoadNVsFromQCN(UInt32 hResourceContext, string sQCN_Path, ref int iNumOfNVItemValuesLoaded,
            ref int iResultCode);


        //adding the methods for downloading and uploading the QCN files to and from phone.
        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_NV_LoadNVDataTypeDictinary(UInt32 hResourceContext, string sNV_Def_XML_Path, ref Int32 piNumNVDefLoaded, ref Int32 piResultCode);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_NV_LoadNVDefinitionFile(UInt32 hResourceContext, string sNV_Def_XML_Path, ref Int32 piNumNVDefLoaded, ref Int32 piResultCode);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_NV_ClearNVDefinition(UInt32 hResourceContext);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_NV_LoadNVsFromMobile(UInt32 hResourceContext, ref Int32 iNumOfNVItemValuesLoaded, ref Int32 iResultCode);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_NV_WriteNVsToQCN(UInt32 hResourceContext, string sQCN_Path, ref Int32 iResultCode);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_NV_WriteNVsToMobile(UInt32 hResourceContext, ref Int32 iResultCode);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_NV_WriteNVsToSource(UInt32 hResourceContext, string sSource_Path, ref Int32 iResultCode);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_NV_LoadReadFilter(UInt32 hResourceContext, string sFilter_XML_Path, ref Int32 iResultCode);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_NV_LoadWriteFilter(UInt32 hResourceContext, string sFilter_XML_Path, ref Int32 iResultCode);
     
        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_NV_SetTargetSupportMultiSIM(UInt32 handle, bool gTargetSupportMultiSim);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_NV_SetCurrentSubscriptionIndex(UInt32 handle, int subscriptionIndex);

        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_BackupNVFromMobileToQCN(UInt32 hResourceContext, string sQCN_Path, ref Int32 iResultCode);
        #endregion

        // QMSL specifies ulong for timeout data type, but when C# marshalls calls, the pointer for Status gets mucked up.
        // Expect data type lengths for ulong in C++ do not match those in C#.  Using uint solves this issue.  
        [DllImport(qmslDllName, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte QLIB_EFS2_SyncWithWait(UInt32 hResourceContext, ref byte FilePath, uint SyncTimeOut, ref byte Status);

    }
}
