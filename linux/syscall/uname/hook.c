#define _GNU_SOURCE         /* See feature_test_macros(7) */

#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <sys/syscall.h>

#define UNAME_MACHINE "armv7l"

int uname(struct utsname *buf)
{
    int ret=0;
    ret=syscall(SYS_uname, buf);

    //printf("hook\n");
    if(ret == 0) {
      memset(buf->machine, '\0', sizeof(buf->machine));
      strncpy(buf->machine, UNAME_MACHINE, sizeof(UNAME_MACHINE));
      return ret;
    }
}
