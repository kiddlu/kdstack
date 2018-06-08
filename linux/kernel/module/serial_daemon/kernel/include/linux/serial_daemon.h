/*
 *  include/linux/serial_daemon.h
 *
 *  Written by Huang Xiaolu <kidd.dawny.lu@gmail.com>, 2014
 *
 *  Serial Log Daemon support.
 *
 *	GPLv2.
 */

#ifndef LINUX_SERIAL_DAEMON_H
#define LINUX_SERIAL_DAEMON_H

#include <net/sock.h>
#include <net/netlink.h> 
#include <linux/completion.h>
#include <linux/kthread.h>

#define NETLINK_SERIAL_DAEMON 25

#define CMD_ATTACH	(NLMSG_MIN_TYPE + 0x1)
#define CMD_DETTACH	(NLMSG_MIN_TYPE + 0x2)

struct uart_daemon
{
	char *buf;
	unsigned long length;

	pid_t pid;
	struct sock *sock;
	struct task_struct *kthread;
	struct completion completion;
};

extern int serial_daemon_write(const unsigned char *data, unsigned int len);
extern int serial_daemon_putchar(unsigned char ch);
extern void serial_daemon_complete(void);

#endif /* LINUX_SERIAL_DAEMON_H */
