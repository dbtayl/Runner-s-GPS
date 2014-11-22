/**********************************************************************
* $Id$		lpc17xx_libcfg.h			2010-05-21
*//**
* @file		lpc17xx_libcfg.h
* @brief	Library configuration file
* @version	2.0
* @date		21. May. 2010
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2010, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/

#ifndef _LPC177x_8x_LIBCFG_H_
#define _LPC177x_8x_LIBCFG_H_

#include "lpc_types.h"


/************************** DEBUG MODE DEFINITIONS *********************************/
/* Un-comment the line below to compile the library in DEBUG mode, this will expanse
   the "CHECK_PARAM" macro in the FW library code */

//#define DEBUG


/******************* PERIPHERAL FW LIBRARY CONFIGURATION DEFINITIONS ***********************/

/* Comment the line below to disable the specific peripheral inclusion */

/* DEBUG_FRAMWORK -------------------- */
//#define _DBGFWK

/* Clock & Power -------------------- */
#define _CLKPWR

/* CRC -------------------- */
#define _CRC

/* GPIO ------------------------------- */
#define _GPIO

/* NVIC ------------------------------- */
#define _NVIC

/* PINSEL ------------------------------- */
#define _PINSEL

/* EXTI ------------------------------- */
#define _EXTI

/* EMC ------------------------------- */
//#define _EMC

/* UART ------------------------------- */
#define _UART

/* SYSTICK --------------------------- */
//#define _SYSTICK

/* SSP ------------------------------- */
#define _SSP


/* I2C ------------------------------- */
//#define _I2C

/* TIMER ------------------------------- */
#define _TIM

/* WDT ------------------------------- */
//#define _WDT


/* GPDMA ------------------------------- */
//required for MCI
#define _GPDMA


/* DAC ------------------------------- */
#define _DAC

/* ADC ------------------------------- */
//#define _ADC

/* EEPROM ------------------------------- */
//#define _EEPROM

/* PWM ------------------------------- */
//#define _PWM

/* RTC ------------------------------- */
//#define _RTC

/* I2S ------------------------------- */
//#define _I2S

/* USB device ------------------------------- */
//#define _USBDEV

/* USB Host ------------------------------- */
//#define _USBHost

/* QEI ------------------------------- */
//#define _QEI

/* MCPWM ------------------------------- */
//#define _MCPWM

/* CAN--------------------------------*/
//#define _CAN

/* EMAC ------------------------------ */
//#define _EMAC

/* LCD ------------------------------ */
//#define _LCD

/* MCI ------------------------------ */
#define _MCI

/************************** GLOBAL/PUBLIC MACRO DEFINITIONS *********************************/


#endif /* _LPC177x_8x_LIBCFG_H_ */
