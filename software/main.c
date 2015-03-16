/*
 * Copyright (c) 2015, Daniel Taylor
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

#include "lpc177x_8x_dac.h"
#include "lpc177x_8x_exti.h"
#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_timer.h"
#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_ssp.h"
#include "fatfs10b/ff.h"
#include "ili9340.h"
#include "lpc177x_8x_clkpwr.h"
#include "conf.h"

#include "font.h"
#include "mmc_interface.h"
#include "lpc178x_mmc.h"
#include "mmc_driver.h"
#include "gps_uart.h"
#include "map_renderer.h"
#include "tsc2046.h"
#include "latlondist.h"
#include "ts.h"

//States used in the main loop
//No GPS lock
#define STATE_NOLOCK		0

//Have a lock, but no run started
#define STATE_LOCK_IDLE		1

//Currently tracking a run
#define STATE_LOG			2

//Had a lock, lost it
#define STATE_LOSTLOCK		3

//Showing menu
#define STATE_MENU			4

//Temporary state to redraw menu
#define STATE_MENU_ENTRY	5


//FatFS stuff
FATFS sdCard;
FIL dataFile;


//Use 16k peripheral RAM as a cache
//uint16_t cache[8192] __attribute__ ((section("RAM2")));

void hardwareInit();
void disableUnusedClocks();
uint8_t showSplashScreen();

int main()
{
	hardwareInit();
	
	//Save some power on DMA
	CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCGPDMA, DISABLE);
	
	//Wait for GPS fix before doing anything
	//FIXME: May want some better way of doing this
	while(!gpsData.valid)
	{
		printStr("   ", 3, x, y);
		Delay(1000);
		printStr(".  ", 3, x, y);
		Delay(1000);
		printStr(".. ", 3, x, y);
		Delay(1000);
		printStr("...", 3, x, y);
		Delay(1000);
	}
	
	//Black out the screen
	ili9340_set_view(ROT_90, 0, TFTHEIGHT_PHYS-1, 0, TFTWIDTH_PHYS-1);
	ILI9340_CS_ENABLE();
	ili9340_writeRegOnly(0x002c);
	ILI9340_MODE_DATA();
	ili9340_floodFill(0x0000, 320*240);
	ILI9340_CS_DISABLE();
	
	#define LOST_GPS_MSG "LOST GPS LOCK"
	#define LOST_GPS_MSG_LEN 13
	x = (screenInfo.w-LOST_GPS_MSG_LEN*FONT_W)/2;
	y = (screenInfo.h-FONT_H)/2;
	
	//Keep track of 
	#define NO_LAT_LON 999.0f
	float lastLat = NO_LAT_LON;
	float lastLon = NO_LAT_LON;
	float distance = 0.0f;
	
	//How long this trip has been
	uint16_t sec = 0;
	
	//String containing the distance travelled and time
	//Screen is 320 pixels = 40 characters wide
	//xx.x km: 7 chars
	//h:mm:ss: 7 chars
	//Space between the two: 4
	
	#define RUNINFO_LEN 18
	uint16_t RUNINFO_X = (screenInfo.w - RUNINFO_LEN*FONT_W)/2;
	#define RUNINFO_Y (screenInfo.h - FONT_H)
	uint8_t runInfo[RUNINFO_LEN];
	//0 and 1 are km
	runInfo[2] = '.';
	//3 is 0.1 km
	runInfo[4] = ' ';
	runInfo[5] = 'K';
	runInfo[6] = 'M';
	runInfo[7] = ' ';
	runInfo[8] = ' ';
	runInfo[9] = ' ';
	runInfo[10] = ' ';
	//11 is hours
	runInfo[12] = ':';
	//13 and 14 are minutes
	runInfo[15] = ':';
	//16 and 17 are seconds
	
	
	//uint8_t state = STATE_NOLOCK;
	//FIXME: Don't really start in menu state
	uint8_t state = STATE_MENU_ENTRY;
	while(1)
	{
		//No lock yet- show waiting for GPS lock screen
		//FIXME: Should probably allow panning of map or something
		//FIXME: This state will never really happen, as we don't go past the boot screen until we have a lock at the moment
		if(state == STATE_NOLOCK)
		{
			UART_Send(UART_0, (uint8_t*)"NOLOCK\n\r", 8, BLOCKING);
			//If we have a GPS lock, move into LOCK_IDLE state
			if(gpsData.valid)
			{
				state = STATE_LOCK_IDLE;
			}
		} //STATE_NOLOCK
		
		else if(state == STATE_LOCK_IDLE)
		{
			UART_Send(UART_0, (uint8_t*)"LOCKIDLE\n\r", 10, BLOCKING);
			//If we lost our lock, go to LOST_LOCK state
			if(!gpsData.valid)
			{
				state = STATE_LOSTLOCK;
			}
			
			//Otherwise, update the map if the screen is on
			if(gpsUpdated)
			{
				gpsUpdated = 0;
				if(isLcdBlOn())
				{
					CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCGPDMA, ENABLE);
					redrawMap(16);
					CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCGPDMA, DISABLE);
				}
			}
			
			//FIXME: Remove
			/*runInfo[1] = ((uint8_t)(gpsData.lat))%10 + '0';
			runInfo[0] = (uint8_t)(gpsData.lat/10.0f) + '0';
			runInfo[8] = ((uint8_t)(-gpsData.lon))%10 + '0';
			runInfo[7] = (uint8_t)(-gpsData.lon/10.0f) + '0';
			printStr((int8_t*)runInfo, RUNINFO_LEN, RUNINFO_X, RUNINFO_Y);*/
			if(ret)
			{
				uint8_t foo = ret + '0';
				UART_Send(UART_0, (uint8_t*)"ret:", 4, BLOCKING);
				UART_Send(UART_0, &foo, 1, BLOCKING);
				UART_Send(UART_0, (uint8_t*)"\r\n", 2, BLOCKING);
			}
			
		} //STATE_LOCK_IDLE
		
		else if(state == STATE_LOG)
		{
			//FIXME: update deltas, refresh screen, write to log file
		}
		
		else if(state == STATE_LOSTLOCK)
		{
			if(gpsData.valid)
			{
				state = STATE_LOCK_IDLE;
			}
			printStr(LOST_GPS_MSG, LOST_GPS_MSG_LEN, x, y);
		}
		
		else if(state == STATE_MENU_ENTRY)
		{
			//Options:
			//Enable/disable GPS(?) - UL
			//Start/stop run - UR
			//Save log - LL
			//Exit (back to map) - LR
			
			//Clear the screen
			ili9340_set_view(screenInfo.rot, 0, screenInfo.w - 1, 0, screenInfo.h - 1);
			ILI9340_CS_ENABLE();
			ili9340_writeRegOnly(0x002c);
			ILI9340_MODE_DATA();
			ili9340_floodFill(0x0000, TFTWIDTH_PHYS*TFTHEIGHT_PHYS);
			
			//Start/stop GPS pane
			uint16_t msgy = (TFTWIDTH_PHYS/2 - FONT_H)/2;
			const uint16_t tfth2 = TFTHEIGHT_PHYS/2;
			if(gpsActive)
			{
				#define GPS_OFF_STR "TURN GPS OFF"
				#define GPS_OFF_STR_L 12
				printStr((uint8_t*)GPS_OFF_STR, GPS_OFF_STR_L, (tfth2 - FONT_W*GPS_OFF_STR_L)/2, msgy);
			}
			else
			{
				#define GPS_ON_STR "TURN GPS ON"
				#define GPS_ON_STR_L 11
				printStr((uint8_t*)GPS_ON_STR, GPS_ON_STR_L, (tfth2 - FONT_W*GPS_ON_STR_L)/2, msgy);
			}
			
			//Save log pane
			//FIXME: Probably need if statement checking if there's a log to save
			#define SAVE_LOG_STR "SAVE LOG"
			#define SAVE_LOG_STR_L 8
			printStr((uint8_t*)SAVE_LOG_STR, SAVE_LOG_STR_L, (tfth2 - FONT_W*SAVE_LOG_STR_L)/2, msgy + TFTWIDTH_PHYS/2);
			
			//Start/stop run pane
			//FIXME: Needs a variable to know if a run is in-progress
			printStr((uint8_t*)"START/STOP", 10, tfth2 + (tfth2 - FONT_W*10)/2, msgy);
			
			//Back button
			#define BACK_STR "BACK"
			#define BACK_STR_L 4
			printStr((uint8_t*)BACK_STR, BACK_STR_L, tfth2 + (tfth2 - FONT_W*BACK_STR_L)/2, msgy + TFTWIDTH_PHYS/2);
			
			
			//We never actually DO anything in this state; go to menu state
			state = STATE_MENU;
		}
		
		else if(state == STATE_MENU)
		{
			if(touchFlag)
			{
				Point p = getTouchCoords();
				
				//Left half of screen
				if(p.x < TFTHEIGHT_PHYS/2)
				{
					//Top half of screen - Enable/disable GPS
					if(p.y < TFTWIDTH_PHYS/2)
					{
						if(gpsActive)
						{
							GPS_Sleep();
						}
						else
						{
							GPS_Wakeup();
						}
						state = STATE_MENU_ENTRY;
					}
					//Bottom half of screen - Save log
					else
					{
						//FIXME: Do something
					}
				}
				//Right half of screen
				else
				{
					//Top half of screen - Start/stop run
					if(p.y < TFTWIDTH_PHYS/2)
					{
						//FIXME: Do something
					}
					//Bottom half of screen - Back
					else
					{
						//FIXME: Need to select appropriate state
						//What's here doesn't count
						if(gpsData.valid)
						{
							state = STATE_LOCK_IDLE;
						}
						else
						{
							state = STATE_LOSTLOCK;
						}
					}
				}
			} //if(touchFlag)
		} //state == STATE_MENU
		
		//Unknown state; go back to NOLOCK
		else
		{
			state = STATE_NOLOCK;
		}
		
		//Regardless of state, assume touchscreen events were handled if we care about them
		touchFlag = 0;
		
		//Regardless of state, go to sleep
		CLKPWR_Sleep();
		
		
		/*
		//If we don't have a GPS lock, display an error
		if(!gpsData.valid)
		{
			printStr(LOST_GPS_MSG, LOST_GPS_MSG_LEN, x, y);
			lastLat = NO_LAT_LON;
			lastLon = NO_LAT_LON;
			
			//And then just sleep until we get a lock
			while(!gpsData.valid)
			{
				CLKPWR_Sleep();
			}
		}
		
		//If we've got a GPS lock, show our current location
		while(gpsData.valid)
		{
			if(gpsUpdated)
			{
				gpsUpdated = 0;
				
				if( (lastLat != NO_LAT_LON) && (lastLon != NO_LAT_LON))
				{
					float delta = distLatLon(lastLat, lastLon, gpsData.lat, gpsData.lon);
					//If the distance is less than 100m, in 1 sec, add it
					//Otherwise, assume it's a fluke (I'm not running at 224 miles/hour)
					//This is especially relevant for the first delta-
					//At that point, lastLat and lastLon will be whatever
					//gpsData.lat/lon started as- an unknown value
					if(delta < 0.1f)
					{
						distance += delta;
					}
				}
				
				//Only bother redrawing the screen if the screen is on
				//FIXME: should really power down LCD when BL is off; this probably isn't the place to do that
				if(isLcdBlOn())
				{
					CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCGPDMA, ENABLE);
					redrawMap(16);
					CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCGPDMA, DISABLE);
					
					//Update info string
					runInfo[0] = (uint8_t)(distance/10.0f) + '0';
					runInfo[1] = (uint8_t)(((uint8_t)distance) % 10) + '0';
					runInfo[3] = (uint8_t)(((uint16_t)(distance*10.0f)) % 10) + '0';
					
					uint16_t secl = sec % 60;
					runInfo[17] = (uint8_t)(secl % 10) + '0';
					runInfo[16] = (uint8_t)(secl / 10) + '0';
					
					uint16_t minl = (sec / 60) % 60;
					runInfo[14] = (uint8_t)(minl % 10) + '0';
					runInfo[13] = (uint8_t)(minl / 10) + '0';
					
					runInfo[11] = (uint8_t)(sec / 3600) + '0';
					
					//Print info to bottom of screen
					printStr((int8_t*)runInfo, RUNINFO_LEN, RUNINFO_X, RUNINFO_Y);
				}
				
				sec++;
				lastLat = gpsData.lat;
				lastLon = gpsData.lon;
			}
			CLKPWR_Sleep();
		}*/
	}
	
	return 0;
}


