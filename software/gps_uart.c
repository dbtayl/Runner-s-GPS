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

#include "gps_uart.h"
#include "LPC177x_8x.h"
#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_clkpwr.h"
#include "lpc177x_8x_gpio.h"

unsigned char GPSUARTBuffer[GPS_UART_DATA_SIZE];
unsigned int GPSUARTIndex = 0;

//Whether the message we care about the message we're currently receiving
unsigned char valid = 0;

#if USE_GPS_MSG_GGA
//Test string to check validity against
//use this check string for GGA
const unsigned char* check = "$GPGGA";
#else
//this one for RMC
const unsigned char* check = "$GPRMC";
#endif

#define GPS_CHECK_LEN 6

volatile uint8_t gpsActive = 1;

volatile GPSData gpsData;

volatile uint8_t gpsUpdated = 0;

void GPS_Init(unsigned int baud)
{
	//Turn on GPS via GPIO
	//GPS enable pin is p5[4]
	//GPIO_SetDir(5, 1<<4, GPIO_DIRECTION_OUTPUT);
	//LPC_GPIO5->SET = 1<<4;
	
	//Give GPS time to power on- may not be necessary, at least not this much time
	Delay(500);
	
	//Set pins to be UART3 pins
	PINSEL_ConfigPin(4, 28, 2); //pin 118, TX
	PINSEL_ConfigPin(4, 29, 2); //pin 122, RX
	
	UART_CFG_Type cfg;
	UART_FIFO_CFG_Type fifocfg;
	
	UART_ConfigStructInit(&cfg);
	cfg.Baud_rate = baud;
	
	UART_Init(UART_3, &cfg);
	
	//Defaults are OK- no DMA, interrupt on every character
	UART_FIFOConfigStructInit(&fifocfg);
	UART_FIFOConfig(UART_3, &fifocfg);
	
	UART_TxCmd(UART_3, ENABLE);
	
	Delay(500);
	
	//Send message to disable messages we don't care about
	//NOTE: This is the right string for RMC and nothing else
	//$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n
	
	//This string should enable GGA and nothing else
	//$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n
	#if USE_GPS_MSG_GGA
	uint8_t* txstr = "$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n";
	#else
	uint8_t* txstr = "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n";
	#endif
	UART_Send(UART_3, txstr, 51, BLOCKING);
	
	//Enable interrupt on data received
	UART_IntConfig(UART_3, UART_INTCFG_RBR, ENABLE);
	UART_IntConfig(UART_3, UART_INTCFG_RLS, ENABLE);
	
	Delay(100);
	//Allow GPS to wake MCU up from sleep
	__set_FAULTMASK(0);
	__set_PRIMASK(0);
	SCB->SCR = 0;
	
	//Set message as invalid before enabling iterrupt
	valid = 0;
	GPSUARTIndex = 0;
	
	NVIC_EnableIRQ(UART3_IRQn);
	
	//Set state to GPS on
	gpsActive = 1;
	
	return;
}


void GPS_Stop(void)
{
	//Disable GPS interrupt
	UART_IntConfig(UART_3, UART_INTCFG_RBR, DISABLE);
	UART_IntConfig(UART_3, UART_INTCFG_RLS, DISABLE);
	
	//Disable GPS wake-from-sleep
	NVIC_DisableIRQ(UART3_IRQn);
	
	//Turn off GPS via GPIO
	//GPS enable pin is p5[4]
	//GPIO_SetDir(5, 1<<4, GPIO_DIRECTION_OUTPUT);
	//LPC_GPIO5->CLR = 1<<4;
	GPS_Sleep();
	
	//Set state to GPS off (redundant, but do it anyway)
	gpsActive = 0;
	gpsData.valid = 0;
	
	//Disable GPS UART TX
	//UART_TxCmd(UART_3, DISABLE);
	
	//Turn off GPS UART
	//CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCUART3, DISABLE);
		
	return;
}


void GPS_Sleep()
{
	//Send GPS Sleep command
	uint8_t* sleepCmd = (uint8_t*)"$PMTK161,0*28\r\n";
	UART_Send(UART_3, sleepCmd, 15, BLOCKING);
	
	CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCUART3, DISABLE);
	
	//Set state to GPS off
	gpsActive = 0;
	gpsData.valid = 0;
}


void GPS_Wakeup()
{
	CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCUART3, ENABLE);
	
	//Send GPS Sleep command
	uint8_t wakeCmd = ' ';
	UART_Send(UART_3, &wakeCmd, 1, BLOCKING);
	
	//Set state to GPS on
	gpsActive = 1;
}


