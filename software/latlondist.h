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

#ifndef LATLONDIST_H
#define LATLONDIST_H

#include "math.h"

#define DIST_METHOD_HAVERSINE 0
#define DIST_METHOD_HAVERSINE_APPROX 1
#define DIST_METHOD_LINEAR 2

//Change this line to configure which distance measurement technique to use
//For small changes, I'm finding the linear approximation to work best.
#define DIST_METHOD DIST_METHOD_LINEAR


//Calculates the distance between two points, using method #defined above
//Returned units are km
float distLatLon(float lat0, float lon0, float lat1, float lon1);


#if DIST_METHOD == DIST_METHOD_HAVERSINE || DIST_METHOD == DIST_METHOD_HAVERSINE_APPROX
//World's radius, in km
#define HAVERSINE_WORLD_R 6371.0f


#elif DIST_METHOD == DIST_METHOD_LINEAR
/*
 * WGS84:
 * a (equatorial radius): 6,378,137.0 m exactly
 * 1/f (inverse flattening): 298.257,223,563 exactly
 * b (polar radius): 6,356,752.3142 m
 * e^2 (eccentricity squared): 0.006,694,379,990,14
 * 
 * Distance between 1 degree of lat (x) (given angle +/- 0.5 degrees, accurate to 0.01m): 111132.954 - 559.822 cos 2x + 1.175 cos 4x
 * For lon: pi a cos x / (180(1 - sqrt(e^2 sin^2(x))))
 */
//World's radius at equator
#define LIN_WORLD_R 6378.1370f
#define LIN_WORLD_E2  0.00669437999014f

#define LAT_MPD_F(x) (111.132954f - 0.559822f * cosf(2*x) + 0.001175f * cosf(4*x))
#define LON_MPD_F(x) ( M_PI * LIN_WORLD_R * cosf(x) / (180.0f*sqrtf(1-(LIN_WORLD_E2 * sinf(x)*sinf(x)))) )

extern float latf[];
extern float lonf[];

void populateLatLonList();

#else
#error Need to specify a valid distance calculation method
#endif //DIST_METHOD

#endif