//Does initial setup of hardware
//Moved here to keep main() clean(er)
void hardwareInit()
{
	//First things first... turn off LCD backlight
	DAC_Init(0);
	//350 has LEDs practically off- screen is still black
	//400 is dim, but visible
	//450 draws an extra maybe 10 mA at 5V
	//DAC_UpdateValue(0, 450);
	DAC_UpdateValue(0, 0);
	
	//Turn off some other parts we don't use
	disableUnusedClocks();
	
	//Set up GPIOs
	GPIO_Init();
	GPIO_SetDir(1, 0xffff0000, GPIO_DIRECTION_OUTPUT);
	//GPIO_SetDir(0, 1<<14, GPIO_DIRECTION_OUTPUT); //TSC2046 CS
	GPIO_SetDir(3, (1<<23)|(1<<24)|(1<<25)|(1<<26), GPIO_DIRECTION_OUTPUT);
	GPIO_SetDir(4, 1<<5, GPIO_DIRECTION_OUTPUT); //LCD reset line
	
	//Set read line high
	LPC_GPIO3->SET = 1<<23;
	
	//GPS enable pin is p5[4]- NOTE: No longer used
	//Start with GPS on
	//GPIO_SetDir(5, 1<<4, GPIO_DIRECTION_OUTPUT);
	//LPC_GPIO5->SET = 1<<4;
	
	//Configure a timer so we can delay
	TIM_TIMERCFG_Type tinit;
	tinit.PrescaleOption = TIM_PRESCALE_TICKVAL;
	tinit.PrescaleValue = 30; //12 seems to be the correct value for 24 MHz (if peripheral clock is divided by 2)
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &tinit);
	
	//Initialize the LCD
	Delay(10);
	ili9340_init(DEFAULT_ROT);
	
	//Configure UART so we can debug
	UART_CFG_Type cfg;
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;
	
    //setting these to '1' gives UART0 control of p0[2,3]
	PINSEL_ConfigPin(0, 2, 1);
	PINSEL_ConfigPin(0, 3, 1);

	UART_ConfigStructInit(&cfg);
		
	UART_Init(UART_0, &cfg);

	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
	UART_FIFOConfig(UART_0, &UARTFIFOConfigStruct);
	UART_TxCmd(UART_0, ENABLE);
	
	//Set up the SD card
	mmc_inithardware();
	UART_Send(UART_0, (uint8_t*)"MCI initialized\n\r", 17, BLOCKING);
	
	int ret = mmc_init_card();
	if(ret != 0)
	{
		UART_Send(UART_0, (uint8_t*)"Card init failure\n\r", 19, BLOCKING);
		
		while(1)
		{
		}
	}
	UART_Send(UART_0, (uint8_t*)"Card initialized\n\r", 18, BLOCKING);
	
	//Try to use FatFS
	FRESULT res = f_mount(&sdCard, "0:", 1);
	if(res != FR_OK)
	{
		uint8_t buf[4];
		buf[0] = res / 10 + '0';
		buf[1] = res % 10 + '0';
		buf[2] = '\n';
		buf[3] = '\r';
		UART_Send(UART_0, (uint8_t*)"Mount failure: ", 15, BLOCKING);
		UART_Send(UART_0, buf, 4, BLOCKING);
		while(1)
		{
		}
	}
	
	UART_Send(UART_0, (uint8_t*)"Mounted card\n\r", 14, BLOCKING);
	
	//Render the spash screen
	if(showSplashScreen())
	{
		UART_Send(UART_0, (uint8_t*)"Failed to show splash\n\r", 23, BLOCKING);
		while(1)
		{
		}
	}
	
	
	//FIXME: Probably a better spot for this