//Internal function to parse GPS message
//FIXME: Do some error checking?
void parseMessage()
{
	//For RMC:
	//Byte 18 is 'valid fix' byte
	//20-21 are degrees N/S
	//22-23, 25-28 are minutes N/S
	//30 is N/S byte
	//32-34 are degrees E/W
	//35-36, 38-41 are minutes E/W
	//43 is E/W byte
	
	//For GGA:
	//Ex: $GPGGA,064951.000,2307.1256,N,12016.4438,E,1,8,0.95,39.9,M,17.8,M,,*65<CR><LF>
	//Bytes 7-12 are hhmmss
	//Byte 13 is '.'
	//Bytes 14-16 are fractional seconds
	//18-19 are degrees N/S
	//20-21, 23-26 are minutes N/S
	//28 is N/S byte
	//30-32 are degrees E/W
	//33-34, 36-39 are minutes E/W
	//41 is E/W byte
	//43 is fix indicator (0 = no fix, 1=GPS fix, 2 = DGPS fix)
	//45 is number of satellites used
	
	
	#if USE_GPS_MSG_GGA
	//If the fix byte isn't '1' or '2', we don't have a fix
	//FIXME: Be more stringent, and check if number of satellites is over some value?
	if(GPSUARTBuffer[43] != '1' && GPSUARTBuffer[43] != '2')
	#else
	//If this isn't A, we don't have a fix
	if(GPSUARTBuffer[18] != 'A')
	#endif
	{
		gpsData.valid = 0;
	}
	else
	{
		gpsData.valid = 1;
		
		//Convert from minutes to decimal degrees
		#if USE_GPS_MSG_GGA
		gpsData.lat = ( (GPSUARTBuffer[18] - '0') * 10.0f + (GPSUARTBuffer[19] - '0') + ( (GPSUARTBuffer[20] - '0') * 10.0f + (GPSUARTBuffer[21] - '0') + (GPSUARTBuffer[23] - '0') / 10.0f + (GPSUARTBuffer[24] - '0') / 100.0f  + (GPSUARTBuffer[25] - '0') / 1000.0f  + (GPSUARTBuffer[26] - '0') / 10000.0f ) / 60.0f ) * (GPSUARTBuffer[28] == 'N' ? 1.0f : -1.0f);
		gpsData.lon = ( (GPSUARTBuffer[30] - '0') * 100.0f + (GPSUARTBuffer[31] - '0') * 10.0f + (GPSUARTBuffer[32] - '0') + ( (GPSUARTBuffer[33] - '0') * 10.0f + (GPSUARTBuffer[34] - '0') + (GPSUARTBuffer[36] - '0') / 10.0f + (GPSUARTBuffer[37] - '0') / 100.0f  + (GPSUARTBuffer[38] - '0') / 1000.0f  + (GPSUARTBuffer[39] - '0') / 10000.0f ) / 60.0f ) * (GPSUARTBuffer[41] == 'E' ? 1.0f : -1.0f);
		#else
		gpsData.lat = ( (GPSUARTBuffer[20] - '0') * 10.0f + (GPSUARTBuffer[21] - '0') + ( (GPSUARTBuffer[22] - '0') * 10.0f + (GPSUARTBuffer[23] - '0') + (GPSUARTBuffer[25] - '0') / 10.0f + (GPSUARTBuffer[26] - '0') / 100.0f  + (GPSUARTBuffer[27] - '0') / 1000.0f  + (GPSUARTBuffer[28] - '0') / 10000.0f ) / 60.0f ) * (GPSUARTBuffer[30] == 'N' ? 1.0f : -1.0f);
		gpsData.lon = ( (GPSUARTBuffer[32] - '0') * 100.0f + (GPSUARTBuffer[33] - '0') * 10.0f + (GPSUARTBuffer[34] - '0') + ( (GPSUARTBuffer[35] - '0') * 10.0f + (GPSUARTBuffer[36] - '0') + (GPSUARTBuffer[38] - '0') / 10.0f + (GPSUARTBuffer[39] - '0') / 100.0f  + (GPSUARTBuffer[40] - '0') / 1000.0f  + (GPSUARTBuffer[41] - '0') / 10000.0f ) / 60.0f ) * (GPSUARTBuffer[43] == 'E' ? 1.0f : -1.0f);
		#endif
		gpsUpdated = 1;
	}
		
	return;
}


void UART3_IRQHandler(void)
{
	//NOTE: There was some other code in here; if it seems wonky, check git
	
	unsigned char byte = 0;

	//Read in the available data
	while(LPC_UART3->LSR & 0x01)
	{
		//Read a byte
		byte = LPC_UART3->RBR;
		
		//Check for first byte of new message
		if(!valid)
		{
			if( (GPSUARTIndex == 0) && (byte == check[0]) )
			{
				valid = 1;
				GPSUARTBuffer[GPSUARTIndex++] = byte;
			}
		}
		
		else //valid
		{
			//In process of validating message
			if( (GPSUARTIndex < GPS_CHECK_LEN) )
			{
				//If the byte checks out, save it
				if(byte == check[GPSUARTIndex])
				{
					GPSUARTBuffer[GPSUARTIndex++] = byte;
				}
				//otherwise, dump it and start over
				else
				{
					valid = 0;
					GPSUARTIndex = 0;
				}
			}
			
			//Just receiving the rest of a known-valid message
			else
			{
				if(byte == '\r')
				{
					parseMessage();
					valid = 0;
					GPSUARTIndex = 0;
				}
				else
				{
					GPSUARTBuffer[GPSUARTIndex++] = byte;
				}
			}
		} // valid
	}//while

	//Start wrapping if our data gets too large (should never happen)
	if(GPSUARTIndex >= GPS_UART_DATA_SIZE)
	{
		GPSUARTIndex = 0;
	}
	
	return;
}
