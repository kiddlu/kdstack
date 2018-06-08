// autolink.cpp : 定义控制台应用程序的入口点。

#include "stdafx.h"
#include "wlanapi.h"
#include <atlconv.h>

using namespace std;

//#define WIN7_ONLY

#define STR_MUTE_AUTOLINK		_T("TP-LINK-NIC-AUTOLINK")
#define STR_MUTE_UTILITY		_T("TP-LINK-NIC-UTILITY")

HANDLE hClientHandle = NULL;
DWORD dwError = ERROR_SUCCESS;
GUID guid;

#ifdef WIN7_ONLY
#define CHECK_INTERVAL		500 /* ms */
#define CONNECTION_DELAY	1500 /* ms */
#else
#define CHECK_INTERVAL		500 /* ms */
#define CONNECTION_DELAY	5000 /* ms */
#endif

#if 0
BOOL LPCTSR2GUID(LPCTSTR szGUID, GUID &Guid)
{  
  memset(&Guid, 0, sizeof(Guid));  
  int nRet =   
    _stscanf(szGUID,  
      _T("%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x"),  
      &Guid.Data1,   
      &Guid.Data2,  
      &Guid.Data3,  
      &Guid.Data4[0], &Guid.Data4[1],  
      &Guid.Data4[2], &Guid.Data4[3], &Guid.Data4[4], &Guid.Data4[5], &Guid.Data4[6], &Guid.Data4[7]  
      );  
  return (nRet == 11)? TRUE : FALSE;  
}  
  
LPCTSTR GUID2LPCTS(GUID Guid)  
{  
  static TCHAR szBuf[65] = {0};    
  _sntprintf(szBuf, 64, _T("%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X"),  
    Guid.Data1,   
    Guid.Data2,  
    Guid.Data3,  
    Guid.Data4[0], Guid.Data4[1],  
    Guid.Data4[2], Guid.Data4[3], Guid.Data4[4], Guid.Data4[5], Guid.Data4[6], Guid.Data4[7]  
    );  
  return szBuf;  
}
#endif

int StringToHexString(PSTR pHexString, DWORD bufferSize, LPCSTR pString, DWORD len)
{
	if (pString == NULL || pHexString == NULL)
	{
		return FALSE;
	}

	for(DWORD i = 0; i < len; i++)
	{
		if (bufferSize < 2 * i + 2)
		{
			return FALSE;
		}

		if ( -1 == sprintf_s(
							pHexString + 2 * i, 
							bufferSize - 2 * i,
							"%02x", 
							(UCHAR)pString[i]))/* modify for CHAR-> UCHAR, by jimt 2010-06-13. */
		{
			return FALSE;
		}
	}

	return TRUE;
}



int Usage()
{
#define VERSION_MAJOR "3"
#define VERSION_MINOR "0"

	char* usage = 
		"Usage:\r\n"
		" NAME\r\n"
		"  autolink - a tool to setup wireless link between adapter and APs.\r\n"
		"\r\n"
		" SYNOPSIS\r\n"
		"  autolink [ -a adapter] [ -s signal] [ -n ssid ]  \\\r\n"
		"      [ -r src_ipaddr ] [ -p src_macaddr] [ -i dst_ipaddr ] [ -e dst_macaddr ] \r\n"
		"  autolink -h \r\n"
		"\r\n"
		" DESCRIPTION\r\n"
		"  autolink - will control adapter to scan APs and connect automatically\r\n"
		"    to one of them which has the strongest signal. If connection is lost,\r\n"
		"    it will control the adapter to rescan again. Only non-security link\r\n"
		"    is supported by now. Requires Windows 7.\r\n"
		"\r\n"
		" PARAMETER\r\n"
		"  -a select adapter, adapter index to autolink.\r\n"
		"    If not set, PC will select adapter 0 to connect.\r\n"			
		"  -s signal, signal level to filter out.\r\n"
		"    If AP's signal strength is below it, adapter will not connect to the AP.\r\n"	
		"  -n ssid, ssid prefix to filter out.\r\n"
		"    If AP's SSID is not prefixed by it, adapter will not connect to the AP.\r\n"
		"  -r src_ipaddr, wireless LAN PC's IP address.\r\n"
		"    If adapter fails to PING to the address, connection is assumed to be lost.\r\n"
		"  -p src_macaddr, wireless LAN PC's MAC address.\r\n"
		"    In case of duplicate IP address behind wired LAN, it uses 'MAC binding' to\r\n"
		"    increase PING accuracy.\r\n"
		"  -i dst_ipaddr, wired LAN PC's IP address.\r\n"
		"    If adapter fails to PING to the address, connection is assumed to be lost.\r\n"
		"  -e dst_macaddr, wired LAN PC's MAC address.\r\n"
		"    In case of duplicate IP address behind wired LAN, it uses 'MAC binding' to\r\n"
		"    increase PING accuracy.\r\n"
		"  -h, print this help message. \r\n"
		"\r\n"
		" VERSION\r\n"
		"  "VERSION_MAJOR"."VERSION_MINOR", "__DATE__" "__TIME__"\r\n"
		"\r\n"
		" AUTHOR\r\n"
		"  huangqingjia@proware.net, huangxiaolu@tp-link.net\r\n"
		"\r\n";

	printf(usage);

	return 0;
}

