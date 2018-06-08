#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/mman.h>

#include <unistd.h>

#include <cutils/properties.h>

//#define DEBUG

void usage(void)
{
	printf("\nUsage:\n");
	printf("otptool -w    (write otp)\n");
	printf("otptool -r    (read  otp)\n");
	printf("otptool -c    (check otp)\n");
	printf("otptool -h    (show help)\n");
	printf("\n");
}

static int execute_cmd(char *cmd)
{
	int ret = system(cmd);
	if (ret != 0)
	{
#ifdef DEBUG
		printf("CMD \"%s\" failed.\n", cmd);
#endif
		return -1;
	}

#ifdef DEBUG
	printf("%s\n", cmd);
#endif

	return 0;
}

static int enter_mfg(void)
{
	int retval;
	int loop;

	retval = execute_cmd("svc wifi disable");
	if (retval < 0)
		return -1;

	retval = execute_cmd("cat /sys/class/rfkill/rfkill0/type | grep bluetooth > /dev/null");
	if (retval < 0)
		return -1;
	else {
		retval = execute_cmd("cat /sys/class/rfkill/rfkill0/state | grep 0 > /dev/null");
		if (retval < 0) {
			execute_cmd("echo 0 > /sys/class/rfkill/rfkill0/state");
		}
	}

	retval = execute_cmd("ifconfig wlan0 down");
	if (retval < 0)
		return -1;

	loop = 1;
	do {
		usleep(200 * 1000);
		retval = execute_cmd("wl ver 2>&1 | grep adapter > /dev/null");

		loop++;
		if(loop > 10)
			return -1;

	} while (retval < 0);

	retval = execute_cmd("echo /etc/firmware/bcm43455_mfg.bin > /sys/module/bcmdhd/parameters/firmware_path");
	if (retval < 0)
		return -1;

	retval = execute_cmd("ifconfig wlan0 up");
	if (retval < 0)
		return -1;

	loop = 1;
	do {
		usleep(200 * 1000);
		retval = execute_cmd("wl ver | grep WLTEST > /dev/null");

		loop++;
		if(loop > 10)
			return -1;

	} while (retval < 0);

	return 0;
}

static int exit_mfg(void)
{
	int retval;

	retval = execute_cmd("ifconfig wlan0 down");
	if (retval < 0)
		return -1;

	retval = execute_cmd("echo /etc/firmware/bcm43455_sta.bin > /sys/module/bcmdhd/parameters/firmware_path");
	if (retval < 0)
		return -1;

	return 0;
}

static int write_otp(void)
{
	int retval;

	if (enter_mfg() < 0) {
		printf("Enter MFG not ok\n");
		return -1;
	}

	retval = execute_cmd("wl cisdump | grep 0x43 > /dev/null");
	if (retval < 0) {
		printf("\nWriting...\n");

		retval = execute_cmd("wl ciswrite -p /etc/firmware/bcm43455_pciehdr.bin");
		if (retval < 0) {
			exit_mfg();
			return -1;
		}

		retval = execute_cmd("wl cisdump | grep 0x43 > /dev/null");
		if (retval < 0) {
			printf("Write OTP ERR! Please Write again\n");
			return -1;
		}

		printf("Write OTP OK!\n");
	} else {
		printf("\nOTP had already written!\n\n");
	}

	if (exit_mfg() < 0) {
		printf("Exit MFG not ok\n");
		return -1;
	}

	return 0;
}

static int read_otp(void)
{
	int retval;

	if (enter_mfg() < 0) {
		printf("Enter MFG not ok\n");
		return -1;
	}

	retval = execute_cmd("wl cisdump");
	if (retval < 0)
		return -1;

	if (exit_mfg() < 0) {
		printf("Exit MFG not ok\n");
		return -1;
	}

	return 0;
}

static int check_otp(void)
{
	int retval;

	if (enter_mfg() < 0) {
		printf("Enter MFG not ok\n");
		return -1;
	}

	retval = execute_cmd("wl cisdump | grep 0x43  > /dev/null");
	if (retval < 0) {
		printf("\nYou need to write OTP!\n\n");
	} else {
		printf("\nOTP had already written!\n\n");
	}

	if (exit_mfg() < 0) {
		printf("Exit MFG not ok\n");
		return -1;
	}

	return retval; 
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
		write_otp();
	}
	else if (strcmp("-r", argv[1]) == 0)
	{
		read_otp();
	}
	else if (strcmp("-c", argv[1]) == 0)
	{
		check_otp();
	}
	else
	{
		usage();
		exit(1);
	}

	return 0;
}
