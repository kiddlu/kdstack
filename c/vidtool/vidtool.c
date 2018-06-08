/*  
 * file		vidtool.c
 * brief		
 * details	
 *
 * author	
 * version	
 * date		
 *
 * history
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <pthread.h>
#define SLEEP_TIME 				100000    /* 100ms */
#define RECV_TIME_OUT			5000		/* 5ms   */
#define SEND_TIME_OUT			5000		/* 5ms  */

#define IPH_LEN 				20
#define UDPH_LEN 				8
#define DHCP_DATA_LEN 			291
#define ETH_HEADER_LEN			14
#define ETH_HEADER_LEN_WITH_VLAN	18
#define ETH_MACADDR_LEN			6

#define WAN_IF_NAME "pon0"

/* UDP协议并没有强制规定需要计算checksum */
#if 0
#define CONFIG_UDP_CSUM
#endif

int sockfd;
unsigned char pppoe_macAddr[ETH_MACADDR_LEN];
unsigned char dhcp_macAddr[ETH_MACADDR_LEN];
unsigned char pppoe_done = 0;
unsigned char dhcp_done = 0;
unsigned char debug_flag = 0;

ssize_t safe_read(int fd, void *buf, size_t count)
{
	ssize_t n;

	do {
		n = read(fd, buf, count);
	} while (n < 0 && errno == EINTR);

	return n;
}

ssize_t safe_write(int fd, const void *buf, size_t count)
{
	ssize_t n;

	do {
		n = write(fd, buf, count);
	} while (n < 0 && errno == EINTR);

	return n;
}

