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

#include "ili9340.h"
#include "ts.h"
#include "tsc2046.h"

Point getTouchCoords()
{
	Point tp = readTSC2046();
	
	//readTSC2046 returns coordinates related to the screen in its "natural" (portrait) state
	//Rotate them to landscape
	uint16_t tmpx = tp.x;
	tp.x = (uint16_t)((float)(tp.y - TS_XMIN) * (float)TFTHEIGHT_PHYS / (float)(TS_XMAX-TS_XMIN) + 0.5f);
	tp.y = TFTWIDTH_PHYS - (uint16_t)((float)(tmpx - TS_YMIN) * (float)TFTWIDTH_PHYS / (float)(TS_YMAX-TS_YMIN) + 0.5f);
	
	//Unsigned, so no need to check less than zero
	tp.x = tp.x >= 320 ? 319 : tp.x;
	tp.y = tp.y >= 240 ? 239 : tp.y;
	
	return tp;
}
