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

#include "atoi.h"
#include "fatfs10b/ff.h"
#include "gps_uart.h"
#include "ili9340.h"
#include "map_renderer.h"
#include "map_tiles.h"

//FIXME: Debug
#include "lpc177x_8x_uart.h"
#include <math.h>



uint8_t redrawMap(uint8_t zoom)
{	
	const uint32_t pow2zoom = 1<<zoom;
	uint32_t br; //place to store f_read bytes read
	//tile X and Y of the upper-left-hand corner of the screen
	//Want to store lat/lon in case we get interrupted by UART
	//That shouldn't happen, but hey...
	const float lat = gpsData.lat;
	const float lon = gpsData.lon;
	float tx;
	float ty;
	coord2tile(lat, lon, &tx, &ty, zoom);
	
	//Render area settings, adjusted for rotation
	const uint16_t renderW = MAP_RENDER_W;
	const uint16_t renderH = MAP_RENDER_H;
	const uint16_t renderX = screenInfo.rot & 0x1 ? MAP_RENDER_X : MAP_RENDER_Y;
	const uint16_t renderY = screenInfo.rot & 0x1 ? MAP_RENDER_Y : MAP_RENDER_X;
	
	//tx and ty now have the tile value of the CENTER of the screen
	//Shift them to the upper left corner
	tx -= ((float)renderW)/2/MAP_TILE_SIZE;
	if(tx < 0.0f)
	{
		tx += pow2zoom;
	}
	
	ty -= ((float)renderH)/2/MAP_TILE_SIZE;
	if(ty < 0.0f)
	{
		ty += pow2zoom;
	}
	
	
	//Now, get the integer tile number of first tile to render, as well as
	//the pixel offset into that tile
	uint32_t txi = (uint32_t)tx;
	uint32_t tyi = (uint32_t)ty;
	const uint8_t offx = (uint8_t)((tx - (float)txi) * MAP_TILE_SIZE);
	const uint8_t offy = (uint8_t)((ty - (float)tyi) * MAP_TILE_SIZE);
	
	//Render the tiles
	uint8_t i, j, k;
	
	//How many tile we have to load to cover the screen in both dimensions
	const uint8_t tilesX = (renderW + offx - 1) / MAP_TILE_SIZE + 1;
	const uint8_t tilesY = (renderH  + offy - 1) / MAP_TILE_SIZE + 1;

	
	//NOTE: assumes we have enough space to hold this...
	uint16_t pixelData[MAP_TILE_SIZE];
	
	//NOTE: assumes this number is less than the max number of files we can have open
	FIL tiles[tilesX];
	
	//Temporary file name buffer
	char buf[16];
	
	//Offsets within files to actual pixel data
	uint32_t pixelDataOffset[tilesX];
	
	//Change to the appropriate zoom level directory
	buf[0] = '/';
	itoau(zoom, buf+1);
#ifdef MAP_RENDER_DEBUG
	if(f_chdir(buf) != FR_OK)
	{
		return 1;
	}
#else
	f_chdir(buf);
#endif

	
	//Load tiles and render to screen
	//Set the window to our rendering area
	ili9340_set_view(screenInfo.rot, renderX, renderX + renderW - 1, renderY, renderY + renderH - 1);
	ILI9340_CS_ENABLE();
	
	//Put LCD into write-pixels-to-screen mode
	ili9340_writeRegOnly(0x002c);
	ILI9340_MODE_DATA();
	
	//How many rows have been rendered thus far
	uint16_t rowsRendered = 0;
	
	//j iterates over rows of tiles
	for(j = 0; j < tilesY; j++)
	{
		//Open the files
		for(i = 0; i < tilesX; i++)
		{
#ifdef MAP_RENDER_DEBUG
			if(f_chdir(itoau((txi + i) % pow2zoom, buf)) != FR_OK)
			{
				return 2;
			}
#else
			f_chdir(itoau((txi + i) % pow2zoom, buf));
#endif
			
			//Get the actual tile name
			itoau((tyi + j) % pow2zoom, buf);
			for(k = 0; buf[k] != '\0'; k++)
			{
			}
			buf[k++] = '.';
			buf[k++] = 'b';
			buf[k++] = 'm';
			buf[k++] = 'p';
			buf[k] = '\0';
			
#ifdef MAP_RENDER_DEBUG
			uint8_t ret = f_open(&(tiles[i]), buf, FA_OPEN_EXISTING|FA_READ);
			if(ret != FR_OK)
			{
				return 3;
			}
#else
			f_open(&(tiles[i]), buf, FA_OPEN_EXISTING|FA_READ);
#endif
			
			//Get offset to real data
			f_lseek(&(tiles[i]), 0xa);
#ifdef MAP_RENDER_DEBUG
			if(f_read(&(tiles[i]), &(pixelDataOffset[i]), 4, (UINT*)&br) != FR_OK) //NOTE: Makes endianness assumptions
			{
				return 4;
			}
#else
			f_read(&(tiles[i]), &(pixelDataOffset[i]), 4, (UINT*)&br); //NOTE: Makes endianness assumptions
#endif
			
			//Move back up to zoom directory
			f_chdir("..");
		}
		
		//Incredibly convoluted logic to render tiles here
		//Each tile:
		//	f_seek to correct location
		//	f_read pixel data
		//	render pixels
		uint32_t currentOffset[tilesX];
		for(k = 0; k < tilesX; k++)
		{
			currentOffset[k] = pixelDataOffset[k] \
								+ (j == 0 ? offy * 2 * MAP_TILE_SIZE : 0) \
								+ (k == 0 ? offx * 2 : 0);
		}
		
		
		//How many rows we've rendered from this set of tiles
		uint16_t tileRowsRendered = (j == 0 ? offy : 0);

		//Iterate over rows
		//rowsRendered < renderH: This should stop the partial tiles being
		//	rendered in the last tile row at the right point
		//tileRowsRendered < MAP_TILE_SIZE: This should stop us from trying
		//	to render too much data from any one tile

		//How many pixels to read in (NOT how many bytes- this is multiplied by 2 later)
		uint16_t readCounts[tilesX];
		uint16_t pixTotal = 0;
		for(k = 0; k < tilesX; k++)
		{
			uint16_t readCount = MAP_TILE_SIZE;
			if(k == 0)
			{
				readCount = MAP_TILE_SIZE - offx;
			}
			else if(k + 1 == tilesX)
			{
				readCount = renderW - pixTotal;
			}
			readCounts[k] = readCount;
			pixTotal += readCount;
		}
		
		
		while(rowsRendered < renderH && tileRowsRendered < MAP_TILE_SIZE)
		{
			//NOTE: One iteration of this loop takes ~4.2 ms, sometimes up to 11 or so
			
			//iterate over the tiles and render a row of pixels
			//uint16_t pixTotal = 0;
			for(k = 0; k < tilesX; k++)
			{
				//NOTE: Seeking and updating offset takes negligible time
				//Seek to data we're rendering
				f_lseek(&(tiles[k]), currentOffset[k]);
				
				//Update offset to next row
				currentOffset[k] += MAP_TILE_SIZE*2;

				//Read in data
				//NOTE: Reading takes up to ~3.5 ms, typically around 1.7ms
#ifdef MAP_RENDER_DEBUG
				if(f_read(&(tiles[k]), pixelData, readCounts[k]*2, (UINT*)&br) != FR_OK)
				{
					return 5;
				}
#else
				f_read(&(tiles[k]), pixelData, readCounts[k]*2, (UINT*)&br);
#endif

				ili9340_writeDataMultiple(pixelData, readCounts[k]);
			}
			
			tileRowsRendered++;
			rowsRendered++;
		}
		
		//Close the files
		for(i = 0; i < tilesX; i++)
		{
			//FIXME: Error checking here
			f_close(&(tiles[i]));
		}
	}
	
	
	//Render the user position (right in middle of map render area)
	//Set the window area to 
	ili9340_set_renderarea(renderX + renderW/2 - MAP_RENDER_USER_SIZE, renderX + renderW/2 + MAP_RENDER_USER_SIZE - 1, renderY + renderH/2 - MAP_RENDER_USER_SIZE, renderY + renderH/2 + MAP_RENDER_USER_SIZE - 1);
	ILI9340_CS_ENABLE();
	
	//Put LCD into write-pixels-to-screen mode
	ili9340_writeRegOnly(0x002c);
	ILI9340_MODE_DATA();
	
	for(i = 0; i < MAP_RENDER_USER_SIZE * MAP_RENDER_USER_SIZE * 4; i++)
	{
		ili9340_writeData(MAP_RENDER_USER_COLOR);
	}
	
	ILI9340_CS_DISABLE();
	
	return 0;
}
