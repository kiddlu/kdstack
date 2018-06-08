#if 0
/*
 *  linux/arch/arm/kernel/fiq.c
 *
 *  Copyright (C) 1998 Russell King
 *  Copyright (C) 1998, 1999 Phil Blundell
 *
 *  FIQ support written by Philip Blundell <philb@gnu.org>, 1998.
 *
 *  FIQ support re-written by Russell King to be more generic
 *
 * We now properly support a method by which the FIQ handlers can
 * be stacked onto the vector.  We still do not support sharing
 * the FIQ vector itself.
 *
 * Operation is as follows:
 *  1. Owner A claims FIQ:
 *     - default_fiq relinquishes control.
 *  2. Owner A:
 *     - inserts code.
 *     - sets any registers,
 *     - enables FIQ.
 *  3. Owner B claims FIQ:
 *     - if owner A has a relinquish function.
 *       - disable FIQs.
 *       - saves any registers.
 *       - returns zero.
 *  4. Owner B:
 *     - inserts code.
 *     - sets any registers,
 *     - enables FIQ.
 *  5. Owner B releases FIQ:
 *     - Owner A is asked to reacquire FIQ:
 *	 - inserts code.
 *	 - restores saved registers.
 *	 - enables FIQ.
 *  6. Goto 3
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/seq_file.h>

#include <asm/cacheflush.h>
#include <asm/fiq.h>
#include <asm/irq.h>
#include <asm/system.h>

static unsigned long no_fiq_insn;

/* Default reacquire function
 * - we always relinquish FIQ control
 * - we always reacquire FIQ control
 */
static int fiq_def_op(void *ref, int relinquish)
{
	if (!relinquish)
		set_fiq_handler(&no_fiq_insn, sizeof(no_fiq_insn));

	return 0;
}

static struct fiq_handler default_owner = {
	.name	= "default",
	.fiq_op = fiq_def_op,
};

static struct fiq_handler *current_fiq = &default_owner;

int show_fiq_list(struct seq_file *p, void *v)
{
	if (current_fiq != &default_owner)
		seq_printf(p, "FIQ:              %s\n", current_fiq->name);

	return 0;
}

void set_fiq_handler(void *start, unsigned int length)
{
	memcpy((void *)0xffff001c, start, length);
	flush_icache_range(0xffff001c, 0xffff001c + length);
	if (!vectors_high())
		flush_icache_range(0x1c, 0x1c + length);
}

/*
 * Taking an interrupt in FIQ mode is death, so both these functions
 * disable irqs for the duration.  Note - these functions are almost
 * entirely coded in assembly.
 */
void __naked set_fiq_regs(struct pt_regs *regs)
{
	register unsigned long tmp;
	asm volatile (
	"mov	ip, sp\n\
	stmfd	sp!, {fp, ip, lr, pc}\n\
	sub	fp, ip, #4\n\
	mrs	%0, cpsr\n\
	msr	cpsr_c, %2	@ select FIQ mode\n\
	mov	r0, r0\n\
	ldmia	%1, {r8 - r14}\n\
	msr	cpsr_c, %0	@ return to SVC mode\n\
	mov	r0, r0\n\
	ldmfd	sp, {fp, sp, pc}"
	: "=&r" (tmp)
	: "r" (&regs->ARM_r8), "I" (PSR_I_BIT | PSR_F_BIT | FIQ_MODE));
}

void __naked get_fiq_regs(struct pt_regs *regs)
{
	register unsigned long tmp;
	asm volatile (
	"mov	ip, sp\n\
	stmfd	sp!, {fp, ip, lr, pc}\n\
	sub	fp, ip, #4\n\
	mrs	%0, cpsr\n\
	msr	cpsr_c, %2	@ select FIQ mode\n\
	mov	r0, r0\n\
	stmia	%1, {r8 - r14}\n\
	msr	cpsr_c, %0	@ return to SVC mode\n\
	mov	r0, r0\n\
	ldmfd	sp, {fp, sp, pc}"
	: "=&r" (tmp)
	: "r" (&regs->ARM_r8), "I" (PSR_I_BIT | PSR_F_BIT | FIQ_MODE));
}

int claim_fiq(struct fiq_handler *f)
{
	int ret = 0;

	if (current_fiq) {
		ret = -EBUSY;

		if (current_fiq->fiq_op != NULL)
			ret = current_fiq->fiq_op(current_fiq->dev_id, 1);
	}

	if (!ret) {
		f->next = current_fiq;
		current_fiq = f;
	}

	return ret;
}

