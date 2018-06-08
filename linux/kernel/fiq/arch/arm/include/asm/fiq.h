#if 0
/*
 *  arch/arm/include/asm/fiq.h
 *
 * Support for FIQ on ARM architectures.
 * Written by Philip Blundell <philb@gnu.org>, 1998
 * Re-written by Russell King
 */

#ifndef __ASM_FIQ_H
#define __ASM_FIQ_H

#include <asm/ptrace.h>

struct fiq_handler {
	struct fiq_handler *next;
	/* Name
	 */
	const char *name;
	/* Called to ask driver to relinquish/
	 * reacquire FIQ
	 * return zero to accept, or -<errno>
	 */
	int (*fiq_op)(void *, int relinquish);
	/* data for the relinquish/reacquire functions
	 */
	void *dev_id;
};

extern int claim_fiq(struct fiq_handler *f);
extern void release_fiq(struct fiq_handler *f);
extern void set_fiq_handler(void *start, unsigned int length);
extern void set_fiq_regs(struct pt_regs *regs);
extern void get_fiq_regs(struct pt_regs *regs);
extern void enable_fiq(int fiq);
extern void disable_fiq(int fiq);
#endif
#endif
/*
 *	arch/arm/include/asm/fiq.h
 *
 *	Written by Huang Xiaolu <kidd.dawny.lu@gmail.com>, 2014
 *
 *	ARM FIQ [debug] support.
 *
 *	GPLv2.
 *
 */
 
#ifndef __ASM_FIQ_H
#define __ASM_FIQ_H

struct fiq_desc {
	const char *name;
	void *dev;
	unsigned long flags;
	unsigned int count;
	void (*handler)(unsigned int fiq, void *dev);
};

extern void show_fiq_debug(void);

extern int __must_check request_fiq(unsigned int fiq, 
	void* handler, unsigned long flags,
	const char *name, void *dev);
extern void free_fiq(unsigned int fiq, void *dev_id);

#endif
