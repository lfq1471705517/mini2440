/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 * Gary Jennejohn <garyj@denx.de>
 * David Mueller <d.mueller@elsoft.ch>
 *
 * Configuation settings for the SAMSUNG SMDK2410 board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_S3C24X0		/* This is a SAMSUNG S3C24x0-type SoC */
#define CONFIG_S3C2440		/* specifically a SAMSUNG S3C2440 SoC */
#define CONFIG_MINI2440		/* on a friendlyarm mini2440 Board */

/* 32MB */
#define CONFIG_SYS_TEXT_BASE	0x32000000//0x33f80000//0x0

#define	CONFIG_SYS_LOAD_ADDR		0x30008000

#define TMP_STACK_ADDR	0x31f00000
#define TMP_STACK_SIZE	0x10000

#define CONFIG_S3C2410_NAND_SKIP_BAD	1

#define CONFIG_SYS_ARM_CACHE_WRITETHROUGH

/* input clock of PLL (the SMDK2410 has 12MHz input clock) */
#define CONFIG_SYS_CLK_FREQ	12000000

#define CONFIG_CMDLINE_TAG	/* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG

/*
 * Hardware drivers
 */
#define CONFIG_CMD_NET	1
#define CONFIG_CMD_PING	1

#define CONFIG_NET_MULTI 1
#define CONFIG_NET_RETRY_COUNT 20
#define CONFIG_DRIVER_DM9000 1
#define CONFIG_DM9000_BASE 0x20000300
#define DM9000_IO CONFIG_DM9000_BASE
#define DM9000_DATA (CONFIG_DM9000_BASE+4)
#define CONFIG_DM9000_USE_16BIT 1
#define CONFIG_DM9000_NO_SROM 1

/*
 * select serial console configuration
 */
#define CONFIG_S3C24X0_SERIAL
#define CONFIG_SERIAL1		1	/* we use SERIAL 1 on SMDK2410 */

/************************************************************
 * USB support (currently only works with D-cache off)
 ************************************************************/
/*
#define CONFIG_USB_OHCI
#define CONFIG_USB_OHCI_S3C24XX
#define CONFIG_DOS_PARTITION
*/

/************************************************************
 * RTC
 ************************************************************/
#define CONFIG_RTC_S3C24X0

#define CONFIG_BAUDRATE		115200

/*
 * BOOTP options
 */
#define CONFIG_BOOTP_BOOTFILESIZE
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME

/*
 * Command line configuration.
 */
#define CONFIG_CMD_BSP
#define CONFIG_CMD_DATE
#define CONFIG_CMD_NAND
#define CONFIG_CMD_REGINFO

#define CONFIG_CMDLINE_EDITING

/* autoboot */
#define CONFIG_BOOT_RETRY_TIME	-1
#define CONFIG_RESET_TO_RETRY

#define CONFIG_LIB_RAND	1
#define CONFIG_NET_RANDOM_ETHADDR
#define CONFIG_NETMASK 255.255.255.0
#define CONFIG_IPADDR 192.168.31.10
#define CONFIG_SERVERIP 192.168.31.237
#define CONFIG_GATEWAYIP 192.168.31.1
#define CONFIG_OVERWRITE_ETHADDR_ONCE

#if defined(CONFIG_CMD_KGDB)
#define CONFIG_KGDB_BAUDRATE	115200	/* speed to run kgdb serial port */
#endif

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_SYS_CBSIZE	256
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE	(CONFIG_SYS_CBSIZE + \
				sizeof(CONFIG_SYS_PROMPT)+16)
#define CONFIG_SYS_MAXARGS	16
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE

#define CONFIG_SYS_MEMTEST_START	0x30000000	/* memtest works on */
#define CONFIG_SYS_MEMTEST_END		0x33F00000	/* 63 MB in DRAM */

/* support additional compression methods */
/* #define CONFIG_BZIP2 
#define CONFIG_LZO
#define CONFIG_LZMA */

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS	1          /* we have 1 bank of DRAM */
#define PHYS_SDRAM_1		0x30000000 /* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE	0x04000000 /* 64 MB */

#define PHYS_FLASH_1		0x00000000 /* Flash Bank #0 */

#define CONFIG_SYS_FLASH_BASE	PHYS_FLASH_1

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */

#define CONFIG_SYS_FLASH_CFI
#define CONFIG_FLASH_CFI_DRIVER
#define CONFIG_FLASH_CFI_LEGACY
#define CONFIG_SYS_FLASH_LEGACY_512Kx16
#define CONFIG_FLASH_SHOW_PROGRESS	45

