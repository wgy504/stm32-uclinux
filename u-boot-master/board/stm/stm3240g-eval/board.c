/*
 * (C) Copyright 2011, 2012
 *
 * Yuri Tikhonov, Emcraft Systems, yur@emcraft.com
 * Alexander Potashev, Emcraft Systems, aspotashev@emcraft.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * Board specific code for the STMicroelectronic STM3220G-EVAL board
 */

#include <common.h>
#include <netdev.h>
#include <ili932x.h>

#include <asm/arch/stm32.h>
#include <asm/arch/stm32f2_gpio.h>
#include <asm/arch/fsmc.h>

#if (CONFIG_NR_DRAM_BANKS > 0)
/*
 * Check if RAM configured
 */
# if !defined(CONFIG_SYS_RAM_CS) || !defined(CONFIG_SYS_FSMC_PSRAM_BCR) ||     \
     !defined(CONFIG_SYS_FSMC_PSRAM_BTR)
#  error "Incorrect PSRAM FSMC configuration."
# endif
#endif /* CONFIG_NR_DRAM_BANKS */

DECLARE_GLOBAL_DATA_PTR;

/*
 * Early hardware init.
 */
int board_init(void)
{
	int rv;

#if !defined(CONFIG_SYS_NO_FLASH)
	if ((rv = fsmc_nor_psram_init(CONFIG_SYS_FLASH_CS, CONFIG_SYS_FSMC_FLASH_BCR,
			CONFIG_SYS_FSMC_FLASH_BTR,
			CONFIG_SYS_FSMC_FLASH_BWTR)))
		return rv;
#endif

#if defined(CONFIG_LCD)
	/*
	 * Configure FSMC for accessing the LCD controller
	 */
	if ((rv = fsmc_nor_psram_init(CONFIG_LCD_CS, CONFIG_LCD_FSMC_BCR,
			CONFIG_LCD_FSMC_BTR, CONFIG_LCD_FSMC_BWTR)))
		return rv;

	gd->fb_base = CONFIG_FB_ADDR;
#endif

	return 0;
}

/*
 * Dump pertinent info to the console.
 */
int checkboard(void)
{
	printf("Board: STM3240G-EVAL board + STM-MEM add-on,%s\n",
		CONFIG_SYS_BOARD_REV_STR);

	return 0;
}

/*
 * Setup external RAM.
 */
int dram_init(void)
{

	int				rv = 0;

	rv = fsmc_nor_psram_init(CONFIG_SYS_RAM_CS,
			CONFIG_SYS_FSMC_PSRAM_BCR,
			CONFIG_SYS_FSMC_PSRAM_BTR,
#ifdef CONFIG_SYS_FSMC_PSRAM_BWTR
			CONFIG_SYS_FSMC_PSRAM_BWTR
#else
			(u32)-1
#endif
		);
	if (rv != 0)
		goto out;

//	rv = stm32f2_gpio_config(&ctrl_gpio, STM32F2_GPIO_ROLE_GPOUT);
//	if (rv != 0)
//		goto out;


	/*
	 * Switch PSRAM in the Asyncronous Read/Write Mode
	 */


	/*
	 * Fill in global info with description of SRAM configuration
	 */
	gd->bd->bi_dram[0].start = CONFIG_SYS_RAM_BASE;
	gd->bd->bi_dram[0].size  = CONFIG_SYS_RAM_SIZE;

	rv = 0;

out:
	return rv;
}

#ifdef CONFIG_STM32_ETH
/*
 * Register ethernet driver
 */
int board_eth_init(bd_t *bis)
{
	return stm32_eth_init(bis);
}
#endif