void release_fiq(struct fiq_handler *f)
{
	if (current_fiq != f) {
		printk(KERN_ERR "%s FIQ trying to release %s FIQ\n",
		       f->name, current_fiq->name);
		dump_stack();
		return;
	}

	do
		current_fiq = current_fiq->next;
	while (current_fiq->fiq_op(current_fiq->dev_id, 0));
}

void enable_fiq(int fiq)
{
	enable_irq(fiq + FIQ_START);
}

void disable_fiq(int fiq)
{
	disable_irq(fiq + FIQ_START);
}

EXPORT_SYMBOL(set_fiq_handler);
EXPORT_SYMBOL(set_fiq_regs);
EXPORT_SYMBOL(get_fiq_regs);
EXPORT_SYMBOL(claim_fiq);
EXPORT_SYMBOL(release_fiq);
EXPORT_SYMBOL(enable_fiq);
EXPORT_SYMBOL(disable_fiq);

void __init init_FIQ(void)
{
	no_fiq_insn = *(unsigned long *)0xffff001c;
}
#endif

/*
 *  linux/arch/arm/kernel/fiq.c
 *
 *  Written by Huang Xiaolu <kidd.dawny.lu@gmail.com>, 2014
 *
 *  ARM FIQ [debug] support.
 *
 *	GPLv2.
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/init_task.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/sysdev.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/preempt.h>
#include <linux/kernel_stat.h>
#include <linux/hugetlb.h>
#include <linux/quicklist.h>
#include <linux/vmstat.h>
#include <linux/swap.h>
#include <linux/mman.h>
#include <linux/ipc_namespace.h>
#include <linux/sem.h>
#include <linux/msg.h>
#include <linux/shm.h>
#include "../../../fs/proc/internal.h"

#include <asm/io.h>
#include <asm/setup.h>
#include <asm/uaccess.h>
#include <asm/fiq.h>
#include <asm/system.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/irq.h>
#include <asm/mach/time.h>

/* arm/mach-%s/fiq.c */
/* 与芯片相关的API */
extern int get_fiq_nr(void);
extern void chip_mask_fiq(unsigned int fiq);
extern void chip_unmask_fiq(unsigned int fiq);
extern int chip_fiq_debug_hook(struct pt_regs *regs);

/* FIQ中断描述符 */
struct fiq_desc fiq_desc[NR_IRQS] __cacheline_aligned_in_smp = {
	[0 ... NR_IRQS-1] = {
		.name = NULL,
		.dev = NULL,
		.handler = NULL,
		.flags = 0,
		.count = 0,
	}
};

/* FIQ中断号转中断描述符 */
static inline struct fiq_desc *fiq_to_desc(unsigned int fiq)
{
	return (fiq < NR_IRQS) ? (fiq_desc + fiq) : NULL;
}

/* 进入FIQ前的寄存器环境, 调试用 */
static struct pt_regs *debug_regs;

/* Debug辅助函数 */
static void fiq_show_tasks(void)
{
	struct task_struct *p;

	printk("              NAME        PID   CPU   STATE       FLAGS\n");
	
	for_each_process(p)
	{
		printk("%18s\t%5d\t%2d\t%1d\t0x%08x\n",
			 p->comm, task_pid_nr(p), task_cpu(p), p->state, p->flags);
	}
}

/* Debug辅助函数 */
static void fiq_show_vmainfo(void)
{
	struct task_struct *p;
	struct mm_struct *mm;
	unsigned long data, text, lib;
	unsigned long total_vm, total_rss;

	printk("  PID   VmSize   VmRSS  VmData   VmStk   VmExe   VmLib   VmPTE\n");	
	for_each_process(p)
	{
		if((mm = p->mm) != NULL)
		{
			total_vm = mm->total_vm;
			total_rss = get_mm_rss(mm);
			
			data = mm->total_vm - mm->shared_vm - mm->stack_vm;
			text = (PAGE_ALIGN(mm->end_code) - (mm->start_code & PAGE_MASK)) >> 10;
			lib = (mm->exec_vm << (PAGE_SHIFT-10)) - text;
			
			printk("%5d\t%4lukB\t%4lukB\t%4lukB\t%4lukB"
				   "\t%4lukB\t%4lukB\t%4lukB\n",
				task_pid_nr(p),
				(total_vm - mm->reserved_vm) << (PAGE_SHIFT-10),
				total_rss << (PAGE_SHIFT-10),
				data << (PAGE_SHIFT-10),
				mm->stack_vm << (PAGE_SHIFT-10),
				text,
				lib,
				(PTRS_PER_PTE*sizeof(pte_t)*mm->nr_ptes) >> 10);

		}
	}
}

