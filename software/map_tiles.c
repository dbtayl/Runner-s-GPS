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

#include "inttypes.h"
#include "math.h"

#include "map_tiles.h"

#include "lpc177x_8x_uart.h"


uint8_t validCoordinates(const float lat, const float lon)
{
	if((abs(lon) > 180.0) || (abs(lat) > MAX_N_S))
	{
		return 0;
	}

	return 1;
}


uint8_t validCoordinatesT(const float x, const float y, const uint8_t zoom)
{
	const int32_t pow2zoom = 1<<zoom;
	if((x >= pow2zoom) || (y >= pow2zoom) || (x < 0) || (y < 0) || (zoom < MIN_ZOOM) || (zoom > MAX_ZOOM))
	{
		return 0;
	}

	return 1;
}


uint8_t coord2tile(const float lat, const float lon, float* x, float* y, const uint8_t zoom)
{
#if MAP_TILES_PARANOID
	if (!validCoordinates (lat, lon))
	{
		return 1;
	}
#endif

	const int32_t pow2zoom = 1<<zoom;
	
	*x = pow2zoom / 360.0f * (lon + 180.0f);

	float sinLatitude = sinf(deg2rad_f (lat));

	*y = (0.5f - logf((1.0f + sinLatitude) / (1.0f - sinLatitude)) / (4.0f * MAP_TILE_PI)) * pow2zoom;

	return 0;
}


uint8_t tile2coord(const float tileX, const float tileY, const uint8_t zoom, float* lat, float* lon)
{
#if MAP_TILES_PARANOID
	if (!validCoordinatesT (tileX, tileY, zoom))
	{
		return 1;
	}
#endif

	const int32_t pow2zoom = 1<<zoom;
	const float ppd = pow2zoom / 360.0f; //pixels per degree
	*lon = tileX / ppd - 180.0f;
	*lat = rad2deg_f(asinf((expf((tileY / pow2zoom - 0.5f) * -4.0f * MAP_TILE_PI) - 1.0f) / (1.0f + expf((tileY / pow2zoom - 0.5f) * -4.0f * MAP_TILE_PI))));

	return 0;
}