#if DIST_METHOD == DIST_METHOD_LINEAR
	populateLatLonList();
#endif
	
	//turn backlight on
	DAC_UpdateValue(0, DEFAULT_LCD_BACKLIGHT);
	#define WAITMSGLEN 20
	uint16_t x = (screenInfo.w-((WAITMSGLEN+3)*FONT_W))/2;
	uint16_t y = screenInfo.h - FONT_H;
	printStr("WAITING FOR GPS LOCK", WAITMSGLEN, x, y);
	x += FONT_W*WAITMSGLEN;
	
	GPS_Init(9600);
	gpsData.valid = 0;
	
	//FIXME: Debug
	UART_Send(UART_0, "GPS init done\n\r", 15, BLOCKING);
	
	//Initialize touchscreen
	TSC2046_init();
	configurePENIRQ();
	
	return;
}


void disableUnusedClocks()
{
	//Turn off parts we don't use
	//See page 28 of user manual
	//See also section 10.2 of datasheet
	CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCTIM1, DISABLE);
	CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCUART1, DISABLE);
	CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCI2C0, DISABLE);
	CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCI2C1, DISABLE);
	CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCI2C2, DISABLE);
	CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCRTC, DISABLE);
	
	return;
}

uint8_t showSplashScreen()
{
	//Fill screen with black
	ili9340_set_renderarea(screenInfo.rot, screenInfo.w - 1, 0, screenInfo.h - 1);
	ILI9340_CS_ENABLE();
	ili9340_writeRegOnly(0x002c);
	ILI9340_MODE_DATA();
	ili9340_floodFill(0x0000, 320 * 240);
	
	//Open splash screen file
	//12 (8.3) characters is the limit for this string, unless you enable LFN
	if(f_open(&dataFile, "splash.bmp", FA_OPEN_EXISTING|FA_READ) != FR_OK)
	{
		UART_Send(UART_0, (uint8_t*)"splash failure\n\r", 16, BLOCKING);
		return 1;
	}
	//Render splash
	{
		//read splash size
		uint32_t splashdim[2], br;
		if(f_lseek(&dataFile, 0x12) != FR_OK)
		{
			UART_Send(UART_0, (uint8_t*)"no seek to w\n\r", 14, BLOCKING);
			return 2;
		}
		if( f_read(&dataFile, splashdim, 8, (UINT*)&br) != FR_OK )
		{
			UART_Send(UART_0, (uint8_t*)"no read w/h\n\r", 13, BLOCKING);
			return 3;
		}
		
		//Bitmaps pack rows somewhat; probably not an issue for us, but hey
		uint32_t rowsize = (16 * splashdim[0] + 31)>>5 * 4;
		
		//Read pixel data offset
		uint32_t pixel_offset;
		if(f_lseek(&dataFile, 0x0a) != FR_OK)
		{
			UART_Send(UART_0, (uint8_t*)"Failed to seek to data offset\n\r", 31, BLOCKING);
			return 4;
		}
		if( f_read(&dataFile, &pixel_offset, 4, (UINT*)&br) != FR_OK )
		{
			UART_Send(UART_0, (uint8_t*)"Failed to read data offset\n\r", 28, BLOCKING);
			return 5;
		}
		if( f_lseek(&dataFile, pixel_offset) != FR_OK )
		{
			UART_Send(UART_0, (uint8_t*)"Failed to seek to pixel offset\n\r", 32, BLOCKING);
			return 6;
		}
		
		//set position for splash image
		ili9340_set_renderarea((screenInfo.w-splashdim[0])/2, (screenInfo.w-splashdim[0])/2 + splashdim[0] - 1, (screenInfo.h-splashdim[1])/2, (screenInfo.h-splashdim[1])/2 + splashdim[1] - 1);
		
		//Make sure we're in pixel-writing mode
		ILI9340_CS_ENABLE();
		ili9340_writeRegOnly(0x002c);
		ILI9340_MODE_DATA();
		
		//Read and render pixels	
		//FIXME: Ignores row width...	
		#define UNROLL_VAL 20
		uint16_t splashcache[UNROLL_VAL];
		int i;
		for(i = 0; i < splashdim[1]*splashdim[0]/UNROLL_VAL; i++)
		{
			f_read(&dataFile, splashcache, UNROLL_VAL*2, (UINT*)&br);
			ili9340_writeDataMultiple(splashcache, UNROLL_VAL);
		}
		
	}
	ILI9340_CS_DISABLE();
	
	return 0;
}