/* Debug辅助函数 */
static void fiq_show_interrupts(void)
{
	int i, j;
	struct irqaction * action;
	unsigned long flags;

	for(i=0; i<=NR_IRQS; i++)
	{
		if (i == 0)
		{
			printk("           ");
			for_each_online_cpu(j)
				printk("CPU%d       ", j);
			printk("\n");
		}

		if (i < NR_IRQS) 
		{
			spin_lock_irqsave(&irq_desc[i].lock, flags);
			action = irq_desc[i].action;
			if (!action)
				goto skip;
			printk("%3d: ", i);
			
#ifndef CONFIG_SMP
			printk("%10u ", kstat_irqs(i));
#else
			for_each_online_cpu(j)
				printk("%10u ", kstat_irqs_cpu(i, j));
#endif
			printk(" %14s", irq_desc[i].chip->name);
			printk("  %s", action->name);

			for (action=action->next; action; action = action->next)
				printk(", %s", action->name);

			printk("\n");
	skip:
			spin_lock_irqsave(&irq_desc[i].lock, flags);
		}
		else if (i == NR_IRQS)
		{
			extern unsigned long irq_err_count;
			printk("ERR: %10u\n", irq_err_count);
		}
	}
}

/* Debug辅助函数 */
/* kilobytes */
#define K(x) ((x) << (PAGE_SHIFT - 10))

static void fiq_show_meminfo(void)
{
	struct sysinfo i;
	unsigned long committed;
	unsigned long allowed;
	struct vmalloc_info vmi;
	long cached;
	unsigned long pages[NR_LRU_LISTS];
	int lru;

	si_meminfo(&i);
	si_swapinfo(&i);
	committed = percpu_counter_read_positive(&vm_committed_as);
	allowed = ((totalram_pages - hugetlb_total_pages())
		* sysctl_overcommit_ratio / 100) + total_swap_pages;

	cached = global_page_state(NR_FILE_PAGES) -
			total_swapcache_pages - i.bufferram;
	if (cached < 0)
		cached = 0;

	get_vmalloc_info(&vmi);

	for (lru = LRU_BASE; lru < NR_LRU_LISTS; lru++)
		pages[lru] = global_page_state(NR_LRU_BASE + lru);

	/*
	 * Tagged format, for easy grepping and expansion.
	 */
	printk(
		"MemTotal:       %8lu kB\n"
		"MemFree:        %8lu kB\n"
		"Buffers:        %8lu kB\n"
		"Cached:         %8lu kB\n"
		"SwapCached:     %8lu kB\n"
		"Active:         %8lu kB\n"
		"Inactive:       %8lu kB\n"
		"Active(anon):   %8lu kB\n"
		"Inactive(anon): %8lu kB\n"
		"Active(file):   %8lu kB\n"
		"Inactive(file): %8lu kB\n"
		"Unevictable:    %8lu kB\n"
		"Mlocked:        %8lu kB\n"
#ifdef CONFIG_HIGHMEM
		"HighTotal:      %8lu kB\n"
		"HighFree:       %8lu kB\n"
		"LowTotal:       %8lu kB\n"
		"LowFree:        %8lu kB\n"
#endif
#ifndef CONFIG_MMU
		"MmapCopy:       %8lu kB\n"
#endif
		"SwapTotal:      %8lu kB\n"
		"SwapFree:       %8lu kB\n"
		"Dirty:          %8lu kB\n"
		"Writeback:      %8lu kB\n"
		"AnonPages:      %8lu kB\n"
		"Mapped:         %8lu kB\n"
		"Shmem:          %8lu kB\n"
		"Slab:           %8lu kB\n"
		"SReclaimable:   %8lu kB\n"
		"SUnreclaim:     %8lu kB\n"
		"KernelStack:    %8lu kB\n"
		"PageTables:     %8lu kB\n"
#ifdef CONFIG_QUICKLIST
		"Quicklists:     %8lu kB\n"
#endif
		"NFS_Unstable:   %8lu kB\n"
		"Bounce:         %8lu kB\n"
		"WritebackTmp:   %8lu kB\n"
		"CommitLimit:    %8lu kB\n"
		"Committed_AS:   %8lu kB\n"
		"VmallocTotal:   %8lu kB\n"
		"VmallocUsed:    %8lu kB\n"
		"VmallocChunk:   %8lu kB\n"
#ifdef CONFIG_MEMORY_FAILURE
		"HardwareCorrupted: %5lu kB\n"
#endif
		,
		K(i.totalram),
		K(i.freeram),
		K(i.bufferram),
		K(cached),
		K(total_swapcache_pages),
		K(pages[LRU_ACTIVE_ANON]   + pages[LRU_ACTIVE_FILE]),
		K(pages[LRU_INACTIVE_ANON] + pages[LRU_INACTIVE_FILE]),
		K(pages[LRU_ACTIVE_ANON]),
		K(pages[LRU_INACTIVE_ANON]),
		K(pages[LRU_ACTIVE_FILE]),
		K(pages[LRU_INACTIVE_FILE]),
		K(pages[LRU_UNEVICTABLE]),
		K(global_page_state(NR_MLOCK)),
#ifdef CONFIG_HIGHMEM
		K(i.totalhigh),
		K(i.freehigh),
		K(i.totalram-i.totalhigh),
		K(i.freeram-i.freehigh),
#endif
#ifndef CONFIG_MMU
		K((unsigned long) atomic_long_read(&mmap_pages_allocated)),
#endif
		K(i.totalswap),
		K(i.freeswap),
		K(global_page_state(NR_FILE_DIRTY)),
		K(global_page_state(NR_WRITEBACK)),
		K(global_page_state(NR_ANON_PAGES)),
		K(global_page_state(NR_FILE_MAPPED)),
		K(global_page_state(NR_SHMEM)),
		K(global_page_state(NR_SLAB_RECLAIMABLE) +
				global_page_state(NR_SLAB_UNRECLAIMABLE)),
		K(global_page_state(NR_SLAB_RECLAIMABLE)),
		K(global_page_state(NR_SLAB_UNRECLAIMABLE)),
		global_page_state(NR_KERNEL_STACK) * THREAD_SIZE / 1024,
		K(global_page_state(NR_PAGETABLE)),
#ifdef CONFIG_QUICKLIST
		K(quicklist_total_size()),
#endif
		K(global_page_state(NR_UNSTABLE_NFS)),
		K(global_page_state(NR_BOUNCE)),
		K(global_page_state(NR_WRITEBACK_TEMP)),
		K(allowed),
		K(committed),
		(unsigned long)VMALLOC_TOTAL >> 10,
		vmi.used >> 10,
		vmi.largest_chunk >> 10
#ifdef CONFIG_MEMORY_FAILURE
		,atomic_long_read(&mce_bad_pages) << (PAGE_SHIFT - 10)
#endif
		);
}
#undef K