int Param_parse(int argc, 
				char* argv[],
				DWORD* adapter_nr,
				DWORD* signal_level,
				char** ssid_prefix,
				char** src_ipaddr,
				char** src_macaddr,
				char** dst_ipaddr, 
				char** dst_macaddr)
{
	int i;

	if(argc>=2 && !strcmp(argv[1], "-h"))
	{
		Usage();
		return -1;
	}

	*adapter_nr = 0;
	*signal_level = 0;
	*ssid_prefix = NULL;
	*src_ipaddr = NULL;
	*src_macaddr = NULL;
	*dst_ipaddr = NULL;
	*dst_macaddr = NULL;

	for(i=1; i<argc; i+=2)
	{
		if(!strcmp(argv[i], "-a"))
		{
			sscanf((const char*)argv[i+1], "%d", adapter_nr);

			printf("Adapter Number %d\r\n", *adapter_nr);
		}
		else if(!strcmp(argv[i], "-s"))
		{
			sscanf((const char*)argv[i+1], "%d", signal_level);

			printf("Signal Level %d\r\n", *signal_level);
		}
		else if(!strcmp(argv[i], "-n"))
		{
			*ssid_prefix = (char*)argv[i+1];

			printf("Ssid Prefix %s\r\n", *ssid_prefix);
		}
		else if(!strcmp(argv[i], "-r"))
		{
			*src_ipaddr = (char*)argv[i+1];
			printf("Src Ip Address %s\r\n", *src_ipaddr);
		}
		else if(!strcmp(argv[i], "-p"))
		{
			*src_macaddr = (char*)argv[i+1];
			printf("Src Mac Address %s\r\n", *src_macaddr);
		}
		else if(!strcmp(argv[i], "-i"))
		{
			*dst_ipaddr = (char*)argv[i+1];
			printf("Dst Ip Address %s\r\n", *dst_ipaddr);
		}
		else if(!strcmp(argv[i], "-e"))
		{
			*dst_macaddr = (char*)argv[i+1];
			printf("Dst Mac Address %s\r\n", *dst_macaddr);
		}
		else
		{
			Usage();
			return -1;
		}
	}

	printf("\r\n");

	return 0;
}

#define MAX_SRC_PING_TRY	5
#define MAX_SRC_PING_DELAY	100 /* ms */

#define MAX_DST_PING_TRY	3
#define MAX_DST_PING_DELAY	160 /* ms */

