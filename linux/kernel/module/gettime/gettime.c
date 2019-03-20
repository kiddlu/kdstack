#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/ktime.h>
#include <asm/uaccess.h>

/* proc read */
static int module_proc_read(char *page, char **start, off_t off, 
	int count, int *eof, void *data)
{
	return 0;
}

/* proc write */
static int module_proc_write(struct file *file, const char *buffer,
unsigned long count, void *data)
{
	char val_string[32] = {0};
	int value = -1;
	struct task_struct *task = NULL;
	ktime_t curr_monotime;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;
	
	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;
	
	sscanf(val_string, "%d", &value);

	if(value == 1) {
		curr_monotime = ktime_get_boottime();
		printk("<0>ktime_get_boottime is:%lld\n", curr_monotime.tv64);
	}

	return 0;
}

static int __init test_module_init(void)
{
	struct proc_dir_entry *module_proc;
	
	module_proc = create_proc_entry("test_module", 0666, NULL);
	if (!module_proc)
	{
		printk("Failed To Create Proc File: /proc/test_module\n");
		return -ENOENT;
	}
	
	module_proc->read_proc =  module_proc_read;
	module_proc->write_proc = module_proc_write;
	module_proc->nlink = 1;

	return 0;
}

static void __exit test_module_exit(void)
{
	return;
}

module_init(test_module_init);
module_exit(test_module_exit);

MODULE_AUTHOR("Huang Xiaolu");
MODULE_DESCRIPTION("test_module");
MODULE_LICENSE("GPL");