/* Debug辅助函数 */
#define IPC_SEM_IDS	0
#define IPC_MSG_IDS	1
#define IPC_SHM_IDS	2

static struct kern_ipc_perm *fiq_find_ipc(struct ipc_ids *ids, loff_t pos, loff_t *new_pos)
{
	struct kern_ipc_perm *ipc;
	int total, id;

	total = 0;
	for (id = 0; id < pos && total < ids->in_use; id++)
	{
		ipc = idr_find(&ids->ipcs_idr, id);
		if (ipc != NULL)
			total++;
	}

	if (total >= ids->in_use)
		return NULL;

	for ( ; pos < IPCMNI; pos++)
	{
		ipc = idr_find(&ids->ipcs_idr, pos);
		if (ipc != NULL)
		{
			*new_pos = pos + 1;
			return ipc;
		}
	}

	/* Out of range - return NULL to terminate iteration */
	return NULL;
}


static void fiq_show_ipcs(void)
{
	struct ipc_namespace *ns = &init_ipc_ns;
	struct ipc_ids *ids;
	loff_t pos;

	struct kern_ipc_perm *ipcp;
	struct shmid_kernel *shp;
	struct msg_queue *msq;
	struct sem_array *sma;

#ifdef CONFIG_SYSVIPC
	printk("/proc/sysvipc/sem:\n       key      semid perms      nsems\n");
	for(pos = 0, ids = &ns->ids[IPC_SEM_IDS];;)
	{
		ipcp = fiq_find_ipc(ids, pos, &pos);
		if(ipcp)
		{
			sma = container_of(ipcp, struct sem_array, sem_perm);

			printk("%10d %10d  %4o %10lu\n",
			  sma->sem_perm.key,
			  sma->sem_perm.id,
			  sma->sem_perm.mode,
			  sma->sem_nsems);
		}
		else
		{
			printk("\n");
			break;
		}
	}
	
	printk("/proc/sysvipc/msg:\n       key      msqid perms      cbytes       qnum lspid lrpid\n");
	for(pos = 0, ids = &ns->ids[IPC_MSG_IDS];;)
	{
		ipcp = fiq_find_ipc(ids, pos, &pos);
		if(ipcp)
		{
			msq = container_of(ipcp, struct msg_queue, q_perm);

			printk("%10d %10d  %4o  %10lu %10lu %5u %5u\n",
			msq->q_perm.key,
			msq->q_perm.id,
			msq->q_perm.mode,
			msq->q_cbytes,
			msq->q_qnum,
			msq->q_lspid,
			msq->q_lrpid);
		}
		else
		{
			printk("\n");
			break;
		}
	}

	printk("/proc/sysvipc/shm:\n       key      shmid perms       size  cpid  lpid nattch\n");
	for(pos = 0, ids = &ns->ids[IPC_SHM_IDS];;)
	{
		ipcp = fiq_find_ipc(ids, pos, &pos);
		if(ipcp)
		{
			shp = container_of(ipcp, struct shmid_kernel, shm_perm);

			printk("%10d %10d  %4o %10lu %5u %5u %5lu\n",
					  shp->shm_perm.key,
					  shp->shm_perm.id,
					  shp->shm_perm.mode,
					  shp->shm_segsz,
					  shp->shm_cprid,
					  shp->shm_lprid,
					  shp->shm_nattch);
		}
		else
		{
			printk("\n");
			break;
		}
	}
#endif	
}


