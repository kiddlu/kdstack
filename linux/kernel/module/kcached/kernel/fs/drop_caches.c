/*
 * Implement the manual drop-all-pagecache function
 */

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/writeback.h>
#include <linux/sysctl.h>
#include <linux/gfp.h>
#include <linux/kthread.h>
#include <linux/syscalls.h>


/* A global variable is a bit ugly, but it keeps the code simple */
int sysctl_drop_caches;

static struct completion kcached_cmp;

static void drop_pagecache_sb(struct super_block *sb)
{
	struct inode *inode, *toput_inode = NULL;

	spin_lock(&inode_lock);
	list_for_each_entry(inode, &sb->s_inodes, i_sb_list) {
		if (inode->i_state & (I_FREEING|I_CLEAR|I_WILL_FREE|I_NEW))
			continue;
		if (inode->i_mapping->nrpages == 0)
			continue;
		__iget(inode);
		spin_unlock(&inode_lock);
		invalidate_mapping_pages(inode->i_mapping, 0, -1);
		iput(toput_inode);
		toput_inode = inode;
		spin_lock(&inode_lock);
	}
	spin_unlock(&inode_lock);
	iput(toput_inode);
}

void drop_pagecache(void)
{
	struct super_block *sb;

	spin_lock(&sb_lock);
restart:
	list_for_each_entry(sb, &super_blocks, s_list) {
		sb->s_count++;
		spin_unlock(&sb_lock);
		down_read(&sb->s_umount);
		if (sb->s_root)
			drop_pagecache_sb(sb);
		up_read(&sb->s_umount);
		spin_lock(&sb_lock);
		if (__put_super_and_need_restart(sb))
			goto restart;
	}
	spin_unlock(&sb_lock);
}

static void drop_slab(void)
{
	int nr_objects;

	do {
		nr_objects = shrink_slab(1000, GFP_KERNEL, 1000);
	} while (nr_objects > 10);
}

int drop_caches_sysctl_handler(ctl_table *table, int write,
	void __user *buffer, size_t *length, loff_t *ppos)
{
	proc_dointvec_minmax(table, write, buffer, length, ppos);
	if (write) {
		if (sysctl_drop_caches & 1)
			drop_pagecache();
		if (sysctl_drop_caches & 2)
			drop_slab();	
	}
	return 0;
}

int kcached_run(void)
{
	complete(&kcached_cmp);
	return 0;
}

static int kcached_mng(void *unused)
{
	set_user_nice(current, -20);

	for(;;)
	{
		wait_for_completion_interruptible(&kcached_cmp);

		sys_sync();
		drop_pagecache();
		drop_slab();

		printk("[kcached] sync and drop cache\n");
		/* show_mem(); */
	}

	return 0;
}

static int __init kcached_init(void)
{
	struct task_struct *p;
	int err = 0;

	init_completion(&kcached_cmp);

	p = kthread_run(kcached_mng, NULL, "kcached");
	if (IS_ERR(p))
		err = PTR_ERR(p);

	return err;
}

static void __exit kcached_exit(void)
{
	return;
}

module_init(kcached_init);
module_exit(kcached_exit);

#if 0
#ifdef INCLUDE_KERNEL_DROPCACHE
#include <linux/kthread.h>
#include <linux/syscalls.h>
#include <linux/swap.h>
/*
 * display in kilobytes.
 */
#define KB(x) ((x) << (PAGE_SHIFT - 10))

#define MEM_THRESHOLD        11  /* 11/128 = 8.59% */

static int drop_cache_run(void *data)
{
    unsigned long threshold_mem = 0;
    unsigned long freeram = 0;

    set_current_state(TASK_UNINTERRUPTIBLE);
    schedule_timeout(HZ * 90);

    threshold_mem = (KB(totalram_pages) * MEM_THRESHOLD) >> 7;

    while (!kthread_should_stop())
    {
        freeram = global_page_state(NR_FREE_PAGES);

        if (KB(freeram) < threshold_mem)
        {
            sys_sync();
            drop_pagecache();
            drop_slab();
        }

        set_current_state(TASK_UNINTERRUPTIBLE);
        schedule_timeout(HZ * 2);
    }
    return 0;
}

static int __init drop_cache_init(void)
{
    struct task_struct *p;
    int error = 0;

    p = kthread_run(drop_cache_run, NULL, "dropcache");
    if (IS_ERR(p))
        error = PTR_ERR(p);

    return error;
}

module_init(drop_cache_init)
#endif /* INCLUDE_KERNEL_DROPCACHE */
#endif
