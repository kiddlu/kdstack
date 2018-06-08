//TPWifiSvc.cpp
//��C/C++��д��Window����һ��
//����һ��
//echo sc delete svc
//echo sc create svc binpath= E:\Debug\svc.exe
//������:
//svc.exe uninstall
//svc.exe install
//svc.exe run

#pragma once
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <comdef.h>
#include <tchar.h>
#include <comutil.h>
#include <dbt.h>

#define SLEEP_TIME 5000
#define LOGFILE "C:\\svcLog.txt"

using namespace std;
CHAR* ServiceName = "TP-Link WiFi Service";

enum EnumRunState
{
	 None=0,
     Running=1,
	 Stoped=2,
	 Pause=3,
};
EnumRunState RunState;

#define DEBUG
#ifdef DEBUG
INT WriteToLog(CHAR* str)
{
    FILE* log;
	log=fopen(LOGFILE,"a+");
	if(log==NULL)  return 0;
	fprintf(log,"%s\n",str);
	fclose(log);
	return 1;
}
#else
INT WriteToLog(CHAR* str)
{
	return 1;
}
#endif

//��������ͷ���
SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;
HDEVNOTIFY hDevNotify;

HANDLE terminateEvent; //�¼�
HANDLE threadHandle;   //�߳�
HANDLE deviceEvent;

VOID terminate(DWORD error);
VOID ServiceMain(INT argc,CHAR** argv);
VOID WINAPI ControlHandler(DWORD dwCtrl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext);
INT InitService();

//�������
BOOL RunService(CHAR* sSvcName);
VOID ResumeService();
VOID PauseService();
VOID StopService();

//����ע�ắ��
BOOL SendStatusToSCM (DWORD dwCurrentState,
   DWORD dwWin32ExitCode, 
   DWORD dwServiceSpecificExitCode,
   DWORD dwCheckPoint,
   DWORD dwWaitHint);

VOID InstallService();
VOID UninstallService();

DWORD WINAPI ServiceThread(LPDWORD param);

//��װ��ڵ�
VOID main(INT argc, CHAR* argv[])
{
	CHAR* TServiceName = ServiceName;
	//����һ	
	SERVICE_TABLE_ENTRY ServiceTable[2];
	//���÷������ƺͷ�����ں���	 
	ServiceTable[0].lpServiceName=TServiceName;
	ServiceTable[0].lpServiceProc=(LPSERVICE_MAIN_FUNCTION)ServiceMain;
	ServiceTable[1].lpServiceName=NULL;
	ServiceTable[1].lpServiceProc=NULL;
 
	 //ע��Ϊ����ͷ�ע��
	if(argc<=1)
	{
		//usage(ʹ�÷���)��ʾ   
		cout<<"useage:"<<endl;
		cout<<"\"install\""; 
		cout<<"or";
		cout<<"\"uninstall\"";
		cout<<"or";
		cout<<"\"run\""<<endl;	   
	}
	
	if(argc>1)
	{
		if(_stricmp("install",argv[1])==0)
		{
			InstallService(); 
			cout<<"installService Succed!"<<endl;
		}		    
		else if(_stricmp("uninstall",argv[1])==0)
		{
			UninstallService(); 
			cout<<"UninstallService Succed!"<<endl;
		}		  
		else if(_stricmp("run",argv[1])==0)
		{
			RunService(TServiceName);		   
		} 
		else
		{    //usage(ʹ�÷���)��ʾ
			cout<<"useage:"<<endl;
			cout<<"\"install\""; 
			cout<<"or";
			cout<<"\"uninstall\"";
			cout<<"or";
			cout<<"\"run\""<<endl;	  
		}
	}
	 
	//��������Ŀ��Ʒ��ɻ��߳�(������ִ��һ�����߳�)
	StartServiceCtrlDispatcher(ServiceTable);
	return;
}


