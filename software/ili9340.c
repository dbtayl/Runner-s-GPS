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

#include <stdint.h>
#include "ili9340.h"
#include "lpc177x_8x_timer.h"

//for timer stuff that should really exist elsewhere
#include "lpc177x_8x_clkpwr.h"

//If stuff isn't working, search for POSSIBLE_ERROR
//??? means "I really don't know what this does"
//LATER means "I might want to use this later"

void Delay(uint32_t ms)
{
	TIM_Waitus(ms*1000);
}

void ili9340_reset()
{
	ILI9340_CS_DISABLE();
	ILI9340_MODE_DATA();
	ILI9340_WRITE_STROBE();

	ILI9340_RESET_OFF();
	Delay(5);
	ILI9340_RESET_ON();
	Delay(15);
	ILI9340_RESET_OFF();
	Delay(15);

	Delay(100);
	return;
}


void ili9340_writeReg(uint16_t reg, uint16_t data)
{
	//NOTE: Do NOT call ILI9340_CS_ENABLE()/ILI9340_CS_DISABLE()... calling functions handle that.
	
	//1: Tell LCD a command is coming
	ILI9340_MODE_CMD();
	
	//2: Send command
	ILI9340_DATA_OUT(reg);
	ILI9340_WRITE_STROBE();
	
	//3: Tell LCD data is coming
	ILI9340_MODE_DATA();
	
	//4: Send data
	ILI9340_DATA_OUT(data);
	ILI9340_WRITE_STROBE();

	return;
}

void ili9340_writeRegOnly(uint16_t reg)
{
	//NOTE: Do NOT call ILI9340_CS_ENABLE()/ILI9340_CS_DISABLE()... calling functions handle that.
	
	//1: Tell LCD a command is coming
	ILI9340_MODE_CMD();
	
	//2: Send command
	ILI9340_DATA_OUT(reg);
	ILI9340_WRITE_STROBE();

	return;
}


void ili9340_writeDataSafe(uint16_t data)
{
	//NOTE: Do NOT call ILI9340_CS_ENABLE()/ILI9340_CS_DISABLE()... calling functions handle that.
	
	//Set to GRAM write mode
	ILI9340_MODE_CMD();
	Delay(1);
	ILI9340_DATA_OUT(0x002c);
	Delay(1);
	ILI9340_WRITE_STROBE();
	Delay(1);
	
	//Actually send the data
	ILI9340_MODE_DATA();
	Delay(1);
	ILI9340_DATA_OUT(data);
	Delay(1);
	ILI9340_WRITE_STROBE();
	Delay(1);
	
	return;
}


#define MV (1<<5)
#define MX (1<<6)
#define MY (1<<7)
#define ML (1<<4)
#define MH (1<<2)
#define BGR (1<<3)

uint8_t ili9340_set_view(uint8_t rot, uint16_t sc, uint16_t ec, uint16_t sr, uint16_t er)
{
	//Or 0x2a -> column address set -> start/endcolumn
	//+  0x2b -> page address set -> start/end row (?)
	//0x36 -> "memory access control" -> set up/down/left/right fill order
	
	//Do some error checking
	//Landscape
	if(rot & 0x01)
	{
		if( (sc >= ec) || (sr >= er) || (er >= TFTWIDTH) || (ec >= TFTHEIGHT) )
		{
			return 1;
		}
	}
	//Portrait
	else
	{
		if( (sc >= ec) || (sr >= er) || (er >= TFTHEIGHT) || (ec >= TFTWIDTH) )
		{
			return 1;
		}
	}
	
	ILI9340_CS_ENABLE();
	
	//0x88 is normal = BGR | MY
	//ML doesn't seem to do anything
	switch(rot)
	{
		case ROT_270:
		{
			ili9340_writeReg(0x0036, BGR | MX | MV); //OK
			break;
		}
		case ROT_180:
		{
			ili9340_writeReg(0x0036, BGR | ML | MX | MY); //OK
			break;
		}
		case ROT_90:
		{
			ili9340_writeReg(0x0036, BGR | MV | ML | MH | MY); //OK
			break;
		}
		//Assume 0 rotation
		default:
		{
			ili9340_writeReg(0x0036, BGR); //Correct
			break;
		}
	}
	
	ili9340_set_renderarea(sc, ec, sr, er);
	
	ILI9340_CS_DISABLE();
	
	return 0;
}

void ili9340_set_renderarea(uint16_t sc, uint16_t ec, uint16_t sr, uint16_t er)
{
	//FIXME: Error checking here?
	
	//Set column bounds
	ili9340_writeReg(0x002a, sc>>8);
	ili9340_writeData(sc);
	ili9340_writeData(ec>>8);
	ili9340_writeData(ec);
	
	//Set row bounds
	ili9340_writeReg(0x002b, sr>>8);
	ili9340_writeData(sr);
	ili9340_writeData(er>>8);
	ili9340_writeData(er);
}


