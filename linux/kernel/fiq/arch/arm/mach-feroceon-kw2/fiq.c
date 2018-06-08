#if 0
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/sysdev.h>

#include <mach/hardware.h>
#include <asm/io.h>
#include <asm/setup.h>
#include <asm/mach-types.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include "ctrlEnv/mvCtrlEnvLib.h"
#include "boardEnv/mvBoardEnvLib.h"
#include "gpp/mvGpp.h"
#include "mvOs.h"

/*
 * Note: FIQ mask registers are IRQ mask registers + 4 bytes.
 */

unsigned int fiq_int_type[NR_IRQS];

void mv_mask_fiq(unsigned int irq)
{
	if (irq < 32) {
		MV_REG_BIT_RESET(MV_FIQ_MASK_LOW_REG, (1 << irq) );
	} else if(irq < 64) {		/* (irq > 32 && irq < 64) */
		MV_REG_BIT_RESET(MV_FIQ_MASK_HIGH_REG, (1 << (irq - 32)) );
	} else if(irq < 96) {		/* (irq > 64 && irq < 96) */
		MV_REG_BIT_RESET(MV_FIQ_MASK_ERROR_REG, (1 << (irq - 64)) );
	} else if(irq < NR_IRQS) {	/* (irq > 96 && irq < NR_IRQS) */
		MV_U32 bitmask = 1 << (irq & 0x1F);
		MV_U32 reg = (irq - IRQ_GPP_START) >> 5;
		MV_REG_BIT_RESET(MV_GPP_IRQ_MASK_REG(reg), bitmask);
	} else
		printk("mv_mask_fiq: ERR, irq-%u out of scope\n",irq);
}

void mv_unmask_fiq(unsigned int irq)
{
	if (irq < 32) {
		MV_REG_BIT_SET(MV_FIQ_MASK_LOW_REG, (1 << irq) );
	} else if(irq < 64) {		/* (irq > 32 && irq < 64) */
		MV_REG_BIT_SET(MV_FIQ_MASK_HIGH_REG, (1 << (irq - 32)) );
	} else if(irq < 96) {		/* (irq > 64 && irq < 96) */
		MV_REG_BIT_SET(MV_FIQ_MASK_ERROR_REG, (1 << (irq - 64)) );
	} else if(irq < NR_IRQS) {
		MV_U32 bitmask = 1 << (irq & 0x1F);
		MV_U32 reg = (irq - IRQ_GPP_START) >> 5;
		MV_REG_BIT_SET(MV_GPP_IRQ_MASK_REG(reg), bitmask);
		if (fiq_int_type[irq] == GPP_IRQ_TYPE_CHANGE_LEVEL) {
			(MV_REG_READ(MV_GPP_IRQ_POLARITY(reg)) & bitmask)?
				MV_REG_BIT_RESET(MV_GPP_IRQ_POLARITY(reg), bitmask):
				MV_REG_BIT_SET(MV_GPP_IRQ_POLARITY(reg), bitmask);
		}
	} else
		printk("mv_unmask_fiq: ERR, irq-%u out of scope\n",irq);
}

irq_handler_t fiq_handlers[NR_IRQS] = {
	[0 ... NR_IRQS-1] = NULL,
};