//������ڵ�(���߳�)
VOID ServiceMain(INT argc,CHAR** argv)
{
	CHAR* TServiceName = ServiceName;
	INT success;

#if 0
	success = InitService();
	if(!success)
	{
        //��ʼ��ʧ��,��ֹ����
		WriteToLog("ServiceMain::InitService��ʼ��ʧ��,��ֹ����");
		ServiceStatus.dwCurrentState=SERVICE_STOPPED;
		ServiceStatus.dwWin32ExitCode=-1;
		SetServiceStatus(hStatus,&ServiceStatus);
		//�˳�ServiceMain
		return 0;
	}
#endif

	//��ʼ����
	RunService(TServiceName);

	//ע�������ƴ�����
	hStatus = RegisterServiceCtrlHandlerEx(TServiceName,(LPHANDLER_FUNCTION_EX)ControlHandler,NULL);   
	if(!hStatus)
	{	
		WriteToLog("RegisterServiceCtrlHandler Failed!");
		return;
	}
	
	//��SCM���湤��״̬
	RunState = EnumRunState::Running;
	
    ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	ServiceStatus.dwServiceType = SERVICE_WIN32;
    ServiceStatus.dwControlsAccepted = 
			SERVICE_ACCEPT_STOP |
			SERVICE_ACCEPT_PAUSE_CONTINUE |
			SERVICE_ACCEPT_SHUTDOWN;
	
	SetServiceStatus(hStatus,&ServiceStatus);
 
	//����һ���ս��¼�����ֹͣ����
	terminateEvent = CreateEvent(0,TRUE,FALSE,0);
	if (!terminateEvent)
	{
		terminate(GetLastError());
		return;
	}   
	success = SendStatusToSCM(SERVICE_START_PENDING,NO_ERROR,0,2,1000);
	if (!success)
	{
		terminate(GetLastError()); 
		return;
	}

	deviceEvent = CreateEvent(0,TRUE,FALSE,0);
	if (!deviceEvent)
	{
		terminate(GetLastError()); 
		return;
	}   

	GUID Guid;
	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
	ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );

#if 0
	CLSIDFromString("4D36E972-E325-11CE-BFC1-08002BE10318", &Guid);
#else
	Guid.Data1 = 0x4d36e967;
	Guid.Data2 = 0xe325;
	Guid.Data3 = 0x11ce;
	Guid.Data4[0] = 0xbf;
	Guid.Data4[1] = 0xc1;
	Guid.Data4[2] = 0x08;
	Guid.Data4[3] = 0x00;
	Guid.Data4[4] = 0x2b;
	Guid.Data4[5] = 0xe1;
	Guid.Data4[6] = 0x03;
	Guid.Data4[7] = 0x18;
#endif

    NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    NotificationFilter.dbcc_classguid = Guid;

	hDevNotify = RegisterDeviceNotification(hStatus,
		&NotificationFilter,
		DEVICE_NOTIFY_SERVICE_HANDLE);	
	
	//��ʼ������
	success =  InitService();
	if (!success)
	{
		terminate(GetLastError());
		return;
	}  
	success = SendStatusToSCM(SERVICE_RUNNING,NO_ERROR,0,0,0);
	if (!success)
	{
		terminate(GetLastError()); 
		return;
	}
   
	//�ȴ�ֹͣ
	WaitForSingleObject(terminateEvent, INFINITE);

	terminate(0);
	WriteToLog("ServiceMain Complete");
}

//�������̨�������
VOID WINAPI ControlHandler(DWORD dwCtrl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext)
{
	DWORD  currentState = 0;
	BOOL success; 

	WriteToLog("ControlHandler:"+dwCtrl);
	switch(dwCtrl)
	{ 
	case SERVICE_CONTROL_STOP:
			RunState=EnumRunState::Stoped;
			currentState = SERVICE_STOP_PENDING;
			ServiceStatus.dwCurrentState=currentState;
			SetServiceStatus(hStatus,&ServiceStatus);		    
			StopService();
			return;

	case SERVICE_CONTROL_DEVICEEVENT:
			if(dwEventType == DBT_DEVICEARRIVAL)
			{
				SetEvent(deviceEvent);
			}
			else if(dwEventType == DBT_DEVICEREMOVECOMPLETE)
			{
			}
			break;
		
#ifdef DEBUG			
	case SERVICE_CONTROL_PAUSE:	
		     RunState=EnumRunState::Pause;
			 currentState = SERVICE_PAUSED;
			 ServiceStatus.dwCurrentState=currentState;
			 SetServiceStatus(hStatus,&ServiceStatus);
			 PauseService();
			break;
	case SERVICE_CONTROL_CONTINUE:	
			RunState=EnumRunState::Running;
			 currentState = SERVICE_RUNNING;
			 ServiceStatus.dwCurrentState=currentState;
			 SetServiceStatus(hStatus,&ServiceStatus);
			 ResumeService();
			break;
	case SERVICE_CONTROL_INTERROGATE:
			// it will fall to bottom and send status
			break;
#endif

	// Do nothing
	case SERVICE_CONTROL_SHUTDOWN:	   
		return;
	default:
		break;
   }   
   //ServiceStatus.dwCurrentState=currentState;
   //SetServiceStatus(hStatus,&ServiceStatus);
}

