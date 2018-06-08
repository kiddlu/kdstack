#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/mman.h>

#include <unistd.h>

#include <cutils/properties.h>
#define BD_ADDR_FILE "/persist/bd_addr.txt"

void usage(void)
{
	printf("Usage:\n");
	printf("bdaddrtool -w 00:11:22:33:44:55    (write bdaddr)\n");
	printf("bdaddrtool -r                      (read  bdaddr)\n");
	printf("bdaddrtool -g                      (generate bdaddr)\n");
	printf("bdaddrtool -h                      (show help)\n");
}

int check_mac_valid(char *mac)
{
	int i;

	if (strlen(mac) != 17)
	{
		return -1;
	}

	for (i=0; i<17; i++) {
		switch (mac[i]) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'a':
			case 'A':
			case 'b':
			case 'B':
			case 'c':
			case 'C':
			case 'd':
			case 'D':
			case 'e':
			case 'E':
			case 'f':
			case 'F':
			case ':':
				break;
			default:
				return -1;
		}
	}

	return 0;
}

void write_mac(int argc, char *argv[])
{
	char *file = BD_ADDR_FILE;
	FILE *fp;

	if (check_mac_valid(argv[2]) < 0)
	{
		printf("Invalid mac %s, exit\n", argv[2]);
		exit(1);
	}

	fp = fopen(file, "w");

	if (fp == NULL)
	{
		printf("Failed to write file");
		return;
	}

	fprintf(fp,	"%s\n", argv[2]);

	fclose(fp);
}

void read_mac(void)
{
	char *file = BD_ADDR_FILE;
	FILE *fp;
	char buf[255], *pt;

	if (access(file, 0) != 0)
	{
		printf("Bluetooth addr file does not exist\n");
		return ;
	}

	fp = fopen(file, "r");

	if (fp == NULL)
	{
		printf("Failed to read file");
		return;
	}

	fread(buf, sizeof(buf), 1, fp);
	printf("%s", buf);

	fclose(fp);
}

int get_serialno(char mac[9])
{
	char value[PROPERTY_VALUE_MAX];

	property_get("ro.serialno", value, "");

	strncpy(mac, value, strlen(mac));

	return 0;
}

void generate_mac(void)
{
	char *file = BD_ADDR_FILE;
	FILE *fp;
	char mac[9] = "00000000";

	if (access(file, 0) == 0)
		return ;

	if (get_serialno(mac) < 0)
		return ;

	fp = fopen(file, "w");

	if (fp == NULL)
	{
		printf("Failed to generate file");
		return;
	}

	fprintf(fp,	"00:02:%c%c:%c%c:%c%c:%c%c\n",
	 mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], mac[6], mac[7]);

	fclose(fp);

	printf("Generate bluetooth device addr by [ro.serialno]\n");
}

int main(int argc, char *argv[])
{
	if (argc < 2 || (argc >= 2 && strlen(argv[1]) != 2))
	{
		usage();
		exit(1);
	}

	if (strcmp("-h", argv[1]) == 0)
	{
		usage();
		return 0;
	}
	else if (strcmp("-w", argv[1]) == 0)
	{
		write_mac(argc, argv);
	}
	else if (strcmp("-r", argv[1]) == 0)
	{
		read_mac();
	}
	else if (strcmp("-g", argv[1]) == 0)
	{
		generate_mac();
	}
	else
	{
		usage();
		exit(1);
	}

	return 0;
}
