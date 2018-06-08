#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

static bool pid_exist(unsigned int pid)
{
    char path[PATH_MAX] = {'\0'};
    sprintf(path, "/proc/%d/comm", pid);
    if (access(path, 0) == 0)
        return true;
    return false;
}

/*
static bool pid_exist(unsigned int pid)
{
    char buf[PATH_MAX] = {'\0'};
    char path[PATH_MAX] = {'\0'};

    sprintf(path, "/proc/%d/comm", pid);

    ssize_t ret = readlink(path, buf, PATH_MAX);
    if (ret < 0) {
        if(errno == ENOENT)
            return false;
    }

    return true;
}
*/

int main(int argc, char *argv[])
{
    if(argc < 2) {
        printf("pidexist <pid>\n");
        return 0;
    }

    int pid = atoi(argv[1]);

    if(pid_exist(pid))
        printf("yes\n");
    else
        printf("no\n");
}