//��ʼ������(��ʼ����ʱִ��)
INT InitService()
{
	INT result;
	CHAR* TServiceName = ServiceName;
	
	//д��־��Ϣ
	result = WriteToLog("Monitoring Started."); 

	//��ʼһ���߳� Start the service's thread
	DWORD id;   
	threadHandle = CreateThread(0, 0,(LPTHREAD_START_ROUTINE)ServiceThread,0, 0, &id);   
	if (threadHandle==0)
	{
		result=0;
	}
	else
	{		
		result=1;
	}

   WriteToLog("InitService Done");
   return result;
}


DWORD WINAPI ServiceThread(LPDWORD param)
{ 

   //�̲߳���
	while(RunState==EnumRunState::Running)
	{
		
		WaitForSingleObject(deviceEvent, INFINITE);
		WriteToLog("Device Event\n");
		ResetEvent(deviceEvent);
	}

	WriteToLog("ServiceThread Return!");
	return 0;
}

BOOL RunService(char* sSvcName)
{   
   SC_HANDLE schSCManager;
   SC_HANDLE scHandle;
   BOOL boolRet;

   //Open to SCM   
   schSCManager = OpenSCManager(0,0,SC_MANAGER_ALL_ACCESS);

   //Open
   scHandle = OpenService(schSCManager,sSvcName,
   						SERVICE_ALL_ACCESS);
   
   //Start
   boolRet = StartService(scHandle, 0, NULL);  

   return boolRet;
}

#ifdef DEBUG
VOID ResumeService()
{
   //pauseService=FALSE;
   //ResumeThread(threadHandle);
   return;
}
VOID PauseService()
{
   //pauseService = TRUE;
   //SuspendThread(threadHandle);
   return;
}
#endif

VOID StopService() 
{
	RunState=EnumRunState::Stoped;
	
   // Set the event that is holding ServiceMain
   // so that ServiceMain can return
   SetEvent(terminateEvent);
}

void InstallService()
{ 
	SC_HANDLE schService; 
	SC_HANDLE schSCManager; 
	TCHAR szPath[512];

	char* TServiceName = ServiceName;
	 
	//Install Starting... 
	//��ȡģ���ļ�·������
	if(!GetModuleFileName(NULL,szPath,512)) 
	{  
		return;  
	}
	
	// open to SCM��
	schSCManager = OpenSCManager( 
								0,  // pointer to machine name string
								0,  // pointer to database name string
								SC_MANAGER_CREATE_SERVICE // type of access
								);
	if (!schSCManager)
	{
		WriteToLog("OpenScManagerErrorID:"+GetLastError());  
		return;
	}
	
	// Install �·���
	schService = CreateService(
			schSCManager, // handle to service control manager database
			TServiceName, // pointer to name of service to start
			TServiceName, // pointer to display name     
			SERVICE_ALL_ACCESS,// type of access to service
			SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS ,// type of service
			SERVICE_DEMAND_START,// when to start service
			SERVICE_ERROR_NORMAL,// severity if service fails to start
			szPath,       // pointer to name of binary file
			NULL,         // pointer to name of load ordering group
			NULL,         // pointer to variable to get tag identifier
			NULL,         // pointer to array of dependency names
			NULL,         // pointer to account name of service
			NULL          // pointer to password for service account
			);
	if (!schService)
	{  
		WriteToLog("CreateServiceErrorID:"+GetLastError()); 
		return;
	}
	else
	{
		WriteToLog("Service Installed\n"); 
	}	 
	// clean up
	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);	 
	WriteToLog("Install Ending...\n"); 
}

