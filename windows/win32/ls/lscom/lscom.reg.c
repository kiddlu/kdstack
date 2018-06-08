#include <stdio.h>
#include <string.h>
#include <windows.h>

#define CHAR_LEN 255

int main()
{
	unsigned long com_nb;
	char reg_key[CHAR_LEN]; 
	char reg_val[CHAR_LEN]; 
	long ret;
	unsigned long reg_index = 0; 
	unsigned long len_key;
	unsigned long len_val; 
	unsigned long type;
	
	HKEY hkey; 
	char  *key_str="HARDWARE\\DEVICEMAP\\SERIALCOMM\\";

	len_key = sizeof(reg_key); 
	len_val = sizeof(reg_val);

	//long ret0 = (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_Set, 0, KEY_READ, &hKey)); 
	if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, key_str, 0, KEY_READ, &hkey) == ERROR_SUCCESS) {
		printf("NODE\t\t                       PORTNUM \t\t       CYGWIN\n\n");
		do {
			ret = RegEnumValue(hkey, reg_index++, reg_key, &len_key, NULL, &type, reg_val, &len_val);
			if((ret == ERROR_SUCCESS) || (ret == ERROR_MORE_DATA)) { 
				com_nb = atoi(strtok(reg_val, "COM"));
				printf("%-30s \t\t %5s \t\t   /dev/ttyS%d\n", reg_key, reg_val, com_nb - 1);
			}

			len_key = sizeof(reg_key); 
			len_val = sizeof(reg_val); 
		} while ((ret == ERROR_SUCCESS) || (ret == ERROR_MORE_DATA)); 
		RegCloseKey(hkey); 
	} else {
		printf("Can not open HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP\\SERIALCOMM\\\n");
	}
}