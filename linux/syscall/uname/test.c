#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    struct utsname testbuff;
    int fb=0;

    fb=uname(&testbuff);
    if(fb<0) {
        perror("uname");
        return 0;
    } else {
        printf(" sysname:%s\n nodename:%s\n release:%s\n version:%s\n machine:%s\n \n ",\
                    testbuff.sysname,\
                    testbuff.nodename,\
                    testbuff.release,\
                    testbuff.version,\
                    testbuff.machine);
#if _UTSNAME_DOMAIN_LENGTH - 0
# ifdef __USE_GNU
    printf(" domainame:%s\n ",testbuff.domainname);
    //char domainname[_UTSNAME_DOMAIN_LENGTH]; //当前域名
# else
    printf(" __domainame:%s\n ",testbuff.__domainname);
    //char __domainname[_UTSNAME_DOMAIN_LENGTH];
# endif
#endif

    }
return 0;
}
