#include <stdio.h>

#define BUF_SIZE 1024

int pexec(const char *cmd)
{
	FILE * fp;
	char buffer[BUF_SIZE];
	fp = popen(cmd, "r");
	while (fgets(buffer, sizeof(buffer), fp)!= NULL) {
		printf("%s", buffer);
	};

	pclose(fp);
	return 0;
}

int main()
{
	pexec("cat /etc/passwd");
}