void HardFault_Handler(void)
{	
	uint32_t bfar = SCB->BFAR;
	uint32_t reg;
	uint8_t regval[12];
	regval[0] = '0';
	regval[1] = 'x';
	regval[11] = '\r';
	regval[10] = '\n';
	
	while(1)
	{
		//Bus fault
		if(SCB->CFSR & SCB_CFSR_BUSFAULTSR_Msk)
		{
			reg = SCB->CFSR;
			uint8_t msg[] = "bus fault: ";
			UART_Send(UART_0, msg, 11, BLOCKING);
			
			char i;
			for(i = 0; i < 8; i++)
			{
				uint8_t nibble = (reg>>4*(7-i)) & 0x0f;
				regval[i+2] = nibble < 10 ? nibble + '0' : nibble + 'a' - 10;
			}
			
			UART_Send(UART_0, regval, 12, BLOCKING);
			
			//If BFAR is valid
			if(reg & 1<<15)
			{
				uint8_t msg[] = "BFAR: ";
				UART_Send(UART_0, msg, 6, BLOCKING);
				
				char i;
				for(i = 0; i < 8; i++)
				{
					uint8_t nibble = (bfar>>4*(7-i)) & 0x0f;
					regval[i+2] = nibble < 10 ? nibble + '0' : nibble + 'a' - 10;
				}
				
				UART_Send(UART_0, regval, 12, BLOCKING);
			}
			
			while(1)
			{
			}
		}
		//Memory fault
		else if(SCB->CFSR & SCB_CFSR_MEMFAULTSR_Msk)
		{
			uint8_t msg[] = "mem fault\n\r";
			UART_Send(UART_0, msg, 11, BLOCKING);
			while(1)
			{
			}
		}
		//Usage fault
		else if(SCB->CFSR & SCB_CFSR_USGFAULTSR_Msk)
		{
			reg = SCB->CFSR;
			uint8_t msg[] = "usg fault\n\r";
			UART_Send(UART_0, msg, 11, BLOCKING);
			
			char i;
			for(i = 0; i < 8; i++)
			{
				uint8_t nibble = (reg>>4*(7-i)) & 0x0f;
				regval[i+2] = nibble < 10 ? nibble + '0' : nibble + 'a' - 10;
			}
			
			UART_Send(UART_0, regval, 12, BLOCKING);
			
			while(1)
			{
			}
		}
	}
	
}