VOID UninstallService()
{ 
	SC_HANDLE schService;
	SC_HANDLE schSCManager; 
	BOOL success;
	SERVICE_STATUS svcStatus;

	CHAR* TServiceName = ServiceName;
	 
	WriteToLog("Uninstall Starting...");
	 
	// open to SCM��
	schSCManager = OpenSCManager(
			0,// pointer to machine name string 
			0,// pointer to database name string
			SC_MANAGER_CREATE_SERVICE // type of access
			);
	
	if (!schSCManager)
	{
		WriteToLog("OpenScManagerErrorID:"+GetLastError());
		return;
	}
	 
	//��һ������
	schService = OpenService(
				  schSCManager,              // handle to service control manager database
				  TServiceName,              // pointer to name of service to start
				  SERVICE_ALL_ACCESS | DELETE// type of access to service
				  );
	 if (!schService)
	 {
		  WriteToLog("OpenServiceErrorID:"+GetLastError());
		  return;
	 }
	 //(if necessary)
	 success = QueryServiceStatus(schService, &svcStatus);
	 if (!success)
	 {
		 WriteToLog("In QueryServiceStatus ErrorID:"+GetLastError());
		  return;
	 }
	 if (svcStatus.dwCurrentState != SERVICE_STOPPED)
	 {
		   WriteToLog("Stopping Service...");
		   success = ControlService(
			   schService,           // handle to service
			   SERVICE_CONTROL_STOP, // control code
			   &svcStatus            // pointer to service status structure
		   );
		   if (!success)
		   {
			   WriteToLog("In ControlServiceErrorID:"+GetLastError());
			   return;
		   }
	 }
	 //�ȴ�
	 do
	 {
		  QueryServiceStatus(schService,&svcStatus);      
		  Sleep(500);    
	  
	 }while(SERVICE_STOP_PENDING==svcStatus.dwCurrentState);        
	 
	 if(SERVICE_STOPPED!=svcStatus.dwCurrentState)
	 {
		 WriteToLog("Failed to Stop Service ErrorID:"+GetLastError());  
	     return;
	 }
	 
	 //ɾ������ 
	 success = DeleteService(schService);
	 if (success)
	 {
	     WriteToLog("Service Removed\n");
	 }
	 else
	 {
		 WriteToLog("In DeleteService ErrorID:"+GetLastError());
	     return;
	 }	 
	 //Clean up
	 CloseServiceHandle(schService);
	 CloseServiceHandle(schSCManager);	 
	 WriteToLog("Uninstal Ending...");
}


BOOL SendStatusToSCM (DWORD dwCurrentState,
   DWORD dwWin32ExitCode, 
   DWORD dwServiceSpecificExitCode,
   DWORD dwCheckPoint,
   DWORD dwWaitHint)
{
	   BOOL success;
	   SERVICE_STATUS serviceStatus;
	   
	   // Fill in all of the SERVICE_STATUS fields
	   serviceStatus.dwServiceType =SERVICE_WIN32_OWN_PROCESS;
	   serviceStatus.dwCurrentState = dwCurrentState;
	   
	   // If in the process of something, then accept
	   // no control events, else accept anything
	   if (dwCurrentState == SERVICE_START_PENDING)
		    serviceStatus.dwControlsAccepted = 0;
	   else
			serviceStatus.dwControlsAccepted = 
			SERVICE_ACCEPT_STOP |
			SERVICE_ACCEPT_PAUSE_CONTINUE |
			SERVICE_ACCEPT_SHUTDOWN;
	   
	   // if a specific exit code is defines, set up
	   // the win32 exit code properly
	   if (dwServiceSpecificExitCode == 0)
			serviceStatus.dwWin32ExitCode =dwWin32ExitCode;
	   else
		    serviceStatus.dwWin32ExitCode =	ERROR_SERVICE_SPECIFIC_ERROR;
	   serviceStatus.dwServiceSpecificExitCode =dwServiceSpecificExitCode;
	   
	   serviceStatus.dwCheckPoint = dwCheckPoint;
	   serviceStatus.dwWaitHint = dwWaitHint;
	   
	   // Pass the status record to the SCM
	   //SERVICE_STATUS_HANDLE serviceStatusHandle;
	   success = SetServiceStatus (hStatus,&serviceStatus);
	   if (!success)
	   {
		  StopService();	 
	   }
	   return success;
 }

VOID terminate(DWORD error)
{
   // if terminateEvent has been created, close it.
   if (terminateEvent)
    CloseHandle(terminateEvent);

   if (deviceEvent)
    CloseHandle(deviceEvent);
   
   // Send a message to the scm to tell about
   // stop age
   if (hStatus)
    SendStatusToSCM(SERVICE_STOPPED, error,
    0, 0, 0);
   
   // If the thread has started kill it off
   if (threadHandle)
    CloseHandle(threadHandle);
   
   // Do not need to close serviceStatusHandle
}