#define CONFIG_SYS_MAX_FLASH_BANKS	1
#define CONFIG_SYS_FLASH_BANKS_LIST     { CONFIG_SYS_FLASH_BASE }

#define CONFIG_ENV_OFFSET		(0x100000)
#define CONFIG_ENV_IS_IN_NAND
#define CONFIG_ENV_SIZE			0x100000	/* Total Size of Environment Sector */
/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

#define CONFIG_SST_VF1601		1

#ifdef CONFIG_SST_VF1601
#define PHYS_FLASH_SIZE 0x00200000 /* 2MB */
#define CONFIG_SYS_MAX_FLASH_SECT (32) /* max number of sectors on one chip */
#define CONFIG_ENV_ADDR (CONFIG_SYS_FLASH_BASE + CONFIG_ENV_OFFSET)
#endif
/*
 * Size of malloc() pool
 * BZIP2 / LZO / LZMA need a lot of RAM
 */
/* #define CONFIG_SYS_MALLOC_LEN	(4 * 1024 * 1024) */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 128*1024)
#define CONFIG_SYS_GBL_DATA_SIZE	128	/* size in bytes reserved for initial data */


#define CONFIG_SYS_MONITOR_LEN	(448 * 1024)
#define CONFIG_SYS_MONITOR_BASE	CONFIG_SYS_FLASH_BASE

/*
 * NAND configuration
 */
#ifdef CONFIG_CMD_NAND
#define CONFIG_NAND_S3C2410
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_BASE		0x4E000000

#define SECTORSIZE 512
#define SECTORSIZE_2K 2048
#define NAND_SECTOR_SIZE SECTORSIZE
#define NAND_SECTOR_SIZE_2K SECTORSIZE_2K
#define NAND_BLOCK_MASK 511
#define NAND_BLOCK_MASK_2K 2047
#define NAND_MAX_CHIPS 1
#define CONFIG_MTD_NAND_VERIFY_WRITE
#define CONFIG_SYS_64BIT_VSPRINTF /* needed for nand_util.c */
#endif

/*
 * File system
 */
/* #define CONFIG_CMD_UBIFS */
/* #define CONFIG_YAFFS2 */
/* #define CONFIG_RBTREE */

/* additions for new relocation code, must be added to all boards */
#define CONFIG_SYS_SDRAM_BASE	PHYS_SDRAM_1
#define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_SDRAM_BASE + 0x20000 - \
				GENERATED_GBL_DATA_SIZE)

#define CONFIG_BOARD_EARLY_INIT_F

#define	CONFIG_EXTRA_ENV_SETTINGS					\
	"embedclub=bmp d 70000\0 "				\
	"stdin=serial\0"					\
	"stdout=serial\0"					\
	"stderr=serial\0"					\
	"lcdtype=w\0"	\
	""

#define CONFIG_CMD_MTDPARTS		1
#define CONFIG_MTD_DEVICE		1
#define CONFIG_MTD_PARTITIONS		1  
/*    
#define CONFIG_CMD_UBIFS           
#define CONFIG_CMD_UBI         
#define CONFIG_LZO			1
#define CONFIG_RBTREE			1
*/

/*
#define MTDIDS_DEFAULT "nand0=nandflash0"
#define MTDPARTS_DEFAULT "mtdparts=nandflash0:1m(u-boot)," \
 "1m(u-boot-env)," \
 "5m(kernel)," \
 "24m(rootfs)"
*/
#define MTDIDS_DEFAULT "nand0=nandflash0"
#define MTDPARTS_DEFAULT "mtdparts=nandflash0:1m(u-boot)," \
					     "1m(u-boot-env)," \
					     "5m(kernel)," \
					     "-(rootfs)"

/* 1MB */
#define UBOOT_RELOCATE_COPY_SIZE	0x100000

#define CONFIG_CMD_BOOTZ		1

/*
#define CONFIG_OF_LIBFDT		1
#define CONFIG_CMD_FDT			1
*/

#define CONFIG_BOOTCOMMAND	"nand read 0x30008000 0x200000 0x500000;bootz 0x30008000"
#define CONFIG_BOOTARGS    	"root=/dev/mmcblk0p1 rootwait rw rootfstype=ext4 init=/linuxrc console=ttySAC0"
/*#define CONFIG_BOOTARGS		"ubi.mtd=3 root=ubi0:rootfs rootfstype=ubifs console=ttySAC0"*/

#endif /* __CONFIG_H */
