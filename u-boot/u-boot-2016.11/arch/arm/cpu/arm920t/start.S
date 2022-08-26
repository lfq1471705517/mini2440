/*
 *  armboot - Startup Code for ARM920 CPU-core
 *
 *  Copyright (c) 2001	Marius Gröger <mag@sysgo.de>
 *  Copyright (c) 2002	Alex Züpke <azu@sysgo.de>
 *  Copyright (c) 2002	Gary Jennejohn <garyj@denx.de>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm-offsets.h>
#include <common.h>
#include <config.h>

/*
 *************************************************************************
 *
 * Startup Code (called from the ARM reset exception vector)
 *
 * do important init only if we don't start from memory!
 * relocate armboot to ram
 * setup stack
 * jump to second stage
 *
 *************************************************************************
 */

	.globl	reset

reset:
	/*
	 * set the cpu to SVC32 mode
	 */
	mrs	r0, cpsr
	bic	r0, r0, #0x1f
	orr	r0, r0, #0xd3
	msr	cpsr, r0

#if	defined(CONFIG_AT91RM9200DK) || defined(CONFIG_AT91RM9200EK)
	/*
	 * relocate exception table
	 */
	ldr	r0, =_start
	ldr	r1, =0x0
	mov	r2, #16
copyex:
	subs	r2, r2, #1
	ldr	r3, [r0], #4
	str	r3, [r1], #4
	bne	copyex
#endif

#ifdef CONFIG_S3C24X0
	/* turn off the watchdog */

# if defined(CONFIG_S3C2400)
#  define pWTCON	0x15300000
#  define INTMSK	0x14400008	/* Interrupt-Controller base addresses */
#  define CLKDIVN	0x14800014	/* clock divisor register */
#else
#  define pWTCON	0x53000000
#  define INTMSK	0x4A000008	/* Interrupt-Controller base addresses */
#  define INTSUBMSK	0x4A00001C
#  define CLKDIVN	0x4C000014	/* clock divisor register */
# endif
#if defined(__MINI2440_PERSONALIZED__)
#define CLK_CTL_BASE	0x4C000000	/* tekkaman */
#define MDIV_405	0x7f << 12	/* tekkaman */
#define PSDIV_405	0x21
#endif

	ldr	r0, =pWTCON
	mov	r1, #0x0
	str	r1, [r0]

	/*
	 * mask all IRQs by setting all bits in the INTMR - default
	 */
	mov	r1, #0xffffffff
	ldr	r0, =INTMSK
	str	r1, [r0]

#if defined(__MINI2440_PERSONALIZED__)
	ldr	r1, =0x7fff	
	ldr	r0, =INTSUBMSK
	str	r1, [r0]
#else
# if defined(CONFIG_S3C2410)
	ldr	r1, =0x3ff
	ldr	r0, =INTSUBMSK
	str	r1, [r0]
# endif
#endif