int safe_memdump(void *addr, int size)
{
	int index;

	while (size > 0) 
	{
		fprintf(stdout, "0x%08x: ", addr);
		index = 0;
		/* 32 bytes in the line */
		while (index < 32) 
		{
			fprintf(stdout, "%02x ", *((unsigned char *)addr));
			
			index++;
			addr++;
			size--;
			
			if (size <= 0)
			{
				break;
			}
		}
		fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");

	return 0;
}

#ifdef CONFIG_UDP_CSUM

static unsigned short safe_checksum(unsigned short *pwbuffer,uint32_t size)  
{  
	unsigned long cksum=0;  
	
	while(size > 1)
	{  
		cksum += *pwbuffer++;  
		size -= sizeof(uint16_t);  
	}
	
	if(size)
	{  
		cksum += *(uint8_t*)pwbuffer;  
	}
	
	cksum=(cksum >> 16) + (cksum & 0xffff);  
	cksum+=(cksum >> 16);  

	return (unsigned short)(~cksum);  
}

static unsigned int inline cal_udp_checksum(unsigned char *buffer, int size)  
{  
	unsigned long sum = 0;  
	unsigned char rawBuf[2048];

	/* 伪IP头 */
	struct pseudo_hdr
	{  
		struct in_addr  src;  
		struct in_addr  dst;  
		uint8_t         mbz;  
		uint8_t         proto;  
		uint16_t        len;  
	} __attribute__((__packed__));
	
	int ph_len = sizeof(struct pseudo_hdr);  
	
	struct pseudo_hdr *ph;  
	ph = (struct pseudo_hdr *)rawBuf;

	/* DHCP Discover使用特殊的IP地址 */
	ph->src.s_addr = 0x0000;  
	ph->dst.s_addr = 0xffff;  
	ph->mbz = 0;  
	ph->proto = IPPROTO_UDP;

	/* buffer = udpheader + payload,  size = sizeof(udpheader + payload) */
	ph->len = htons(size); 
	memcpy(rawBuf + ph_len, buffer, size);   
	
	/* ph长＋UDP长＋payload长来计算checksum */
	sum = safe_checksum((unsigned short *)rawBuf, ph_len + size);  
	
	return sum;  
}
#endif /* CONFIG_UDP_CSUM */

static void fill_dhcp_header(unsigned char *buf)
{
	*(buf++) = 0x01;
	*(buf++) = 0x01;
	*(buf++) = 0x06;
	*(buf++) = 0x00;
}

static void fill_dhcp_xid(unsigned char *buf, unsigned int n)
{
	*(unsigned int*)(buf + 4) = htonl(n);
}

static void fill_dhcp_options(unsigned char *buf)
{
	unsigned char opts[] = 
	{
		0x63, 0x82, 0x53, 0x63,
		0x35, 0x01, 0x01,
		0x74, 0x01, 0x01,
		0x3d, 0x07, 0x01, 0x50, 0xe5, 0x49, 0x1d, 0xfd, 0x12,
		0x0c, 0x0a, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
		0x3c, 0x08, 0x4d, 0x53, 0x46, 0x54, 0x20, 0x35, 0x2e, 0x30,
		0x37, 0x0b, 0x01, 0x0f, 0x03, 0x06, 0x2c, 0x2e, 0x2f, 0x1f, 0x21, 0xf9, 0x2b,
		0xff
	};

	memcpy(buf + 236, opts, sizeof(opts));	
}

static void fill_dhcp_macaddr(unsigned char *buf)
{
	int i;
	for (i = 0; i < ETH_MACADDR_LEN; i++)
	{
		buf[i + 28] = dhcp_macAddr[i];
	}
}

static void fill_udp_len(unsigned char *buf, unsigned short len)
{
	*(unsigned short*)(buf + 4) = htons(len);
}

static void fill_ip_len(unsigned char *buf, unsigned short len)
{
	*(unsigned short*)(buf + 2) = htons(len);
}

static void fill_ip_checksum(unsigned char *buf)
{
	int i;
	unsigned short sum = 0;
	*(buf + 10) = 0;
	*(buf + 11) = 0;
	for (i = 0; i < 10; ++i)
	{
		sum += ntohs( *(unsigned short *)(buf + i * 2));
	}
	sum = ~sum + 1 - 3;
	
	*(unsigned short*)(buf + 10) = htons(sum);
}

static void fill_dstmac_broadcast(unsigned char *buf)
{
	int i;
	
	for (i = 0; i < ETH_MACADDR_LEN; ++i)
	{
		buf[i] = 0xff;
	}
}

static void fill_dhcp_srcmac(unsigned char *buf)
{
	int i;

	for (i = 0; i < ETH_MACADDR_LEN; i++)

	{
		buf[6 + i] = dhcp_macAddr[i];
	}
}

static void fill_pppoe_srcmac(unsigned char *buf)
{
	int i;

	for (i = 0; i < ETH_MACADDR_LEN; i++)

	{
		buf[6 + i] = pppoe_macAddr[i];
	}
}


static void fill_protocol_ip(unsigned char *buf)
{
	*(buf + 12) = 0x08;
	*(buf + 13) = 0x00;
}

static void fill_protocol_ip_with_vlan(unsigned char * buf)
{
	*(buf + 12) = 0x81;
	*(buf + 13) = 0x00;
	*(buf + 14) = 0x00;
	*(buf + 15) = 0x00;
	*(buf + 16) = 0x08;
	*(buf + 17) = 0x00;
}

void *send_dhcp_scan_packet(void *arg)
{
	int i;
	unsigned short csum;
	
	unsigned char packet[512];

	unsigned char dhcpData[236 + 54];
	unsigned char udpHeader[] ={0x00, 0x44, 0x00, 0x43, 0x00, 0x00, 0x00, 0x00};

	unsigned char ipLayerData[IPH_LEN + UDPH_LEN + DHCP_DATA_LEN];
	unsigned char ipHeader[] = 
	{
		0x45, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00,
		0x40, 0x11, 0x00, 0x00,0x00, 0x00, 0x00, 0x00,
		0xff, 0xff, 0xff, 0xff
	};
	
	unsigned char *pPacket = packet;

	/* 改变源MAC地址的3, 4, 5字节 */
	unsigned short *pSrcMacPos = (unsigned short*)(packet + 9);
	unsigned short *pVlanTagPos = (unsigned short*)(packet + 14);
	unsigned short *pUdpCsumPos = (unsigned short*)(packet + 40);
	unsigned int *pDhcpXidPos = (unsigned int*)(packet + ETH_HEADER_LEN_WITH_VLAN + IPH_LEN + UDPH_LEN + 4);	
	
	memset(dhcpData, 0, sizeof(dhcpData));
	fill_dhcp_header(dhcpData);
	fill_dhcp_macaddr(dhcpData);
	fill_dhcp_options(dhcpData);
	fill_dhcp_xid(dhcpData, 0);

	fill_udp_len(udpHeader, UDPH_LEN + DHCP_DATA_LEN);
	
	fill_ip_len(ipHeader, IPH_LEN + UDPH_LEN + DHCP_DATA_LEN);
	fill_ip_checksum(ipHeader);

	memcpy(ipLayerData, ipHeader, IPH_LEN);
	memcpy(ipLayerData + IPH_LEN, udpHeader, UDPH_LEN);
	memcpy(ipLayerData + IPH_LEN + UDPH_LEN, dhcpData, DHCP_DATA_LEN);

	fill_dstmac_broadcast(packet);
	fill_dhcp_srcmac(packet);

	/* 不带VLAN Tag */
	fill_protocol_ip(packet);
	memcpy(packet + ETH_HEADER_LEN, ipLayerData, IPH_LEN + UDPH_LEN + DHCP_DATA_LEN);


	/* 因为计算的时候直接按照buffer算的 */
	/* 所以赋值的时候不需要使用htons() */
#ifdef CONFIG_UDP_CSUM
	csum = cal_udp_checksum((packet + ETH_HEADER_LEN + IPH_LEN), (UDPH_LEN + DHCP_DATA_LEN));
	*pUdpCsumPos = csum;
#endif

	safe_write(sockfd, packet, ETH_HEADER_LEN + IPH_LEN + UDPH_LEN + DHCP_DATA_LEN);

	/* 发送带VLAN TAG的包 */
	fill_protocol_ip_with_vlan(packet);
	memcpy(packet + ETH_HEADER_LEN_WITH_VLAN, ipLayerData, IPH_LEN + UDPH_LEN + DHCP_DATA_LEN);
	pUdpCsumPos = (unsigned short*)(packet + 44);

	for (i = 1; i <= 4094; i++)
	{
		*pSrcMacPos = htons((unsigned short)i);
		*pVlanTagPos = htons(i);
		*pDhcpXidPos = htonl(i);	

#ifdef CONFIG_UDP_CSUM
		*pUdpCsumPos = ntohs(0);
		csum = cal_udp_checksum((packet + ETH_HEADER_LEN_WITH_VLAN + IPH_LEN), (UDPH_LEN + DHCP_DATA_LEN));
		*pUdpCsumPos = csum;
#endif

		safe_write(sockfd, packet, ETH_HEADER_LEN_WITH_VLAN + IPH_LEN + UDPH_LEN + DHCP_DATA_LEN);
	}
	
	usleep(SLEEP_TIME);
	dhcp_done = 1;

	return ((void *)0);
}

void *send_pppoe_scan_packet(void *arg)
{
	unsigned char pppoePacket[60] = {0};
	unsigned char packIndex = 0;
	int i;

	unsigned char pppoe_data[] = 
	{
		0x88, 0x63, 0x11, 0x09,
		0x00, 0x00, 0x00, 0x10,
		0x01, 0x01, 0x00, 0x00,
		0x01, 0x03, 0x00, 0x08,
		0x01, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x00
	};

	unsigned short *pVlanTagPos;
	unsigned short *pSrcMacPos;

	fill_dstmac_broadcast(pppoePacket);
	fill_pppoe_srcmac(pppoePacket);
	packIndex += 12;

	/* 不带VLAN Tag */
	memcpy(pppoePacket + packIndex, pppoe_data, sizeof(pppoe_data));
	safe_write(sockfd, pppoePacket, sizeof(pppoePacket));
	
	pppoePacket[packIndex++] = 0x81;
	pppoePacket[packIndex++] = 0x00;

	pSrcMacPos = (unsigned short*)&pppoePacket[9];
	pVlanTagPos = (unsigned short*)&pppoePacket[packIndex];
	
	pppoePacket[packIndex++] = 0x00;
	pppoePacket[packIndex++] = 0x00;

	/* 带VLAN Tag */
	memcpy(pppoePacket + packIndex, pppoe_data, sizeof(pppoe_data));
	packIndex += sizeof(pppoe_data);
	memset(pppoePacket + packIndex, 0, sizeof(pppoePacket) - packIndex);

	for(i = 4094; i >= 1; i--)
	{	
		*pSrcMacPos = htons((unsigned short)i);
		*pVlanTagPos = htons((unsigned short)i);
		safe_write(sockfd, pppoePacket, sizeof(pppoePacket));
	}
	
	usleep(SLEEP_TIME);
	pppoe_done = 1;
	
	return ((void *)0);
}

int userGuide(void)
{
	fprintf(stdout, "\nVID = 0 --> NO VLAN ID\n");
	fprintf(stdout, "\nAuthor: huangxiaolu@tp-link.net\n");
}

int pkt_process()
{
	int ret;
	pthread_t dhcp_tid, pppoe_tid;

	int length;
	unsigned char buf[2048];
	
	unsigned char pppoe_vid[4095];
	unsigned char dhcp_vid[4095];
	unsigned short id;
	unsigned char low, hight;

	memset(pppoe_vid, 0, sizeof(pppoe_vid));
	memset(dhcp_vid, 0, sizeof(dhcp_vid));
	
	ret = pthread_create(&dhcp_tid, NULL, send_dhcp_scan_packet, NULL);
	if(ret != 0)
	{
		fprintf(stdout, "pthread creat err\n");
		return -1;
	}

	ret = pthread_create(&pppoe_tid, NULL, send_pppoe_scan_packet, NULL);
	if(ret != 0)
	{
		fprintf(stdout, "pthread creat err\n");
		return -1;
	}

	while(!(pppoe_done && dhcp_done))
	{
	
		length = safe_read(sockfd, buf, 2048);

		if(length <= 0)
		{
			continue;
		}
		
		if((buf[12] == 0x81) && (buf[13] == 0x00))/*VLAN*/
		{
			hight = buf[14] & 0x0F;
			low = buf[15];
			id = hight * 256 + low;

			if((buf[2] == 0x0e) && (buf[16] == 0x88) && (buf[17] == 0x63))
			{
				pppoe_vid[id] = 1;
			}
			
			if(((buf[2] == 0xdc) || (buf[5] == 0xff)) && (buf[39] == 0x43) && (buf[41] == 0x44)) /* DHCP 单播包或广播报 */
			{
				dhcp_vid[id] = 1;
			}
		}
		
		else if((buf[12] == 0x88) && (buf[13] == 0x63) && (buf[2] == 0x0e)) /* PADO */
		{
			pppoe_vid[0] = 1; /* VLAN ID == 0，表示不带VLAN Tag */
		}	
		
		else if((buf[12] == 0x08) && (buf[13] == 0x00) && ((buf[5] == 0xff) || (buf[2] == 0xdc))
					&& (buf[35] ==0x43) && (buf[37] == 0x44))
		{
			dhcp_vid[0] = 1; 
		}
	}

	for(id = 0; id <= 4094; id++)
	{
		if(dhcp_vid[id])
		{
			fprintf(stdout, "DHCP  Available VLAN ID: %d\n", id);
		}
	}

	for(id = 0; id <= 4094; id++)
	{
		if(pppoe_vid[id])
		{
			fprintf(stdout, "PPPoE Available VLAN ID: %d\n", id);
		}
	}
	return 0;
}

int main(int argc, char **argv)
{	
	int ret, cmd;
	int flags;
	struct ifreq ifr = {0};
	struct sockaddr_ll
sll = {0};
	struct timeval timeout= {0};
	int idx = 0;
	
	if(argc != 1)
	{
		goto cmd_err;
	}

	/* 文件存在，说明扫描完成。重新扫描前，需要把它删除 */
	if (access("/var/tmp/vlan_scan_done", F_OK) == 0)
	{
		system("rm -rf /var/tmp/vlan_scan_done");
	}

	/* 如果存在，说明有另外的vidtool进程在执行, 返回 */
	if (access("/var/tmp/vlan_scan_start", F_OK) == 0)
	{
		return 0;
	}
	else
	{
		system("echo 1 > /var/tmp/vlan_scan_start");
	}
	

	/* Create the RAW socket. */
	sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(sockfd < 0)
	{
		fprintf(stdout, "socket err!\n");
		return -1;
	}

	strncpy(ifr.ifr_name, WAN_IF_NAME, IFNAMSIZ);
	
	if(ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0 )
	{
		fprintf(stdout, "ioctl get ifindex err\n");
	}
	idx = ifr.ifr_ifindex;
	
	if(ioctl(sockfd, SIOCGIFHWADDR, &ifr) < 0)
	{
		fprintf(stdout, "ioctl get macaddr err\n");
	}
	memcpy(pppoe_macAddr, ifr.ifr_hwaddr.sa_data, ETH_MACADDR_LEN);
	memcpy(dhcp_macAddr, ifr.ifr_hwaddr.sa_data, ETH_MACADDR_LEN);

	pppoe_macAddr[2] = 0x0e;
	dhcp_macAddr[2] = 0xdc;

	sll.sll_family = PF_PACKET;
	sll.sll_ifindex = idx;

	if(bind(sockfd, (struct sockaddr *)(&sll), sizeof(sll)) < 0 ) 
	{
		perror("");
		return -1;
	}

#if 1 /* TIME OUT */

	timeout.tv_sec = 0;
	timeout.tv_usec = RECV_TIME_OUT;

	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (void *)&timeout, sizeof(struct timeval)) < 0) 
	{
	    fprintf(stdout, "setsockopt err\n!");
	    return -1;
	}

	timeout.tv_sec = 0;
	timeout.tv_usec = SEND_TIME_OUT;

	if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (void *)&timeout, sizeof(struct timeval)) < 0) 
	{
		fprintf(stdout, "setsockopt err\n!");
		return -1;
	}

#endif

#if 0 /* NONBLOCK */

	flags = fcntl(sockfd, F_GETFL);
	fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

#endif

#if 0 /* PROMISC */

    if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0)
    {
        printf("ioctl(sockfd, SIOCGIFFLAGS, &ifr) err!\n");
        return -1;
    }

    ifr.ifr_flags |= IFF_PROMISC;
	
    if (ioctl(sockfd, SIOCSIFFLAGS, &ifr) < 0)
    {
        printf("ioctl(sockfd, SIOCSIFFLAGS, ifr) err\n!");
        return -1;
    }  

#endif

	ret = pkt_process();

	if(ret)
	{
		fprintf(stdout, "proccess error\n");
		system("rm       /var/tmp/vlan_scan_start");
		return -1;
	}
	else
	{
		system("rm       /var/tmp/vlan_scan_start");
		system("echo 1 > /var/tmp/vlan_scan_done");
	}

	return 0;
		
cmd_err:
	
	userGuide();

	return -1;
}