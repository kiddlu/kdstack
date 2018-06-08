#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define COMMAND1 0
#define COMMAND2 1
#define COMMAND3 8
#define COMMAND4 16
int main()
{
int fd, fd1;
int i;
char data[40960];
int retval;
fd =open("/dev/demo", O_RDWR);
if(fd == -1){
     perror("error open\n");
     exit(-1);
}
printf("open /dev/demo successfully\n");
/*
retval = ioctl(fd, COMMAND1,0);
if(retval ==-1) {
    perror("ioctl error\n");
    exit(-1);
}
else{
    read(fd, data,40960);
    int s;
    for(s=0;s<40960;s++)
    {
        printf("%c",data[s]);
    }
    
    printf("\n");
    
    printf("send command1 successfully\n");
}
*/
/*
retval = ioctl(fd, COMMAND2,0);
if(retval ==-1) {
    perror("ioctl error\n");
    exit(-1);
}
else{
    
    read(fd,data,40960);
    int s;
    for(s=0;s<40960;s++)
    {
        printf("%c",data[s]);
    }
    
    printf("\n");
    
    printf("send command2 successfully\n");
}
*/
/*
pid_t pid;
printf("Please input the process pid: \n");
scanf("%d",&pid);
retval = ioctl(fd, COMMAND3,pid);
if(retval ==-1) {
    perror("ioctl error\n");
    exit(-1);
}
else{
    read(fd,data,40960);
    int s;
    for(s=0;s<40960;s++)
    {
        printf("%c",data[s]);
    }
    
    printf("\n");
    
    printf("send command3 successfully\n");
}
*/
retval = ioctl(fd, COMMAND4,0);
if(retval ==-1) {
    perror("ioctl error\n");
    exit(-1);
}
else{
    read(fd,data,40960);
    int s;
    for(s=0;s<40960;s++)
    {
        printf("%c",data[s]);
    }
    
    printf("\n");
    
    printf("send command4 successfully\n");
}
printf("read successfully\n");
close(fd);
}