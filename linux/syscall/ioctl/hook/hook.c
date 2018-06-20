#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/syscall.h>

int ioctl(int fd, int request, ...)
{
    int ret=0;

    va_list args;
    void *argp;

    va_start(args, request);
    argp = va_arg(args, void *);
    va_end(args);

    ret=syscall(SYS_ioctl, fd, request, argp);

    return ret;
}