int Connection_status(
			char* src_ipaddr, 
			char* src_macaddr, 
			char* dst_ipaddr, 
			char* dst_macaddr)
{
	int ret;
	char cmd[256];
	int i;

	static int conn_state = FALSE;

	WLAN_INTERFACE_STATE *pIntfState;
	void *p = NULL;
	DWORD size;
	
	dwError = WlanQueryInterface(hClientHandle, &guid, wlan_intf_opcode_interface_state,
				NULL, &size, &p, NULL);

	pIntfState = (WLAN_INTERFACE_STATE *)p;

	if(dwError != ERROR_SUCCESS)
	{
		printf("Error: WlanQueryInterface %d\r\n", dwError);
		conn_state = FALSE;
		return conn_state;
	}
	else if(*pIntfState != wlan_interface_state_connected)
	{
		/* physically error */
		printf("Connection lost. (Physically error)\r\n\r\n");
		conn_state = FALSE;
		return conn_state;
	}

	if(!conn_state && src_ipaddr)
	{	
		if(!conn_state && src_macaddr)
		{
			/* src mac binding */
			sprintf(cmd, "arp -s %s %s", src_ipaddr, src_macaddr);
			ret = system(cmd);
		}

		for(i=0; i<MAX_SRC_PING_TRY; i++)
		{
			sprintf(cmd, "ping %s -n 1 -w %d > nul", src_ipaddr, MAX_SRC_PING_DELAY);
			ret = system(cmd);
			if(!ret)
			{
				/* ping ok */
				break;
			}
		}

		if(i>=MAX_SRC_PING_TRY)
		{
			/* ping fail */
			printf("Connection lost. (All Src Ping fail)\r\n\r\n");
			conn_state = FALSE;
			return conn_state;
		}
	}

	if(dst_ipaddr)
	{
		if(!conn_state && dst_macaddr)
		{
			/* dst mac binding */
			sprintf(cmd, "arp -s %s %s", dst_ipaddr, dst_macaddr);
			ret = system(cmd);
		}

		for(i=0; i<MAX_DST_PING_TRY; i++)
		{
			sprintf(cmd, "ping %s -n 1 -w %d > nul", dst_ipaddr, MAX_DST_PING_DELAY);
			ret = system(cmd);

			if(!ret)
			{
				/* ping ok */
				break;
			}
		}

		if(i>=MAX_DST_PING_TRY)
		{
			/* ping fail */
			printf("Connection lost. (All Dst Ping fail)\r\n\r\n");
			conn_state = FALSE;
			return conn_state;
		}
	}
	
	if(!conn_state)
	{
		printf("Connection ok\r\n");
	}

	conn_state = TRUE;
	return conn_state;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	DWORD signal_level = 0;
	DWORD adapter_nr = 0;
	DWORD retCode = 0;

	char* ssid_prefix;
	char* src_ipaddr;
	char* src_macaddr;
	char* dst_ipaddr;
	char* dst_macaddr;

	DWORD dwNegotiatedVersion;
	PWLAN_INTERFACE_INFO_LIST pInterfaceList = NULL;
	PWLAN_AVAILABLE_NETWORK_LIST pAvailableNetworkList = NULL;
	PWLAN_AVAILABLE_NETWORK pBssEntry = NULL;

	WLAN_CONNECTION_PARAMETERS wlanConnPara;
	DOT11_SSID lastConnSsid;

	DWORD  index;
	DWORD  curSignalQuality, curIndex;
	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		return 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。什么都不做。
	}

	hClientHandle = CreateMutex(NULL, TRUE, STR_MUTE_AUTOLINK);

	if ( GetLastError() == ERROR_ALREADY_EXISTS)
	{
		printf("More than one instance detected.\r\n"); 
		return -1;
	}

	hClientHandle = CreateMutex(NULL, TRUE, STR_MUTE_UTILITY);

	if ( GetLastError() == ERROR_ALREADY_EXISTS)
	{
		printf("Shut down TP-LINK utility please.\r\n"); 
		return -1;
	}

	if(Param_parse(
		argc,
		argv,
		&adapter_nr,
		&signal_level, 
		&ssid_prefix, 
		&src_ipaddr,
		&src_macaddr,
		&dst_ipaddr,
		&dst_macaddr))
	{
		printf("Param parse error, exit\r\n");
		return -1;
	}

	dwError = WlanOpenHandle(WLAN_API_VERSION_2_0, NULL,
							&dwNegotiatedVersion, &hClientHandle);
	if(dwError != ERROR_SUCCESS)
	{
		printf("Error: WlanOpenHandle %d, No Wlan Interface?\r\n", dwError);
		return -1;
	}

	/* 程序主循环 */
	while(TRUE)
	{
		/* 枚举无线网卡 */
		dwError = WlanEnumInterfaces(hClientHandle, NULL, &pInterfaceList);
		if(dwError != ERROR_SUCCESS)
		{
			printf("Error: WlanEnumInterfaces %d, No Wlan Interface?\r\n", dwError);
			return -1;
		}

		printf("%d Wlan Adapter(s)\r\n", pInterfaceList->dwNumberOfItems);
		for(index = 0; index < pInterfaceList->dwNumberOfItems; index++)
		{
			guid = pInterfaceList->InterfaceInfo[index].InterfaceGuid;
			printf("Adapter %d: %ws\r\n",index, pInterfaceList->InterfaceInfo[index].strInterfaceDescription);
			printf("%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X\r\n",
					guid.Data1,   
	    			guid.Data2,  
	    			guid.Data3,  
	    			guid.Data4[0], guid.Data4[1],  
	    			guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);  
			
		}

		if(adapter_nr && adapter_nr < pInterfaceList->dwNumberOfItems)
		{
			guid = pInterfaceList->InterfaceInfo[adapter_nr].InterfaceGuid;
			printf("Select Adapter %d: %ws to Connect\r\n",adapter_nr, 
				pInterfaceList->InterfaceInfo[adapter_nr].strInterfaceDescription);
		}
		else
		{
			guid = pInterfaceList->InterfaceInfo[0].InterfaceGuid;
			printf("Select Adapter 0: %ws to Connect\r\n", 
				pInterfaceList->InterfaceInfo[0].strInterfaceDescription);
		}

		dwError = WlanDisconnect(hClientHandle, &guid, NULL);
		if(dwError != ERROR_SUCCESS)
		{
			printf("Error: WlanDisconnect %d, Adapter Shutdown?\r\n", dwError);
			return -1;
		}

		dwError = WlanGetAvailableNetworkList(hClientHandle, &guid, 
			WLAN_AVAILABLE_NETWORK_INCLUDE_ALL_ADHOC_PROFILES,
			NULL, &pAvailableNetworkList);
		if(dwError != ERROR_SUCCESS)
		{
			printf("Error: WlanGetavaibleNetworkList %d, No SSID?\r\n", dwError);
			return -1;
		}

		if(pAvailableNetworkList->dwNumberOfItems == 0)
		{
			printf("Error: No SSID Network\r\n");
			Sleep(CONNECTION_DELAY);
			continue;
		}

		printf("Scanning...\r\n");
		printf(" %-35s\tENCRYPT\tSIGNAL\r\n", "  SSID");
		
		for(index = 0, curSignalQuality = 0, curIndex = -1;
			index < pAvailableNetworkList->dwNumberOfItems;
			index++)
		{
			//如果需要获得信道号, 请使用WlanGetNetworkBssList()
			pBssEntry = (WLAN_AVAILABLE_NETWORK *)&pAvailableNetworkList->Network[index];
			printf(" %-35s\t  %d\t  %d",
					pBssEntry->dot11Ssid.ucSSID,
					pBssEntry->bSecurityEnabled,
					pBssEntry->wlanSignalQuality);	
				
			if(pBssEntry->dot11Ssid.ucSSID[0] == '\0')
			{
				/* empty ssid not supported */
				printf("\r\n");
				continue;
			}

			if(pBssEntry->bSecurityEnabled)
			{
				/* security not supported */
				printf("\r\n");
				continue;
			}

			if(pBssEntry->wlanSignalQuality < signal_level)
			{
				/* signal too low */
				printf("\r\n");
				continue;
			}

			if(ssid_prefix)
			{
				if(strncmp((const char *)pBssEntry->dot11Ssid.ucSSID, (const char *)ssid_prefix,
					strlen(ssid_prefix)) != 0)
				{
					/* prefix not match */
					printf("\r\n");
					continue;
				}
			}
			
			if(strcmp((const char *)pBssEntry->dot11Ssid.ucSSID, (const char *)lastConnSsid.ucSSID) == 0)
			{
				printf("\t  #\r\n");
				continue;
			} 

			printf("\t  *\r\n");
			if(pBssEntry->wlanSignalQuality > curSignalQuality)
			{
				curSignalQuality = pBssEntry->wlanSignalQuality;
				curIndex = index;
			}
		}/* 循环结束, 找到合适的SSID或者返回 */
		
		if(curIndex == -1)
		{
			/* no ssid selected */
			printf("No SSID Selected\r\n");
			memset(&lastConnSsid, 0, sizeof(lastConnSsid));
			Sleep(CONNECTION_DELAY);
			continue;
		}
		else
		{
			pBssEntry = (WLAN_AVAILABLE_NETWORK *)&pAvailableNetworkList->Network[curIndex];
			printf("SSID Selected: %-35s\r\n",pBssEntry->dot11Ssid.ucSSID);
		}
#ifdef WIN7_ONLY
		wlanConnPara.wlanConnectionMode = wlan_connection_mode_discovery_unsecure;
		wlanConnPara.strProfile = NULL;
		wlanConnPara.pDot11Ssid = &(pBssEntry->dot11Ssid);
		wlanConnPara.pDesiredBssidList = NULL;
		wlanConnPara.dot11BssType = dot11_BSS_type_infrastructure;
		wlanConnPara.dwFlags = 0;
		
		dwError = WlanConnect(hClientHandle, &guid, &wlanConnPara, NULL);
		if(dwError != ERROR_SUCCESS)
		{
			printf("Error: WlanConnect %d\r\n", dwError);
			continue;
		}

#else
		CString szProfileXML("");
		/*头*/
        szProfileXML += _T("<?xml version=\"1.0\"?>");
        szProfileXML += _T("<WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\">");

		/*name，与SSID相同*/
		szProfileXML += _T("<name>"); 
        szProfileXML += _T((char*)pBssEntry->dot11Ssid.ucSSID);
		szProfileXML += _T("</name>");
		
		/*SSIDConfig*/  
		szProfileXML += _T("<SSIDConfig><SSID>");
//		szProfileXML += _T("<hex>");
//		szProfileXML += _T(pSSIDHex);
//		szProfileXML += _T("434d43432d46524545");
//		szProfileXML += _T("</hex>");
		szProfileXML += _T("<name>");
		szProfileXML += _T((char*)pBssEntry->dot11Ssid.ucSSID);
		szProfileXML += _T("</name></SSID>");
		szProfileXML += _T("</SSIDConfig>");
		
		/*Connection*/  
		szProfileXML += _T("<connectionType>");
        szProfileXML += _T("ESS");
		szProfileXML += _T("</connectionType>");
		szProfileXML += _T("<connectionMode>");
        szProfileXML += _T("manual");
		//szProfileXML += _T("auto");
		szProfileXML += _T("</connectionMode>");
		szProfileXML += _T("<autoSwitch>");
        szProfileXML += _T("false");
		szProfileXML += _T("</autoSwitch>");
		
		/* authEncryption */
		szProfileXML += _T("<MSM><security><authEncryption>");  
		szProfileXML += _T("<authentication>");
		szProfileXML += _T("open");
		szProfileXML += _T("</authentication>");
		szProfileXML += _T("<encryption>");
		szProfileXML += _T("none");
		szProfileXML += _T("</encryption>");
		szProfileXML += _T("<useOneX>");
		szProfileXML += _T("false");
		szProfileXML += _T("</useOneX>");
		szProfileXML += _T("</authEncryption></security></MSM>");
		
		szProfileXML += _T("</WLANProfile>");

//		printf("%s\n", szProfileXML);

		USES_CONVERSION;
		
		dwError = WlanSetProfile(hClientHandle, &guid, 0, A2CW((LPCSTR)szProfileXML), NULL, 1,
			NULL, &retCode);
		if(dwError != ERROR_SUCCESS)
		{
			printf("Error: WlanSetProfile %d\r\n", dwError);
			continue;
		}

		wlanConnPara.wlanConnectionMode = wlan_connection_mode_profile;
		wlanConnPara.strProfile = A2CW((LPCSTR)pBssEntry->dot11Ssid.ucSSID);
		wlanConnPara.pDot11Ssid = &(pBssEntry->dot11Ssid);
		wlanConnPara.pDesiredBssidList = NULL;
		wlanConnPara.dot11BssType = dot11_BSS_type_infrastructure;
		wlanConnPara.dwFlags = 0;

		dwError = WlanConnect(hClientHandle, &guid, &wlanConnPara, NULL);
		if(dwError != ERROR_SUCCESS)
		{
			printf("Error: WlanConnect %d\r\n", dwError);
			continue;
		}

#endif

		Sleep(CONNECTION_DELAY);
		
		while(Connection_status(
			src_ipaddr, 
			src_macaddr, 
			dst_ipaddr, 
			dst_macaddr))
		{
			/* connection kept */
			Sleep(CHECK_INTERVAL);
		}
		
		lastConnSsid = pBssEntry->dot11Ssid;
	}
	return 0;
}
