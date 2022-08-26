/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002, 2010
 * David Mueller, ELSOFT AG, <d.mueller@elsoft.ch>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <netdev.h>
#include <asm/io.h>
#include <asm/arch/s3c24x0_cpu.h>
#include <video_fb.h>

#if defined(CONFIG_CMD_NAND)
#include <linux/mtd/nand.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

#define FCLK_SPEED 1

#if (FCLK_SPEED == 0)		/* Fout = 203MHz, Fin = 12MHz for Audio */
#define M_MDIV	0xC3
#define M_PDIV	0x4
#define M_SDIV	0x1
#elif (FCLK_SPEED == 1)		/* Fout = 202.8MHz */
#if defined(CONFIG_S3C2410)
/* Fout = 202.8MHz */
#define M_MDIV	0xA1
#define M_PDIV	0x3
#define M_SDIV	0x1
#endif

#if defined(CONFIG_S3C2440)
/* Fout = 405MHz */
#define M_MDIV 0x7f	
#define M_PDIV 0x2
#define M_SDIV 0x1
#endif
#endif
#define USB_CLOCK 1

#if (USB_CLOCK == 0)
#define U_M_MDIV	0xA1
#define U_M_PDIV	0x3
#define U_M_SDIV	0x1
#elif (USB_CLOCK == 1)
#if defined(CONFIG_S3C2410)
#define U_M_MDIV	0x48
#define U_M_PDIV	0x3
#endif

#if defined(CONFIG_S3C2440)
#define U_M_MDIV 0x38
#define U_M_PDIV 0x2
#endif
#define U_M_SDIV	0x2
#endif

static inline void pll_delay(unsigned long loops)
{
	__asm__ volatile ("1:\n"
	  "subs %0, %1, #1\n"
	  "bne 1b" : "=r" (loops) : "0" (loops));
}

/*
 * Miscellaneous platform dependent initialisations
 */

int board_early_init_f(void)
{
	struct s3c24x0_clock_power * const clk_power =
					s3c24x0_get_base_clock_power();
	struct s3c24x0_gpio * const gpio = s3c24x0_get_base_gpio();
	
	unsigned int tmp_data = 0;

	/* to reduce PLL lock time, adjust the LOCKTIME register */
	writel(0xFFFFFF, &clk_power->locktime);

	/* configure MPLL */
	writel((M_MDIV << 12) + (M_PDIV << 4) + M_SDIV,
	       &clk_power->mpllcon);

	/* some delay between MPLL and UPLL */
	pll_delay(4000);

	/* configure UPLL */
	writel((U_M_MDIV << 12) + (U_M_PDIV << 4) + U_M_SDIV,
	       &clk_power->upllcon);

	/* some delay between MPLL and UPLL */
	pll_delay(8000);

	/* set up the I/O ports */
	writel(0x007FFFFF, &gpio->gpacon);
	writel(0x00295551, &gpio->gpbcon);
	writel(0x000007FF, &gpio->gpbup);
	writel(0xAAAAA6AA, &gpio->gpccon);
	writel(0xFFFFFFFF, &gpio->gpcup);
	tmp_data = readl(&gpio->gpcdat);
	tmp_data &= ~(1<<5);
	writel(tmp_data, &gpio->gpcdat);
	
	writel(0xAAAAAAAA, &gpio->gpdcon);
	writel(0xFFFFFFFF, &gpio->gpdup);
	writel(0xAAAAAAAA, &gpio->gpecon);
	writel(0x0000FFFF, &gpio->gpeup);
	writel(0x000055AA, &gpio->gpfcon);
	writel(0x000000FF, &gpio->gpfup);
	writel(0xFF95FF3A, &gpio->gpgcon);
	writel(0x0000FFFF, &gpio->gpgup);
	writel(0x0016FAAA, &gpio->gphcon);
	writel(0x000007FF, &gpio->gphup);
	
	writel(0x22222222, &gpio->extint0);
	writel(0x22222222, &gpio->extint1);
	writel(0x22222222, &gpio->extint2);

	return 0;
}

int board_init(void)
{
	/* arch number of MINI2440-Board */
	gd->bd->bi_arch_number = MACH_TYPE_MINI2440;

	/* adress of boot parameters */
	gd->bd->bi_boot_params = 0x30000100;

	icache_enable();
	dcache_enable();

	return 0;
}

#if 0

#define MVAL_USED 	(0)		//0=each frame   1=rate by MVAL
#define INVVDEN		(1)		//0=normal       1=inverted
#define BSWP		(0)		//Byte swap control
#define HWSWP		(1)		//Half word swap control
#define LCDCON5_PWREN (1<<3)   //LCD_PWREN output signal enable

