// lscom.cpp : Defines the entry point for the console application.
//build with mingw:  x86_64-w64-mingw32-gcc.exe -o lscom lscom.c -lsetupapi -static

#include <windows.h>
#include <stdio.h>
#include <setupapi.h>
#include <stdbool.h>

static void ListupSerialPort(void)
{
	GUID class_guid[1];
	unsigned long required_size;
	bool ret;
	HDEVINFO devinfo_set = NULL;
	SP_DEVINFO_DATA devinfo_data;
	unsigned int member_index = 0;
	int i;

	devinfo_data.cbSize = sizeof(SP_DEVINFO_DATA);

	// Get ClassGuid from ClassName for PORTS class
	ret = SetupDiClassGuidsFromName("PORTS", (LPGUID)&class_guid, 1,
		                          &required_size);
	if (ret != true) {
		printf("SetupDiClassGuidsFromName Error\n");
		goto cleanup;
	}

	// Get class devices
	devinfo_set = SetupDiGetClassDevs(&class_guid[0], NULL, NULL, DIGCF_PRESENT | DIGCF_PROFILE);

	printf("PORT \t\t CYGWIN \t\t NAME\n\n");

	if (devinfo_set) {
		// Enumerate devices
		member_index = 0;
		while (SetupDiEnumDeviceInfo(devinfo_set, member_index++, &devinfo_data)) {
			char friendly_name[MAX_PATH];
			char port_name[MAX_PATH];
			char message[MAX_PATH];
			unsigned long req_size = 0;
			unsigned long prop_type;
			unsigned long type = REG_SZ;
			int port_nb;
			HKEY hKey = NULL;

			// Get friendly_name
			ret = SetupDiGetDeviceRegistryProperty(devinfo_set,
			                                        &devinfo_data,
			                                        SPDRP_FRIENDLYNAME,
			                                        &prop_type,
			                                        friendly_name,
			                                        sizeof(friendly_name),
			                                        &req_size);

			// Open device parameters reg key
			hKey = SetupDiOpenDevRegKey(devinfo_set,
			                            &devinfo_data,
			                            DICS_FLAG_GLOBAL,
			                            0, DIREG_DEV, KEY_READ);
			if (hKey) {
				// Qurey for portname
				req_size = sizeof(port_name);
				RegQueryValueEx(hKey,
				                "PortName",
				                0,
				                &type,
				                (unsigned char *)&port_name,
				                &req_size);

				// Close reg key
				RegCloseKey(hKey);
			}

			if (strncmp(port_name, "COM", 3) == 0)
				port_nb = atoi(&port_name[3]);

			printf("%-5s \t\t /dev/ttyS%-2d\t\t %-30s\n", port_name, (port_nb-1), friendly_name);
		}
	}
cleanup:
// Destroy device info list
	SetupDiDestroyDeviceInfoList(devinfo_set);
}

int main(int argc, char* argv[])
{
	ListupSerialPort();
    return 0;
}