#if defined(__MINI2440_PERSONALIZED__)
	/* FCLK:HCLK:PCLK = 1:4:8 */
	ldr	r0, =CLKDIVN
	mov	r1, #5
	str	r1, [r0]
	
	mrc	p15, 0, r1, c1, c0, 0	
	orr	r1, r1, #0xc0000000		
	mcr	p15, 0, r1, c1, c0, 0	
	
	
	mov	r1, #CLK_CTL_BASE	
	mov	r2, #MDIV_405	
	add	r2, r2, #PSDIV_405	
	str	r2, [r1, #0x04]		/* MPLLCON tekkaman */

#else
	/* FCLK:HCLK:PCLK = 1:2:4 */
	/* default FCLK is 120 MHz ! */
	ldr	r0, =CLKDIVN
	mov	r1, #3
	str	r1, [r0]
#endif
#endif	/* CONFIG_S3C24X0 */

	/*
	 * we do sys-critical inits only at reboot,
	 * not when booting from ram!
	 */
#ifndef CONFIG_SKIP_LOWLEVEL_INIT
	bl	cpu_init_crit
#endif

#if defined(__MINI2440_PERSONALIZED__)
/***************** NAND_BOOT *************************************************/
#define LENGTH_UBOOT	UBOOT_RELOCATE_COPY_SIZE
#define NAND_CTL_BASE	0x4E000000

#ifdef CONFIG_S3C2440
/* Offset */
#define oNFCONF 0x00
#define oNFCONT 0x04
#define oNFCMD 0x08
#define oNFSTAT 0x20

	@ reset NAND
	mov	r1, #NAND_CTL_BASE
	ldr	r2, =( (7<<12)|(7<<8)|(7<<4)|(0<<0) )
	str	r2, [r1, #oNFCONF]
	ldr	r2, [r1, #oNFCONF]
	
	ldr	r2, =( (1<<4)|(0<<1)|(1<<0) )	@ Active low CE Control 
	str	r2, [r1, #oNFCONT]
	ldr	r2, [r1, #oNFCONT]
	
	ldr	r2, =(0x6)	@ RnB Clear
	str	r2, [r1, #oNFSTAT]
	ldr	r2, [r1, #oNFSTAT]
	
	mov	r2, #0xff	@ RESET command
	strb	r2, [r1, #oNFCMD]
	
	mov	r3, #0	@ wait
nand1: 
	add	r3, r3, #0x1
	cmp	r3, #0xa
	blt	nand1

nand2:
	ldr	r2, [r1, #oNFSTAT]	@ wait ready
	tst	r2, #0x4
	beq	nand2
	
	
	ldr	r2, [r1, #oNFCONT]
	orr	r2, r2, #0x2	@ Flash Memory Chip Disable
	str	r2, [r1, #oNFCONT]
	
	@ get read to call C functions (for nand_read())
	ldr	sp, =TMP_STACK_ADDR	@ setup stack pointer
	mov	fp, #0	@ no previous frame, so fp=0

	@ copy U-Boot to RAM
	ldr	r0, =CONFIG_SYS_TEXT_BASE
	mov	r1, #0x0
	mov	r2, #LENGTH_UBOOT
/*
	mov	r0, #0x32000000
	mov	r1, #0xa5
	str	r1, [r0]
	mov	r1, #0x00
	ldr	r1, [r0]
	cmp	r1, #0xa5
	bne	open_leds

tmp:
	b	tmp

open_leds:
#if 1
#define GPIO_CTL_BASE 0x56000000
#define oGPIO_B 0x10
#define oGPIO_CON 0x0
#define oGPIO_DAT 0x4
#define oGPIO_UP 0x8
	mov	r1, #GPIO_CTL_BASE
	add	r1, r1, #oGPIO_B

	ldr	r2, [r1, #oGPIO_CON]
	bic	r2, r2, #0x0003fc00
	orr	r2, r2, #0x00015400
	str	r2, [r1, #oGPIO_CON]

	ldr	r2, [r1, #oGPIO_UP]
	bic	r2, r2, #0x1e0
	orr	r2, r2, #0x1e0
	str	r2, [r1, #oGPIO_UP]

	ldr	r2, [r1, #oGPIO_DAT]
	bic	r2, r2, #0x1e0
	str	r2, [r1, #oGPIO_DAT]

#endif
	*/

	bl	nand_read_ll
	tst	r0, #0x0
	beq	ok_nand_read

loop2:
	b	loop2	@ infinite loop
ok_nand_read:
	@ verify
	mov	r0, #0
	ldr	r1, =CONFIG_SYS_TEXT_BASE
	mov	r2, #0x400	@ 4 bytes * 1024 = 4Kbytes
go_next:
	ldr	r3, [r0], #4
	ldr	r4, [r1], #4
	teq	r3, r4
	bne	notmatch
	subs	r2, r2, #4
	bne	go_next
	b	enter_to_main

notmatch:
loop3:
	b	loop3	@ infinite loop
#endif
/***************** NAND_BOOT *************************************************/

enter_to_main:
	/* jump to sdram */
	ldr	pc, =_main
#else
 	bl	_main
#endif

/*------------------------------------------------------------------------------*/

	.globl	c_runtime_cpu_setup
c_runtime_cpu_setup:

	mov	pc, lr

/*
 *************************************************************************
 *
 * CPU_init_critical registers
 *
 * setup important registers
 * setup memory timing
 *
 *************************************************************************
 */


#ifndef CONFIG_SKIP_LOWLEVEL_INIT
cpu_init_crit:
	/*
	 * flush v4 I/D caches
	 */
	mov	r0, #0
	mcr	p15, 0, r0, c7, c7, 0	/* flush v3/v4 cache */
	mcr	p15, 0, r0, c8, c7, 0	/* flush v4 TLB */

	/*
	 * disable MMU stuff and caches
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002300	@ clear bits 13, 9:8 (--V- --RS)
	bic	r0, r0, #0x00000087	@ clear bits 7, 2:0 (B--- -CAM)
	orr	r0, r0, #0x00000002	@ set bit 1 (A) Align
	orr	r0, r0, #0x00001000	@ set bit 12 (I) I-Cache
	mcr	p15, 0, r0, c1, c0, 0

#ifndef CONFIG_SKIP_LOWLEVEL_INIT_ONLY
	/*
	 * before relocating, we have to setup RAM timing
	 * because memory timing is board-dependend, you will
	 * find a lowlevel_init.S in your board directory.
	 */
	mov	ip, lr

	bl	lowlevel_init
	mov	lr, ip
#endif
	mov	pc, lr
#endif /* CONFIG_SKIP_LOWLEVEL_INIT */
