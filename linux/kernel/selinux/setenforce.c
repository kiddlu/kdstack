#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

int security_setenforce(int value)
{
	int fd, ret;
	char buf[20];

	fd = open("/sys/fs/selinux/enforce", O_RDWR);
	if (fd < 0)
		return -1;

	snprintf(buf, sizeof(buf), "%d", value);
	ret = write(fd, buf, strlen(buf));
	close(fd);
	if (ret < 0)
		return -1;

	return 0;
}

int main(int argc, void * argv[])
{
	return security_setenforce(1);
}