//TFT 240320
#define LCD_XSIZE_TFT_240320 	(320)	
#define LCD_YSIZE_TFT_240320 	(240)

//TFT240320
#define HOZVAL_TFT_240320	(LCD_XSIZE_TFT_240320-1)
#define LINEVAL_TFT_240320	(LCD_YSIZE_TFT_240320-1)

//Timing parameter for NEC 3.5"
#if defined(CONFIG_mini2440_N35)
#define MVAL_N35				(0)
#define LCD_PIXCLOCK_N35            (4)
#define VBPD_240320_N35		(3)		
#define VFPD_240320_N35		(10)
#define VSPW_240320_N35		(1)

#define HBPD_240320_N35		(5)
#define HFPD_240320_N35		(2)
#define HSPW_240320_N35		(36)

#define HSPW_240320_NEC		(36)  //Adjust the horizontal displacement of the screen :tekkamanninja@163.com
				      //+ �� -->    - : <--
#define CLKVAL_TFT_240320_N35	(3) 	
#define LCD_CON5_N35 ( (1 << 11)| (1<<0) | (1 << 8) | (1 << 6) | (1 << 9) | ( 1<< 10))
//FCLK=101.25MHz,HCLK=50.625MHz,VCLK=6.33MHz
#endif

//Timing parameter for Toploy 3.5"
#if defined(CONFIG_mini2440_T35)
#define MVAL_T35				(0)
#define LCD_PIXCLOCK_T35 		(4)
#define VBPD_240320_T35		(3)		
#define VFPD_240320_T35		(10)
#define VSPW_240320_T35		(1)

#define HBPD_240320_T35		(5)
#define HFPD_240320_T35		(2)
#define HSPW_240320_T35		(23)

#define HSPW_240320_TD		(23)  //64MB nand mini2440 is 36 ,128MB is 23
#define LCD_CON5_T35 ( (1 << 11)| (1<<0) | (1 << 8) | (1 << 6) | (1 << 9) | ( 1<< 10))

#define CLKVAL_TFT_240320_T35	(3) 	
//FCLK=101.25MHz,HCLK=50.625MHz,VCLK=6.33MHz
#endif

//Timing parameter for sony 3.5"
#if defined(CONFIG_mini2440_X35)

#define MVAL_X35		(13)
#define LCD_PIXCLOCK_X35 	(4)
#define VBPD_240320_X35	(0)		
#define VFPD_240320_X35	(4)
#define VSPW_240320_X35		(9)

#define HBPD_240320_X35		(25)
#define HFPD_240320_X35		(0)
#define HSPW_240320_X35	(4)
#define LCD_CON5_X35 ( (1 << 11)| (1<<0) | (1 << 8) | (1 << 6) | (1 << 9) | ( 1<< 10))

#define CLKVAL_TFT_240320	(3) 	
#endif

//Timing parameter for W35 320*240"
#if defined(CONFIG_mini2440_W35)

#define MVAL_W35		(13)
#define LCD_PIXCLOCK_W35 	(4)
#define VBPD_240320_W35	(10)		
#define VFPD_240320_W35	(4)
#define VSPW_240320_W35		(1)

#define HBPD_240320_W35		(1)
#define HFPD_240320_W35		(0x1)
#define HSPW_240320_W35	(10)
#define LCD_CON5_W35 ((1<<11) | (1<<8) | (1<<9) | (1<<4)| (1<<0) )

#define CLKVAL_TFT_240320	(3) 	
#endif


