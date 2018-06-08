#include <windows.h>
#include <stdio.h>
#include <setupapi.h>
#include <stdbool.h>

int enum_usb_device_info()
{		
	int i = 0;
	int res = 0;
	HDEVINFO h_devinfo;
	SP_DEVINFO_DATA devinfo_data = {sizeof(devinfo_data)};

	GUID class_guid[1];
	unsigned long required_size = 0;

	res = SetupDiClassGuidsFromName("USB", (LPGUID)&class_guid, 1,
		                          &required_size);
	if (res != true) {
		printf("SetupDiClassGuidsFromName Error\n");
		res = GetLastError();     
		return res;
	}

	// get device class information handle
	h_devinfo = SetupDiGetClassDevs(&class_guid[0], NULL, NULL, DIGCF_PRESENT);       
	if (h_devinfo == INVALID_HANDLE_VALUE)     
	{         
		res = GetLastError();     
		return res;
	}  

	// enumerute device information
	for (i = 0; SetupDiEnumDeviceInfo(h_devinfo, i, &devinfo_data); i++)
	{		
		unsigned long data_type;         
		char friendly_name[MAX_PATH] = {0};         
		unsigned long buf_size = MAX_PATH;        
		unsigned long req_bufsize = 0;      
		
		// get device description information
		if (!SetupDiGetDeviceRegistryPropertyA(h_devinfo, &devinfo_data, SPDRP_DEVICEDESC, &data_type, friendly_name, buf_size, &req_bufsize))
		{
			res = GetLastError();
			continue;
		}
		printf("USB device %d: %s\n", i, friendly_name);
	}

	return 0;
}

int main()
{
	enum_usb_device_info();
}