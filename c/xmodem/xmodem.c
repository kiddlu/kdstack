#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define XMODEM_NUL			0x00
#define XMODEM_SOH			0x01
#define XMODEM_STX			0x02
#define XMODEM_EOT			0x04
#define XMODEM_ACK			0x06
#define XMODEM_NAK			0x15
#define XMODEM_CAN			0x18
#define XMODEM_EOF			0x1A
#define XMODEM_STA			0xAA

#define XMODEM_BLOCK_HEAD_SIZE		1
#define XMODEM_BLOCK_NO_SIZE		     2
#define XMODEM_BLOCK_BODY_SIZE		128
#define XMODEM_BLOCK_CHK_SIZE		     1
#define XMODEM_BLOCK_ALL_SIZE          (1 + 2 + 128 + 1)


// int uart_send(unsigned char *send, const int length)	- ???ڷ??ͺ???
// int uart_recv(unsigned char *recv, const int length)	- ???ڽ??պ???

int send_xmodem(unsigned char *dataBuf, const int length, const int retryTimes)
{

	unsigned char sendPacket[XMODEM_BLOCK_ALL_SIZE];
	unsigned char recvPacket[XMODEM_BLOCK_ALL_SIZE];
	
	unsigned char blkNum = 0;
	unsigned char chkSum = 0;

	int retry = 0;
	int blkPos, dataPos;

	sprintf(sendPacket, "%c", XMODEM_STA);
	while(1)
	{
		uart_send(sendPacket, 1);
		sleep(10);

		if(uart_recv(recvPacket, sizeof(recvPacket)) && recvPacket[0] == XMODEM_NAK)
		{
			break;
		}
		else
		{
			retry++;
		}
		if(retry >= retryTimes)
		{
			return -ECOMM;
		}
	}
	retry = 0;


	for(blkPos = 0; blkPos < length; blkPos += XMODEM_BLOCK_BODY_SIZE)
	{
		memset(sendPacket, NULL, sizeof(sendPacket));

		if (blkNum == 0xFF)
		{
			blkNum = 0;
		}
		else
		{
			blkNum++;
		}

		chkSum = 0;
		
		sendPacket[0] = XMODEM_SOH;
		sendPacket[1] = blkNum;
		sendPacket[2] = (0xFF - blkNum);

		for (dataPos = 0; dataPos < XMODEM_BLOCK_BODY_SIZE; dataPos++)
		{
			if ((blkPos + dataPos) < length)
			{
				sendPacket[XMODEM_BLOCK_HEAD_SIZE + XMODEM_BLOCK_NO_SIZE + dataPos] = *(dataBuf + blkPos + dataPos)];
			}
			else
			{
				sendPacket[XMODEM_BLOCK_HEAD_SIZE + XMODEM_BLOCK_NO_SIZE + dataPos] = XMODEM_EOF;
			}
			chkSum += sendPacket[XMODEM_BLOCK_HEAD_SIZE + XMODEM_BLOCK_NO_SIZE + dataPos];
		}

		sendPacket[sizeof(sendPacket) - 1] = chkSum;

		uart_send(sendPacket, sizeof(sendPacket));

		while(1)
		{
			sleep(1);
			
			if(uart_recv(recvPacket, sizeof(recvPacket)))
			{
				if (recvPacket[0] == XMODEM_ACK)
				{
					break;
				}

				else	if(recvPacket[0] == XMODEM_NAK)
				{
					uart_send(sendPacket, sizeof(sendPacket));
					retry++;
				}
			}

			if (retry >= retryTimes)
			{
				sprintf(sendPacket, "%c%c%c%c%c", XMODEM_CAN, XMODEM_CAN, XMODEM_CAN, XMODEM_CAN, XMODEM_CAN);
				uart_send(sendPacket, 5);
				return -ECOMM;
			}
		}
	}

	sprintf(sendPacket, "%c%c%c%c%c", XMODEM_EOT, XMODEM_EOT, XMODEM_EOT, XMODEM_EOT, XMODEM_EOT);
	uart_send(sendPacket, 5);
	
	return 0;
}