void board_video_init(GraphicDevice *pGD) 
{ 
	struct s3c24x0_lcd * const lcd	 = s3c24x0_get_base_lcd(); 
	//struct s3c2410_nand * const nand = s3c2410_get_base_nand();
	
	char *s;
	char lcdtype ;
	s = getenv ("lcdtype");
	//lcdtype = *s;
	lcdtype = 'w';
	//lcdtype = s ? (int)simple_strtol(s, NULL, 10) : CONFIG_LCDTYPE;
	printf("In board_video_init, lcdtype=%c\n",lcdtype);
	if (lcdtype == CONFIG_mini2440_X35) {
		lcd->lcdcon1 = (LCD_PIXCLOCK_X35 << 8) | (3 <<  5) | (12 << 1);
		lcd->lcdcon2 = (VBPD_240320_X35<<24)|(LINEVAL_TFT_240320<<14)|(VFPD_240320_X35<<6)|(VSPW_240320_X35); 
		lcd->lcdcon3 = (HBPD_240320_X35<<19)|(HOZVAL_TFT_240320<<8)|(HFPD_240320_X35); 

			lcd->lcdcon4 = (MVAL_X35<<8)|(HSPW_240320_X35); //Hanson add
		
		//lcd->lcdcon5 = 0x00000f09; //Hanson change
		lcd->lcdcon5 = LCD_CON5_X35 | LCDCON5_PWREN; //LCD_PWREN output signal enable
	}
	else if (lcdtype == CONFIG_mini2440_T35) {
		lcd->lcdcon1 = (LCD_PIXCLOCK_T35 << 8) | (3 <<  5) | (12 << 1);
		lcd->lcdcon2 = (VBPD_240320_T35<<24)|(LINEVAL_TFT_240320<<14)|(VFPD_240320_T35<<6)|(VSPW_240320_T35); 
		lcd->lcdcon3 = (HBPD_240320_T35<<19)|(HOZVAL_TFT_240320<<8)|(HFPD_240320_T35); 

		/*
			if ( (nand->NFCONF) & 0x08 )	{ 
			lcd->lcdcon4 = (MVAL<<8)|(HSPW_240320_TD);
			}
			else	{
			  lcd->lcdcon4 = (MVAL<<8)|(HSPW_240320_NEC);
			}
		*/
		lcd->lcdcon4 = (MVAL_T35<<8)|(HSPW_240320_T35); //Hanson add
		lcd->lcdcon5 = LCD_CON5_T35 | LCDCON5_PWREN; //LCD_PWREN output signal enable
	}
	else if (lcdtype == CONFIG_mini2440_N35) {

		lcd->lcdcon1 = (LCD_PIXCLOCK_N35 << 8) | (3 <<  5) | (12 << 1);
		lcd->lcdcon2 = (VBPD_240320_N35<<24)|(LINEVAL_TFT_240320<<14)|(VFPD_240320_N35<<6)|(VSPW_240320_N35); 
		lcd->lcdcon3 = (HBPD_240320_N35<<19)|(HOZVAL_TFT_240320<<8)|(HFPD_240320_N35); 

		lcd->lcdcon4 = (MVAL_N35<<8)|(HSPW_240320_N35); //Hanson add
		
		lcd->lcdcon5 = LCD_CON5_N35 | LCDCON5_PWREN; //LCD_PWREN output signal enable
	}
	else if (lcdtype == CONFIG_mini2440_W35) {

		lcd->lcdcon1 = (LCD_PIXCLOCK_W35 << 8) | (3 <<  5) | (12 << 1);
		lcd->lcdcon2 = (VBPD_240320_W35<<24)|(LINEVAL_TFT_240320<<14)|(VFPD_240320_W35<<6)|(VSPW_240320_W35); 
		lcd->lcdcon3 = (HBPD_240320_W35<<19)|(HOZVAL_TFT_240320<<8)|(HFPD_240320_W35); 

		lcd->lcdcon4 = (MVAL_W35<<8)|(HSPW_240320_W35); //Hanson add
		
		lcd->lcdcon5 = LCD_CON5_W35 | LCDCON5_PWREN; //LCD_PWREN output signal enable
	}
	//lcd->lpcsel  = 0x00000000; 
	lcd->lcdintmsk |= 3; 
	lcd->lpcsel  &= (~7); 
	lcd->tpal     = 0x0;
	lcd->lcdcon1 |= 0x01; //Enable LCD display by Hanson
	
} 
#endif

int dram_init (void)
{
	/* dram_init must store complete ramsize in gd->ram_size */
	gd->ram_size = PHYS_SDRAM_1_SIZE;
	return 0;
}

#ifdef CONFIG_CMD_NET
int board_eth_init(bd_t *bis)
{
	int rc = 0;
#ifdef CONFIG_CS8900
	rc = cs8900_initialize(0, CONFIG_CS8900_BASE);
#endif
#ifdef CONFIG_DRIVER_DM9000
	rc = dm9000_initialize(bis);
#endif
	return rc;
}
#endif

/*
 * Hardcoded flash setup:
 * Flash 0 is a non-CFI AMD AM29LV800BB flash.
 */
ulong board_flash_get_legacy(ulong base, int banknum, flash_info_t *info)
{
	info->portwidth = FLASH_CFI_16BIT;
	info->chipwidth = FLASH_CFI_BY16;
	info->interface = FLASH_CFI_X16;
	return 1;
}
