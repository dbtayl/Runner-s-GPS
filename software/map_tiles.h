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
 * This code is to perform operations between lat/lon and "tile coordinates"
 * as would be used when using OpenStreetMap (OSM) map tiles.
 * 
 * Map tiles are stored as: "/z/x/y.png"
 */

#ifndef MAP_TILES_H
#define MAP_TILES_H

#include <inttypes.h>

//Set this to non-zero to enable extra error checking
#define MAP_TILES_PARANOID 0

//Since M_PI is a double...
#define MAP_TILE_PI 3.14159265358979323846264338327f

//Pixel dimension of a tile
#define MAP_TILE_SIZE 256

//How far in/out one can zoom on the map
#define MIN_ZOOM 3
#define MAX_ZOOM 17

//How far North/South this code will work with
#define MAX_N_S 85.05112878f

//Radian/degree conversions
#ifndef rad2deg_f
#define rad2deg_f(x) x / MAP_TILE_PI * 180.0f
#endif
#ifndef deg2rad_f
#define deg2rad_f(x) x * MAP_TILE_PI / 180.0f
#endif


//Returns whether or not the given coordinate position is valid.
//Versions are for standard lat (y) / lon (x) and tile coords (validCoordinatesT)
uint8_t validCoordinates(const float lat, const float lon);
uint8_t validCoordinatesT(const float x, const float y, const uint8_t zoom);


//Converts between "tile coords" and real lat/lon- outputs are placed in
//pointer arguments
//Returns 0 on success, something else on failure
uint8_t coord2tile(const float lat, const float lon, float* x, float* y, const uint8_t zoom);
uint8_t tile2coord(const float tileX, const float tileY, const uint8_t zoom, float* lat, float* lon);


//Computes the distance between two lat/lon pairs
float haversine(float lat0, float lon0, float lat1, float lon1);
float haversine_fast(float lat0, float lon0, float lat1, float lon1);

#endif