static inline int find_fiq_bit(void)
{
	int bit, high_cause_sum_bit;
	MV_U32 mask;
	unsigned long cause;

	/*
	 * Handle Low Cause interrupts
	 */
	cause = MV_REG_READ(MV_IRQ_CAUSE_LOW_REG);

	/* Save High Cause Summary bit to save access to Cause register. */
	high_cause_sum_bit = cause & 0x1;

	cause &= MV_REG_READ(MV_FIQ_MASK_LOW_REG);
	bit = find_first_bit(&cause, 32);
	if (bit < 32)
		goto out;

	/*
	 * Handle High Cause interrupts
	 */
	if (!high_cause_sum_bit)
		goto check_cause_error;
	cause = MV_REG_READ(MV_IRQ_CAUSE_HIGH_REG);
	cause &= MV_REG_READ(MV_FIQ_MASK_HIGH_REG);
	bit = find_first_bit(&cause, 32);
	if (bit >= 32)
		goto check_cause_error;
	if (bit >= 4 && bit <= 15) {
		/* Handle GPIO case */
		bit = (bit - 4) * 8 + IRQ_GPP_START;
		if (bit > IRQ_GPP_START + 88) {
			/* Print some info in the future.
			 * Currently, this case may be traced
			 * only through FIQ exception counter
			 * (cat /proc/fiq_dump).
			 */
			goto err_out;
		}
	} else
		bit += 32;
	goto out;

	/*
	 * Handle Error Cause interrupts
	 */
check_cause_error:
	mask  = MV_REG_READ(MV_FIQ_MASK_ERROR_REG);
	cause = mask & MV_REG_READ(MV_IRQ_CAUSE_ERROR_REG);
	bit = find_first_bit(&cause, 32);
	if (bit < 32)
		goto out;

err_out:
	/* No interrupt found. */
	return -1;

out:
	return bit;
}

typedef void (*fiq_handler_t)(void);
extern fiq_handler_t fiq_userptr;

void handle_all_fiqs(void)
{
	irq_handler_t handler;
	int fiq = find_fiq_bit();

	handler = fiq_handlers[fiq];
	if (handler)
		handler(fiq, NULL);
}

int __must_check
request_fiq(unsigned int irq, irq_handler_t handler, unsigned long flags,
	    const char *name, void *dev)
{
	if ((irq < 0) | (irq >= NR_IRQS))
		return -EINVAL;

	/*
	 * if (flags & IRQF_DISABLED)
	 *	return -EINVAL;
	 */

	mv_mask_fiq(irq);

	if (!fiq_userptr)
		fiq_userptr = handle_all_fiqs;

	if (fiq_handlers[irq])
		return -EINVAL;

	fiq_handlers[irq] = handler;
	mv_unmask_fiq(irq);
	return 0;
}

void free_fiq(unsigned int irq, void *dev_id)
{
	mv_mask_fiq(irq);
	fiq_handlers[irq] = NULL;
}
#endif
/*
 *	linux/arch/arm/mach-feroceon/fiq.c
 *
 *	Written by Marvell
 *	Modified by Huang Xiaolu <kidd.dawny.lu@gmail.com>, 2014
 *
 *	Marvell Feroceon KW2 FIQ support.
 *
 *	GPLv2
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/sysdev.h>
#include <linux/kallsyms.h>

#include <mach/hardware.h>
#include <asm/io.h>
#include <asm/setup.h>
#include <asm/mach-types.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include "ctrlEnv/mvCtrlEnvLib.h"
#include "boardEnv/mvBoardEnvLib.h"
#include "gpp/mvGpp.h"
#include "mvOs.h"

unsigned int fiq_int_type[NR_IRQS];

inline unsigned int get_fiq_nr(void)
{
	int bit, high_cause_sum_bit;
	MV_U32 mask;
	unsigned long cause;

	/*
	 * Handle Low Cause interrupts
	 */
	cause = MV_REG_READ(MV_IRQ_CAUSE_LOW_REG);

	/* Save High Cause Summary bit to save access to Cause register. */
	high_cause_sum_bit = cause & 0x1;

	cause &= MV_REG_READ(MV_FIQ_MASK_LOW_REG);
	bit = find_first_bit(&cause, 32);
	if (bit < 32)
		goto out;

	/*
	 * Handle High Cause interrupts
	 */
	if (!high_cause_sum_bit)
		goto check_cause_error;
	cause = MV_REG_READ(MV_IRQ_CAUSE_HIGH_REG);
	cause &= MV_REG_READ(MV_FIQ_MASK_HIGH_REG);
	bit = find_first_bit(&cause, 32);
	if (bit >= 32)
		goto check_cause_error;
	if (bit >= 4 && bit <= 15) {
		/* Handle GPIO case */
		bit = (bit - 4) * 8 + IRQ_GPP_START;
		if (bit > IRQ_GPP_START + 88) {
			/* Print some info in the future.
			 * Currently, this case may be traced
			 * only through FIQ exception counter
			 * (cat /proc/fiq_dump).
			 */
			goto err_out;
		}
	} else
		bit += 32;
	goto out;

	/*
	 * Handle Error Cause interrupts
	 */
