/*
 *  linux/drivers/serial/serial_daemon.c
 *
 *  Written by Huang Xiaolu <kidd.dawny.lu@gmail.com>, 2014
 *
 *  Serial Log Daemon support.
 *
 *	GPLv2.
 */
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/circ_buf.h>
#include <linux/serial_daemon.h>

/* serial daemon buffer size is 4KB */
static struct uart_daemon serial_daemon;

/* write�ػ�app�����/dev/tty*�µ���Ϣ */
int serial_daemon_write(const unsigned char *data, unsigned int len)
{
	if(serial_daemon.sock && serial_daemon.pid)
	{
		if(serial_daemon.length + len > PAGE_SIZE)
			serial_daemon.length = 0;
		
		memcpy(serial_daemon.buf + serial_daemon.length, data, len);
		serial_daemon.length += len;		
	}
	return len;
}
EXPORT_SYMBOL(serial_daemon_write);

/*
	putchar���ڽ�ȡprintk�Ĵ�ӡ, �������kthread����֮��,
	prink��ӡ��������Ϣ[����һ���жϴ�������],
	��ǰ�����Ϣ�ᱻ����
*/
int serial_daemon_putchar(unsigned char ch)
{
	if(serial_daemon.sock && serial_daemon.pid)
	{
		if(serial_daemon.length + sizeof(ch) > PAGE_SIZE)
			serial_daemon.length = 0;

		serial_daemon.buf[serial_daemon.length] = ch;
		serial_daemon.length += sizeof(ch);
	}
	
	return 0;
}
EXPORT_SYMBOL(serial_daemon_putchar);

/* �����, �����ں��߳���� */
void serial_daemon_complete(void)
{
	if(serial_daemon.sock && serial_daemon.pid)
	{
		complete(&serial_daemon.completion);
	}
}
EXPORT_SYMBOL(serial_daemon_complete);

/* ѭ���ȴ�buf������, ��ʹ��nl��daemon�������ݵ��ں��߳� */
static int serial_daemon_kthread(void *data)
{
    struct sk_buff *skb_temp;
    struct nlmsghdr *nlh;

	for(;;)
	{			
		/* �߳�ѭ���ȴ����������� */
		wait_for_completion_interruptible(&serial_daemon.completion);

		/* netlink sock��daemon���̱��붼����, ���Ͳ������� */
		if(serial_daemon.sock && serial_daemon.pid && serial_daemon.length)
		{
		    skb_temp = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);;
		    if(!skb_temp)
		    {
		        printk("[seriald]: Alloc Skb Err\n");
		    }

			/* ���nlmsg��ͷ��, ����ʹ��nlmsg_flags��Ա�������ݳ��� */
		    nlh = nlmsg_put(skb_temp, 0, 0, 0, serial_daemon.length, serial_daemon.length);

			/* �����ں˵ĵ������� */
		    NETLINK_CB(skb_temp).pid = 0;
		    NETLINK_CB(skb_temp).dst_group = 0;

			/* ���skb��payload */
		    memcpy(NLMSG_DATA(nlh), serial_daemon.buf, serial_daemon.length);
			serial_daemon.length = 0;

			/* �����ض��Ľ��� */
		    netlink_unicast(serial_daemon.sock, skb_temp, serial_daemon.pid, MSG_DONTWAIT);
		}
	}

	return 0;
}

/* ����userland���͹����Ŀ������� */
static void serial_daemon_control(struct sk_buff *__skb)
{
	struct sk_buff *skb;
	struct nlmsghdr *nlh;

	struct task_struct *task = NULL;

	/* ��netlink�����������õ�skb */
	skb = skb_get (__skb);
	if(skb->len >= NLMSG_SPACE(0))
	{
		nlh = nlmsg_hdr(skb);

		/* ����, �û��ռ�ʹ��nlmsg_flags����������� */
		if(nlh->nlmsg_flags == CMD_ATTACH)
		{
			/* ���յ�Attach����, ȷ��daemon���̴��� */
			task = find_task_by_vpid(nlh->nlmsg_pid);
			if(task)
			{
				serial_daemon.pid = nlh->nlmsg_pid;
				printk("Attach! Serial Daemon %s[Pid:%d]\n", 
					task->comm, serial_daemon.pid);
			}
			else
			{
				/* �Ҳ���daemon���� */
				printk("No Such Proccess, Pid %d\n", nlh->nlmsg_pid);
			}
		}
		else if(nlh->nlmsg_flags == CMD_DETTACH)
		{
			/* daemon������Dettach���� */
			serial_daemon.pid = 0;
			printk("Serial Daemon Dettach!\n");
		}
		else
		{
			/* �޷�ʶ������ */
			printk("Do Not Support This Cmd: 0x%x\n", nlh->nlmsg_flags);
		}
        kfree_skb(skb);
    }
}

/* proc��д���� */
static int serial_daemon_proc_read(char *page, char **start, off_t off, 
	int count, int *eof, void *data)
{
	return sprintf(page, "Serial Daemon Pid = %d\n", serial_daemon.pid);
}

/* proc��д���� */
static int serial_daemon_proc_write(struct file *file, const char *buffer,
unsigned long count, void *data)
{
	char val_string[32] = {0};
	int value = -1;
	struct task_struct *task = NULL;
	
	if (count > sizeof(val_string) - 1)
		return -EINVAL;
	
	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;
	
	sscanf(val_string, "%d", &value);

	task = find_task_by_vpid(value);

	if(task)
	{
		serial_daemon.pid = value;
		printk("Attach! Serial Daemon %s[Pid:%d]\n", task->comm, serial_daemon.pid);
	}
	else
	{
		serial_daemon.pid = 0;
		printk("Serial Daemon Dettach!\n");
	}
	
	return count;
}

/* ��ʼ�� */
static int __init serial_daemon_init(void)
{
	struct proc_dir_entry *daemon_proc;
	
	/* ����netlinkl���� */
    serial_daemon.sock = netlink_kernel_create(&init_net, NETLINK_SERIAL_DAEMON, 1,
					serial_daemon_control, NULL, THIS_MODULE);

    if(!serial_daemon.sock)
	{
        printk(KERN_ERR "Serial Daemon Create Netlink Socket Err.\n");
        return -EBUSY;
    }

	/* ��ʼ��procfs */
	daemon_proc = create_proc_entry("tty/serial_daemon_pid", 0666, NULL);
	if (!daemon_proc)
	{
		printk("Failed To Create Proc File: /proc/tty/serial_daemon_pid\n");
		return -ENOENT;
	}
	
	daemon_proc->read_proc =  serial_daemon_proc_read;
	daemon_proc->write_proc = serial_daemon_proc_write;
	daemon_proc->nlink = 1;

	serial_daemon.buf = (char *)get_zeroed_page(GFP_KERNEL);
	serial_daemon.length = 0;
	
	/* ��ʼ������� */
	init_completion(&serial_daemon.completion);

	/* ��ʼ�����ݷ����߳� */
	serial_daemon.kthread = kthread_run(serial_daemon_kthread, NULL,"seriald");

	return 0;
}

/* �˳� */
static void __exit serial_daemon_exit(void)
{
    if(serial_daemon.sock != NULL)
	{
        sock_release(serial_daemon.sock->sk_socket);
    }

	serial_daemon.pid = 0;
}

module_init(serial_daemon_init);
module_exit(serial_daemon_exit);

MODULE_AUTHOR("Huang Xiaolu");
MODULE_DESCRIPTION("Serial Log Daemon Support");
MODULE_LICENSE("GPL");
