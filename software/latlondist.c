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

#include "latlondist.h"


//NOTE: Single-precision haversine (regardless of approximations) seems
//to over-estimate distance mostly changing in lat, and under-estimate
//distance mostly changing in lon, at least for small changes around
//where I run.
#if DIST_METHOD == DIST_METHOD_HAVERSINE
#error You sure you want to use this?

//Haversien formula, no sine approximation. Uses floats, so still accumulates
//error that way.
float distLatLon(float lat0, float lon0, float lat1, float lon1)
{
	lat0 = deg2rad_f(lat0);
	lon0 = deg2rad_f(lon0);
	lat1 = deg2rad_f(lat1);
	lon1 = deg2rad_f(lon1);
	float dlat = lat1 - lat0;
	float dlon = lon1 - lon0;
	double sindlat = sin(dlat/2.0);
	sindlat *= sindlat;
	double sindlon = sin(dlon/2.0);
	sindlon *= sindlon;
	double a = sindlat + cos(lat0) * cos(lat1) * sindlon;
	double c = 2 * atan2(sqrt(a), sqrt(1-a));

	return WORLD_R * c;
}

#elif DIST_METHOD == DIST_METHOD_HAVERSINE_APPROX
#error You sure you want to use this?

//Haversine formula, except approximating sin(x) = x. As this is likely
//to only ever deal with small values fed to sine, this works well and
//should avoid some excessive calculations
float distLatLon(float lat0, float lon0, float lat1, float lon1)
{
	lat0 = deg2rad_f(lat0);
	lon0 = deg2rad_f(lon0);
	lat1 = deg2rad_f(lat1);
	lon1 = deg2rad_f(lon1);
	float dlat = lat1 - lat0;
	float dlon = lon1 - lon0;
	float sindlat = dlat/2.0f;
	sindlat *= sindlat;
	float sindlon = dlon/2.0f;
	sindlon *= sindlon;
	float a = sindlat + cosf(lat0) * cosf(lat1) * sindlon;
	float c = 2 * atan2f(sqrtf(a), sqrtf(1-a));

	return HAVERSINE_WORLD_R * c;
}

#elif DIST_METHOD == DIST_METHOD_LINEAR
float latf[180];
float lonf[180];

void populateLatLonList()
{
	float lat;
	int i;
	for(lat = 0.0f, i=0; lat < 90.0f; lat += 0.5f, i++)
	{
		float latmf = LAT_MPD_F(lat * M_PI/180.0f);
		float lonmf = LON_MPD_F(lat * M_PI/180.0f);
		
		latf[i] = latmf;
		lonf[i] = lonmf;
	}
}

//Linear approximation to distance. Works well for small incremental
//changes in distance; doesn't use a lot of floating-point calculations
float distLatLon(float lat0, float lon0, float lat1, float lon1)
{
	int idx = (int)(abs(lat0) * 2 + 0.5f);
	float dlat = latf[idx] * (lat1 - lat0);
	float dlon = lonf[idx] * (lon1 - lon0);
	
	return sqrtf(dlat*dlat + dlon*dlon);
}

#endif