/* Debug辅助函数 */
static void fiq_dump_mem(unsigned long start, unsigned int bytes)
{
	unsigned long end = start + bytes;

	unsigned long first;
	int i;

	mm_segment_t fs;
	fs = get_fs();
	set_fs(KERNEL_DS);
	
	printk("From 0x%08lx To 0x%08lx\n", start, end);

	for (first = start & ~31; first < end; first += 32) 
	{
		unsigned long p;
		char str[sizeof(" 12345678") * 8 + 1];

		memset(str, ' ', sizeof(str));
		str[sizeof(str) - 1] = '\0';

		for (p = first, i = 0; i < 8 && p < end; i++, p += 4)
		{
			if (p >= start && p < end)
			{
				sprintf(str + i * 9, " %08lx", (unsigned long)readl(p));
			}
		}
		printk("%04lx:%s\n", first & 0xffff, str);
	}

	set_fs(fs);
}

/* Debug辅助函数 */
static void fiq_dump_code(unsigned long addr)
{
	char str[sizeof("00000000 ") * 9 + 4 + 1], *p = str;
	int i;

	memset(str, 0, sizeof(str));
	str[sizeof(str) - 1] = '\0';

	mm_segment_t fs;
	fs = get_fs();
	set_fs(KERNEL_DS);

	for (i = -4; i < 5; i++) 
	{
		unsigned long val, bad;
		
		bad = __get_user(val, &((u32 *)addr)[i]);

		if (!bad)
		{
			p += sprintf(p, i == 0 ? "[<%08lx>] " : "%08lx ", val);
		}
		else 
		{
			p += sprintf(p, "Bad Value");
			break;
		}
	}
	printk("%s\n", str);

	set_fs(fs);
}

