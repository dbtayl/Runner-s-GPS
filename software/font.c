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

#include <inttypes.h>

#include "font.h"
#include "ili9340.h"

//Change to '1' to enable checks on incoming data
#define FONT_SAFETY 0

#define FONT_FOREGROUND (uint16_t)0xffff
#define FONT_BACKGROUND (uint16_t)0x0000

void printStr(char* str, uint8_t len, uint16_t x, uint16_t y, uint8_t rot)
{
	//Configure writing area
	ili9340_set_view(rot, x, x+len*FONT_W-1, y, y+FONT_H-1);
	
	//Get ready to write data
	ILI9340_CS_ENABLE();
	ili9340_writeRegOnly(0x002c);
	ILI9340_MODE_DATA();
	
	//Iterate over rows to render
	int i;
	for(i = 0; i < FONT_H; i++)
	{
		#if FONT_W != 8
		#error This code assumes a font width of 8 pixels
		#endif
		int rowoffset = i * FONT_CHARS;
		//Iterate over symbols
		int j;
		for(j = 0; j < len; j++)
		{
			//Iterate over pixels in each symbol
			//Font is stored as a packed bitmap
			//Bytes go in order, but bits are arguably reversed
			uint8_t x = str[j] - FONT_OFFSET;
			#if FONT_SAFETY
			if(x >= FONT_CHARS)
			{
				x = '?' - FONT_OFFSET;
			}
			#endif
			
			//get bits for this symbol
			x = font[x + rowoffset];
			
			#if FONT_W != 8
			#error This code assumes a font width of 8 pixels
			#endif
			//Write pixels
			uint16_t color;
			
			color = x & 0x01 ? FONT_FOREGROUND : FONT_BACKGROUND;
			ili9340_writeData(color);
			
			color = x & 0x02 ? FONT_FOREGROUND : FONT_BACKGROUND;
			ili9340_writeData(color);
			
			color = x & 0x04 ? FONT_FOREGROUND : FONT_BACKGROUND;
			ili9340_writeData(color);
			
			color = x & 0x08 ? FONT_FOREGROUND : FONT_BACKGROUND;
			ili9340_writeData(color);
			
			color = x & 0x10 ? FONT_FOREGROUND : FONT_BACKGROUND;
			ili9340_writeData(color);
			
			color = x & 0x20 ? FONT_FOREGROUND : FONT_BACKGROUND;
			ili9340_writeData(color);
			
			color = x & 0x40 ? FONT_FOREGROUND : FONT_BACKGROUND;
			ili9340_writeData(color);
			
			color = x & 0x80 ? FONT_FOREGROUND : FONT_BACKGROUND;
			ili9340_writeData(color);
		}
	}
	
	ILI9340_CS_DISABLE();
	
	return;
}