check_cause_error:
	mask  = MV_REG_READ(MV_FIQ_MASK_ERROR_REG);
	cause = mask & MV_REG_READ(MV_IRQ_CAUSE_ERROR_REG);
	bit = find_first_bit(&cause, 32);
	if (bit < 32)
		goto out;

err_out:
	/* No interrupt found. */
	return -1;

out:
	return bit;
}
EXPORT_SYMBOL(get_fiq_nr);

void chip_mask_fiq(unsigned int fiq)
{
	if (fiq < 32) 
	{
		MV_REG_BIT_RESET(MV_FIQ_MASK_LOW_REG, (1 << fiq) );
	} 
	else if(fiq < 64) 
	{
		MV_REG_BIT_RESET(MV_FIQ_MASK_HIGH_REG, (1 << (fiq - 32)) );
	}
	else if(fiq < 96)
	{
		MV_REG_BIT_RESET(MV_FIQ_MASK_ERROR_REG, (1 << (fiq - 64)) );
	}
	else if(fiq < NR_IRQS)
	{
		MV_U32 bitmask = 1 << (fiq & 0x1F);
		MV_U32 reg = (fiq - IRQ_GPP_START) >> 5;
		MV_REG_BIT_RESET(MV_GPP_IRQ_MASK_REG(reg), bitmask);
	}
	else
		printk("mv_mask_fiq: ERR, irq-%u out of scope\n",fiq);
}
EXPORT_SYMBOL(chip_mask_fiq);

void chip_unmask_fiq(unsigned int fiq)
{
	if (fiq < 32) 
	{
		MV_REG_BIT_SET(MV_FIQ_MASK_LOW_REG, (1 << fiq) );
	} else if(fiq < 64) 
	{ 
		MV_REG_BIT_SET(MV_FIQ_MASK_HIGH_REG, (1 << (fiq - 32)) );
	}
	else if(fiq < 96)
	{ 
		MV_REG_BIT_SET(MV_FIQ_MASK_ERROR_REG, (1 << (fiq - 64)) );
	}
	else if(fiq < NR_IRQS)
	{
		MV_U32 bitmask = 1 << (fiq & 0x1F);
		MV_U32 reg = (fiq - IRQ_GPP_START) >> 5;
		MV_REG_BIT_SET(MV_GPP_IRQ_MASK_REG(reg), bitmask);
		if (fiq_int_type[fiq] == GPP_IRQ_TYPE_CHANGE_LEVEL)
		{
			(MV_REG_READ(MV_GPP_IRQ_POLARITY(reg)) & bitmask)?
				MV_REG_BIT_RESET(MV_GPP_IRQ_POLARITY(reg), bitmask):
				MV_REG_BIT_SET(MV_GPP_IRQ_POLARITY(reg), bitmask);
		}
	}
	else
		printk("mv_unmask_fiq: ERR, irq-%u out of scope\n",fiq);
}
EXPORT_SYMBOL(chip_unmask_fiq);

int chip_fiq_debug_hook(struct pt_regs *regs)
{
	char ksym_pc[KSYM_SYMBOL_LEN];
	char ksym_lr[KSYM_SYMBOL_LEN];
			
	sprint_symbol(ksym_pc, regs->ARM_pc);
	sprint_symbol(ksym_lr, regs->ARM_lr);
			
	/* mmp.ko没有源码也没有符号表, 回溯会Panic */
	if(strstr(ksym_pc, "[mmp]") || strstr(ksym_lr, "[mmp]"))
	{
		printk("\nDon't Trace mmp.ko\n");
		return -1;
	}

	return 0;
}
EXPORT_SYMBOL(chip_fiq_debug_hook);
