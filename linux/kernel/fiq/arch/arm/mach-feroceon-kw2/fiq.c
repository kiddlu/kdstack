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

int chip_fiq_debug_hook(struct pt_regs *regs)
{
	char ksym_pc[KSYM_SYMBOL_LEN];
	char ksym_lr[KSYM_SYMBOL_LEN];
			
	sprint_symbol(ksym_pc, regs->ARM_pc);
	sprint_symbol(ksym_lr, regs->ARM_lr);
			
	if(strstr(ksym_pc, "[mmp]") || strstr(ksym_lr, "[mmp]"))
	{
		printk("\nDon't Trace mmp.ko\n");
		return -1;
	}

	return 0;
}
EXPORT_SYMBOL(chip_fiq_debug_hook);
