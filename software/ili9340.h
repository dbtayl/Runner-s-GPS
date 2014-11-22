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

/*
 * NOTE: Do NOT leave the read pin floating- tie it high!
 */

#ifndef ILI9340_H
#define ILI9340_H

#include <stdint.h>

//FIXME: This is platform-dependent!
#include "LPC177x_8x.h"
#include "lpc177x_8x_gpio.h"


//The resolution of the LCD... Required for things to work properly
//Defined as for rotation = 0, that is, "regular" portrait mode
#define TFTWIDTH 240
#define TFTHEIGHT 320

//0 is regular portrait, everything else is CW rotated from there
//(90 is with ribbon cable on left of LCD)
#define ROT_0	0
#define ROT_90	1
#define ROT_180	2
#define ROT_270 3


//Set to 1 to assume that the chip is always selected -> calls to change
//CS line will be omitted
#define ALWAYS_CS 0

//Note that CS is active LOW
//CS on the LPC1778 is P3[26]
#if ALWAYS_CS
#define ILI9340_CS_ENABLE() /*Nothing*/ 
#define ILI9340_CS_DISABLE() /*Nothing*/
#else
#define ILI9340_CS_ENABLE() LPC_GPIO3->CLR = 0x04000000
#define ILI9340_CS_DISABLE() LPC_GPIO3->SET = 0x04000000
#endif

/*
 * This macro takes a 16-bit value, and outputs it on the LCD parallel
 * data lines.
 */
#define ILI9340_DATA_OUT(x) GPIO1_HalfWord->FIOPINU = (uint16_t)x


/*
 * This macro should pulse the write strobe and thus write the data on
 * the parallel interface to the LCD
 * 
 * Write strobe is active LOW
 */
//P3[24] on LPC1778
#define ILI9340_WRITE_LOW() LPC_GPIO3->CLR = 0x01000000
#define ILI9340_WRITE_HIGH() LPC_GPIO3->SET = 0x01000000
#define ILI9340_WRITE_STROBE() LPC_GPIO3->CLR = 0x01000000; LPC_GPIO3->SET = 0x01000000


/* This macro disables the read line */
#define ILI9340_READ_HIGH() LPC_GPIO3->SET = 1<<23
#define ILI9340_READ_LOW() LPC_GPIO3->CLR = 1<<23

/*
 * These macros set the LCD into data mode or command mode, by setting
 * the appropriate pin to the appropriate level
 * 
 * DATA is high, CMD is low
 */
//P3[25] on the LPC1778
#define ILI9340_MODE_DATA() LPC_GPIO3->SET = 0x02000000
#define ILI9340_MODE_CMD() LPC_GPIO3->CLR = 0x02000000

/*
 * These macros affect the LCD reset state 
 */
//Reset on LPC1778 is P4[5]
#define ILI9340_RESET_ON() LPC_GPIO4->CLR = 1<<5
#define ILI9340_RESET_OFF() LPC_GPIO4->SET = 1<<5


//Resets the LCD
void ili9340_reset();

//Initializes the LCD. Defaults to portrait mode
//Assumes pins already configured
void ili9340_init(void);


//Writes 'data' into register 'reg'
void ili9340_writeReg(uint16_t reg, uint16_t data);
void ili9340_writeRegOnly(uint16_t reg);

//Writes data to the LCD (ie, pixel value at the current location)
//The 'safe' version makes sure the LCD is in the correct mode (data mode,
//GRAM write mode, etc.), while the regular version just writes the values
void ili9340_writeDataSafe(uint16_t data);

#define ili9340_writeData(data) ILI9340_DATA_OUT(data); ILI9340_WRITE_STROBE()

void ili9340_writeDataMultiple(uint16_t* data, uint16_t count);

void ili9340_floodFill(uint16_t color, uint32_t count);

//Approximate delay function
void Delay(uint32_t ms);


uint8_t ili9340_set_view(uint8_t rot, uint16_t sc, uint16_t ec, uint16_t sr, uint16_t er);
void ili9340_set_renderarea(uint16_t sc, uint16_t ec, uint16_t sr, uint16_t er);


#endif
