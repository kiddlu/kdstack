#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <string.h>
#include <linux/sockios.h>

unsigned char g_ifr_name[16] = "lo";
unsigned char g_macaddr[16];

unsigned int g_subnetmask;
unsigned int g_ipaddr;
unsigned int g_broadcast_ipaddr;

void init_net(void)
{
    int i;
    int sock;
    struct sockaddr_in sin;
    struct ifreq ifr;

    sock=socket(AF_INET, SOCK_DGRAM, 0);
    if(sock==-1)
        perror("socket");

    strcpy(ifr.ifr_name, g_ifr_name);
    printf("ifr name:\t%s\n", g_ifr_name);

    if(ioctl(sock, SIOCGIFHWADDR, &ifr) <0 )
        perror("ioctl error\n");

    memcpy(g_macaddr, ifr.ifr_hwaddr.sa_data, 6);
    printf("local mac:\t");
    for(i=0;i<5;i++)
        printf("%.2x:", g_macaddr[i]);
    printf("%.2x\n", g_macaddr[i]);

    if(ioctl(sock, SIOCGIFADDR, &ifr)<0)
        perror("ioctl error\n");
    memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
    g_ipaddr = sin.sin_addr.s_addr;
    printf("local ip:\t%s\n", inet_ntoa(sin.sin_addr));
    
    if(ioctl(sock, SIOCGIFBRDADDR, &ifr)<0)
        perror("ioctl error\n");
    memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
    g_broadcast_ipaddr = sin.sin_addr.s_addr;
    printf("broadcast:\t%s\n", inet_ntoa(sin.sin_addr));
    
    if(ioctl(sock,SIOCGIFNETMASK,&ifr)<0)
        perror("ioctl error\n");
    memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
    g_subnetmask = sin.sin_addr.s_addr;
    printf("subnetmask:\t%s\n", inet_ntoa(sin.sin_addr));
    
    close(sock); 
}

int main(void)
{
    init_net();
}
