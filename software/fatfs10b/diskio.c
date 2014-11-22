/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include <inttypes.h>
#include "diskio.h"		/* FatFs lower layer API */
#include "mmc_driver.h"		/* Example: MMC/SDC contorl */

//FIXME: Debug
#include "../Drivers/include/lpc177x_8x_uart.h"

/* Definitions of physical drive number for each drive */
#define MMC		0	/* Example: Map MMC/SD card to drive number 0 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

//0000: This has been updated for GPS thingy, but is untested
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	
	if(pdrv != MMC)
	{
		return STA_NOINIT;
	}

	stat = 0;
	uint32_t t = mmc_isinit();
	if(!t)
	{
		UART_Send(UART_0, "disk_status error\n\r", 19, BLOCKING);
		stat |= STA_NOINIT;
	}

	return stat;
}



/*-----------------------------------------------------------------------*/
/* Initialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

//0000: This has been updated for GPS thingy, but is untested
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	
	if(pdrv != MMC)
	{
		return STA_NOINIT;
	}

	stat = 0;
	
	mmc_inithardware();
	
	int t = mmc_init_card();
	if(t)
	{
		UART_Send(UART_0, "disk_init error\n\r", 17, BLOCKING);
		stat |= STA_NODISK;
	}

	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

//0000: This has been updated for GPS thingy, but is untested
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	//DRESULT res;
	if(pdrv != MMC)
	{
		return RES_PARERR;
	}

	int32_t result;
	result = mmc_readmultiplesector(buff, sector, count);
	if(result)
	{
		UART_Send(UART_0, "disk_read error\n\r", 17, BLOCKING);
		//FIXME: Return some meaningful error
		return RES_ERROR;
	}
	
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

//0000: This has been updated for GPS thingy, but is untested
#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	//DRESULT res;
	//int result;
	
	if(pdrv != MMC)
	{
		return RES_PARERR;
	}

	int32_t result = mmc_writemultiplesector(buff, sector, count);
	if(result)
	{
		UART_Send(UART_0, "disk_write error\n\r", 18, BLOCKING);
		//FIXME: Return some meaningful error
		return RES_ERROR;
	}
	
	return RES_OK;
}
#endif


//Dummy function, since we don't have a clock
//FIXME: Should use GPS time if available
DWORD get_fattime (void)
{
	/* If no RTC support. Return a fixed value 2010/4/26 0:00:00 */
	return	  ((DWORD)(2010 - 1980) << 25)	/* Y */
			| ((DWORD)4  << 21)				/* M */
			| ((DWORD)26 << 16)				/* D */
			| ((DWORD)0  << 11)				/* H */
			| ((DWORD)0  << 5)				/* M */
			| ((DWORD)0  >> 1);				/* S */
}


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	//int result;
	if(pdrv != MMC)
	{
		return RES_PARERR;
	}

	//FIXME- actually needs implementing, at least the "CTRL_SYNC" option
	// Process of the command for the MMC/SD card

	return res;
}
#endif
