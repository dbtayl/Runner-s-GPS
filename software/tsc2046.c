/*
 * Copyright (c) 2014, Daniel Taylor
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Daniel Taylor BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "LPC177x_8x.h"
#include "tsc2046.h"
#include "conf.h"

#include "lpc177x_8x_ssp.h"
#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_dac.h"
#include "lpc177x_8x_exti.h"
#include "lpc177x_8x_pinsel.h"

//A2_A1_A0
//Measure X: 101
//Measuer Y: 001

//SPI bytes to send for reading X and Y
#define TSC2046_READ_X 0xd0
#define TSC2046_READ_Y 0x90

#define TSC2046_READ_X_8 ((uint8_t) 0xd8)
#define TSC2046_READ_Y_8 ((uint8_t) 0x98)

#define TSC2046_CS_DISABLE() LPC_GPIO0->SET = 1<<14
#define TSC2046_CS_ENABLE() LPC_GPIO0->CLR = 1<<14


//Handles the PENIRQ input
//FIXME: Needs to actually DO something (probably just set a flag so the main thread can handle it)
//FIXME: Need to configure priority
//FIXME: Need to allow this to wake up device from sleep
void EINT0_IRQHandler(void)
{
	EXTI_ClearEXTIFlag(EXTI_EINT0);
	
	//Notify main loop that screen was touched
	touchFlag = 1;
	
	/*
	//If the screen is on, turn it off
	if(isLcdBlOn())
	{
		DAC_UpdateValue(0, 0);
		
	}
	//Otherwise, turn it on
	else
	{
		DAC_UpdateValue(0, DEFAULT_LCD_BACKLIGHT);
	}
	*/
}


Point readTSC2046()
{
	Point pos;
	//FIXME: Debug
	pos.x = 123;
	pos.y = 456;
	
	//Start at 2 to basically introduce rounding
	uint16_t x = 2;
	uint16_t y = 2;
	
	uint8_t dummy;
	uint8_t i;
	
	for(i = 0; i < 4; i++)
	{
		TSC2046_CS_ENABLE();
		while (LPC_SSP1->SR & 0x10);
		
		//Write byte to read X (blocking operation)
		LPC_SSP1->DR = TSC2046_READ_X;
		while (LPC_SSP1->SR & 0x10);
		
		//remove write from FIFO
		dummy = LPC_SSP1->DR;
		
		//Read back the 2 bytes
		LPC_SSP1->DR = 0x00;
		while (LPC_SSP1->SR & 0x10);
		pos.x = ((uint16_t)(LPC_SSP1->DR)) << 5;
		LPC_SSP1->DR = 0x00;
		while (LPC_SSP1->SR & 0x10);
		pos.x |= (LPC_SSP1->DR) >> 3;
		
		//CS high
		TSC2046_CS_DISABLE();
		
		x += pos.x;
	}
	
	for(i = 0; i < 4; i++)
	{
		TSC2046_CS_ENABLE();
		
		//Write byte to read Y (blocking operation)
		LPC_SSP1->DR = TSC2046_READ_Y;
		while (LPC_SSP1->SR & 0x10);
		
		//remove write from FIFO
		dummy = LPC_SSP1->DR;
		
		//Read back the 2 bytes
		LPC_SSP1->DR = 0x00;
		while (LPC_SSP1->SR & 0x10);
		pos.y = ((uint16_t)(LPC_SSP1->DR)) << 5;
		LPC_SSP1->DR = 0x00;
		while (LPC_SSP1->SR & 0x10);
		pos.y |= (LPC_SSP1->DR) >> 3;
		
		TSC2046_CS_DISABLE();
		
		y += pos.y;
	}
	
	pos.x = x>>2;
	pos.y = y>>2;
	
	return pos;
}


uint8_t TSC2046_init()
{
	//NOTE: Assumes GPIO0 is enabled already
	
	//Set GPIO0[14] to output- CS
	GPIO_SetDir(0, 1<<14, GPIO_DIRECTION_OUTPUT);
	
	//Set GPIO0[31] to input- BUSY signal
	GPIO_SetDir(0, 1<<31, GPIO_DIRECTION_INPUT);
	
	TSC2046_CS_DISABLE();
	
	//Configure SPI
	//FIXME: These values may not be right
	SSP_CFG_Type cfg;
	SSP_ConfigStructInit(&cfg);
	cfg.Databit = SSP_DATABIT_8;
	cfg.Mode = SSP_MASTER_MODE;
	cfg.FrameFormat = SSP_FRAME_SPI;
	cfg.CPHA = SSP_CPHA_SECOND;
	cfg.CPOL = SSP_CPOL_LO;
	
	cfg.ClockRate = 1000000;
	
	//Configure SSP pins
	PINSEL_ConfigPin(0, 13, 2); //MOSI
	PINSEL_ConfigPin(0, 12, 2); //MISO
	//PINSEL_ConfigPin(0, 14, 0); //CS- manual control
	PINSEL_ConfigPin(0, 7, 2);  //CLK
	
	SSP_Init(LPC_SSP1, &cfg);
	SSP_Cmd(LPC_SSP1, ENABLE);
		
	//FIXME: Set up PENIRQ (P0[29])
	//FIXME: This line is NOT just what we need- use functions in lpc177x_8x_exti.c
	//FIXME: Need to add EINT0_Handler
	//FIXME: Need to set interrupt priorities
	PINSEL_ConfigPin(0, 29, 2);
	
	
	return 0;
}

//p0[29] is PENIRQ; set up an interrupt
void configurePENIRQ()
{
	EXTI_Init();
	PINSEL_ConfigPin(0, 29, 2);
	PINSEL_SetPinMode(0, 29, PINSEL_BASICMODE_PULLUP);
	EXTI_InitTypeDef t;
	t.EXTI_Line = EXTI_EINT0;
	t.EXTI_Mode = EXTI_MODE_EDGE_SENSITIVE;
	t.EXTI_polarity = EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE;
	
	EXTI_Config(&t);
	EXTI_ClearEXTIFlag(EXTI_EINT0);
	
	NVIC_EnableIRQ(EINT0_IRQn);
}
