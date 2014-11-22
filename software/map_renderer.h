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

#ifndef MAP_RENDERER_H
#define MAP_RENDERER_H

#include <inttypes.h>

#include "font.h"

//Uncomment to enable some debugging
#define MAP_RENDER_DEBUG

//How many pixels wide/tall the map render window is, in pixels
#define MAP_RENDER_W (screenInfo.w)
#define MAP_RENDER_H (screenInfo.h - FONT_H)

//Where on the screen the map will be rendered (x,y offset of top-left map
//corner from the top left corner of the screen, in pixels)
#define MAP_RENDER_X 0
#define MAP_RENDER_Y 0

//Size, in pixels, of the radius of the user position dot
//Must be >= 2
#define MAP_RENDER_USER_SIZE 4

//Color of user position indicator, 16-bit rgb565
#define MAP_RENDER_USER_COLOR 0xf800

//Renders the map and user position based on GPS location
uint8_t redrawMap(uint8_t zoom);

#endif