/* Debug信息, 确保注册的FIQ驱动有办法调用到这里 */
/* 一般在GPIO驱动或者UART驱动中调用 */
void show_fiq_debug(void)
{
	if(!debug_regs)
		return;

	if(processor_mode(debug_regs) == SVC_MODE)
	{
		if(chip_fiq_debug_hook(debug_regs))
		{	
			return;
		}
		
		/* 正常流程 */
		struct thread_info *thread;
		struct task_struct *tsk;
		
		/* 拿到thread和tsk信息 */
		thread = (struct thread_info *)((debug_regs->ARM_sp) & ~(THREAD_SIZE - 1));	
		tsk = thread->task;

		printk("\nTasks:\n");
		fiq_show_tasks();
		
		printk("\nVma:\n");
		fiq_show_vmainfo();

		printk("\nInterrupts:\n");
		fiq_show_interrupts();

		printk("\nMeminfo:\n");
		fiq_show_meminfo();

		printk("\nIpcs:\n");
		fiq_show_ipcs();
		
		printk("\nSysfile: \n");
		sysfs_printk_last_file();

		printk("\nModules: \n");
		print_modules();

		printk("\nResgiters: \n");
		printk("Pid: %d    Task: %s\n", task_pid_nr(tsk), tsk->comm);
		__show_regs(debug_regs);
		
		printk("\nStack: \n");
		unsigned int len = (THREAD_SIZE + (unsigned long)task_stack_page(tsk)) - debug_regs->ARM_sp;
		fiq_dump_mem(debug_regs->ARM_sp, len);
		
		printk("\nBacktrace: \n");
		c_backtrace(debug_regs->ARM_fp, processor_mode(debug_regs));

		printk("\nCode: \n");
		fiq_dump_code(debug_regs->ARM_pc);
	}
	else
	{
		printk("\nNot in SVC mode\nResgiters: \n");
		printk("mode = 0x%x\n", processor_mode(debug_regs));
		printk("pc = 0x%08lx\n", debug_regs->ARM_lr);
		printk("cpsr = 0x%08lx\n", debug_regs->ARM_cpsr);
	}
}

/* cat /proc/interrupts */
int show_fiq_list(struct seq_file *p, void *v)
{
	int fiq;
	struct fiq_desc *fiq_desc;
	for(fiq = 0; fiq < NR_IRQS; fiq++)
	{
		fiq_desc = fiq_to_desc(fiq);
		if(fiq_desc->handler)
		{
			seq_printf(p, "%3d: ", fiq);
			seq_printf(p, "%10u ", fiq_desc->count);
			seq_printf(p, " %10s", "FIQ");
			seq_printf(p, "  %s",  fiq_desc->name);			
			seq_putc(p, '\n');
		}
	}
	return 0;
}

/* FIQ中断注册函数 */
/* 除了flags无效之外, 中断处理例程与IRQ模式兼容 */
int __must_check request_fiq(unsigned int fiq, 
	void* handler, unsigned long flags,
	const char *name, void *dev)
{
	if ((fiq < 0) || (fiq >= NR_IRQS))
		return -EINVAL;

	if(handler == NULL)
		return -EINVAL;

	if (fiq_desc[fiq].handler) /* 已经被注册 */
		return -EINVAL;

	chip_mask_fiq(fiq);

	fiq_desc[fiq].name = name;
	fiq_desc[fiq].dev = dev;
	fiq_desc[fiq].flags = flags;
	fiq_desc[fiq].count = 0;
	fiq_desc[fiq].handler = handler;
	
	chip_unmask_fiq(fiq);
	
	return 0;
}

/* FIQ中断释放函数 */
void free_fiq(unsigned int fiq, void *dev_id)
{
	chip_mask_fiq(fiq);
	fiq_desc[fiq].name = NULL;
	fiq_desc[fiq].dev = NULL;
	fiq_desc[fiq].flags = 0;
	fiq_desc[fiq].count = 0;
	fiq_desc[fiq].handler = NULL;
}
static inline void fiq_enter(void)
{
	add_preempt_count(HARDIRQ_OFFSET);
}

static inline void fiq_exit(void)
{
	sub_preempt_count(HARDIRQ_OFFSET);
}

/* 根据FIQ号调用中断处理例程 */
static inline void generic_handle_fiq(unsigned int fiq)
{
	struct fiq_desc *fiq_decs = fiq_to_desc(fiq);
	
	if(fiq_decs->handler)
	{
		fiq_decs->handler(fiq, fiq_decs->dev);
		fiq_decs->count++;
	}
	else
		printk("No FIQ Handler\n");
}

/* FIQ中断的C语言处理入口 */
/* 尽量让FIQ伪装得像不可重入不可抢占的IRQ */
/* 这样才能比较好地欺骗中断处理例程 */
asmlinkage void __exception asm_do_FIQ(struct pt_regs *regs)
{
	debug_regs = regs;

	fiq_enter();

	unsigned int fiq = get_fiq_nr();

	if (unlikely(fiq >= NR_IRQS)) 
		printk("Bad FIQ %d\n", fiq);
	else
		generic_handle_fiq(fiq);
	
	fiq_exit();
	
	debug_regs = NULL;
}

/* 注意: FIQ对标准内核代码来说是不可屏蔽的 */
/* 既然不可屏蔽, 则内核中需要屏蔽中断后才执行的代码就有可能出问题 */
/* 正式使用前, 一定要进行大量测试 */