//Assumes CS and mode are handled externally
void ili9340_writeDataMultiple(uint16_t* data, uint16_t count)
{
	//Handle modulo 4 elements
	uint8_t slow = count & 0x03;
	if(slow)
	{
		do
		{
			ili9340_writeData(data[0]);
			data++;
		}
		while(--slow);
	}
	
	//4x unrolling
	count >>= 2;
	if(count)
	{
		do
		{
			ili9340_writeData(data[0]);
			ili9340_writeData(data[1]);
			ili9340_writeData(data[2]);
			ili9340_writeData(data[3]);
			data += 4;
		}
		while(--count);
	}
}


//Just dumps pixels; assumes all else is handled elsewhere
//NOTE: No unrolling of loop; unrolled -> too fast
void ili9340_floodFill(uint16_t color, uint32_t count)
{
	ILI9340_DATA_OUT(color);
	
	//Handle modulo 4 elements
	/*uint8_t slow = count & 0x01;
	if(slow)
	{
		do
		{
			ILI9340_WRITE_STROBE();
		}
		while(--slow);
	}*/
	
	//2x unrolling
	//count >>= 2;
	if(count)
	{
		do
		{
			ILI9340_WRITE_STROBE();
			//ILI9340_WRITE_STROBE();
		}
		while(--count);
	}
}


void ili9340_init(void)
{
	//GPIO_ResetBits(GPIOC, CS1);
	ILI9340_CS_ENABLE();
	
	//GPIO_SetBits(GPIOC, nRD);
	ILI9340_READ_HIGH();
	
	//GPIO_ResetBits(GPIOC, nWR);
	ILI9340_WRITE_LOW();
	
	//GPIO_WriteBit(GPIOC, RES, Bit_RESET);
	ILI9340_RESET_ON();
	Delay(100);
	//GPIO_WriteBit(GPIOC, RES, Bit_SET);
	ILI9340_RESET_OFF();
	Delay(100);
	
	ili9340_writeRegOnly(0x0029);			//display ON
	ili9340_writeRegOnly(0x0013);			//normal mode
	ili9340_writeReg(0x0011, 0x0000);		//exit SLEEP mode
	ili9340_writeReg(0x00C0, 0x0026);		//power control 1
		ili9340_writeData(0x0004);	
	ili9340_writeReg(0x00C1, 0x0004);		//power control 2
	ili9340_writeReg(0x00C5, 0x0034);		//VCOM control 1
		ili9340_writeData(0x0040);
	ili9340_writeReg(0x0036, 0x0088);		//memory access control = BGR
	ili9340_writeReg(0x00B1, 0x0000);
		ili9340_writeData(0x0018);			//frame rate control
	ili9340_writeReg(0x00B6, 0x000A);		//display function control
		ili9340_writeData(0x00A2);
	ili9340_writeReg(0x00C7, 0x00C0);		//VCOM control 2
	ili9340_writeReg(0x003A, 0x0055);		//pixel format = 16 bit per pixel
	ili9340_writeReg(0x00E0, 0x001F);		//positive gamma correction
		ili9340_writeData(0x001B);
		ili9340_writeData(0x0018);
		ili9340_writeData(0x000B);
		ili9340_writeData(0x000F);
		ili9340_writeData(0x0009);
		ili9340_writeData(0x0046);
		ili9340_writeData(0x00B5);
		ili9340_writeData(0x0037);
		ili9340_writeData(0x000A);
		ili9340_writeData(0x000C);
		ili9340_writeData(0x0007);
		ili9340_writeData(0x0007);
		ili9340_writeData(0x0005);
		ili9340_writeData(0x0000);
	ili9340_writeReg(0x00E1, 0x0000);		//negative gamma correction
		ili9340_writeData(0x0024);
		ili9340_writeData(0x0027);
		ili9340_writeData(0x0004);
		ili9340_writeData(0x0010);
		ili9340_writeData(0x0006);
		ili9340_writeData(0x0039);
		ili9340_writeData(0x0074);
		ili9340_writeData(0x0048);
		ili9340_writeData(0x0005);
		ili9340_writeData(0x0013);
		ili9340_writeData(0x0038);
		ili9340_writeData(0x0038);
		ili9340_writeData(0x003A);
		ili9340_writeData(0x001F);
		
	//Power up in portrait mode
	ili9340_set_renderarea(0x0000, 0x00ef, 0x0000, 0x013f);
}
