/**
 *  \file  mmc_interface.h
 *  \brief Interface of mmc driver to the hardware
 *
 * Date: 	2011-11-17
 * Author:	Wolfgang Muees
 *
 * Copyright (c) 2012 Auerswald GmbH & Co.KG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef MMC_INTERFACE_H_
#define MMC_INTERFACE_H_

/* Response types, coded into the command */
#define MMC_RESP_NONE         0x000     // Go Idle
#define MMC_RESP_SHORT        0x100     // R1, R1B, R3
#define MMC_RESP_LONG         0x200     // R2
#define MMC_NOCRC             0x400     // Some commands have no CRC in the answer

/* Data Direction, coded into the command */
#define MMC_DATA_WRITE        0x1000

typedef int (*mmc_inserted)  (void);
typedef int (*mmc_inithw)    (void);
typedef int (*mmc_protected) (void);
typedef void (*mmc_setclock) (long frequency);
typedef int (*mmc_command)   (unsigned int cmd, unsigned int arg, unsigned int* resp,
                              void* data, unsigned int sectors);
typedef void (*mmc_buswidth) (unsigned int width);

/* Flags */
#define MMC_FLAG_4BIT       0x0001      // Interface can transfer 4 bit wide

#define MMC_SECTORSIZE      512         // defined as information for lower layers

typedef struct {
    mmc_inserted  inserted;
    mmc_inithw    inithw;
    mmc_protected protected;
    mmc_setclock  setclock;
    mmc_command   command;
    mmc_buswidth  buswidth;
    unsigned int  flags;
    unsigned int  max_transfer_sectors;
} mmc_driver_interface;

#endif /* MMC_INTERFACE_H_ */
