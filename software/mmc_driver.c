/**
 *  \file  mmc_driver.c
 *  \brief mmc/sd Treiber.
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

#include "mmc_interface.h"
#include "lpc178x_mmc.h"
#include <stddef.h>

#include "mmc_driver.h"

// =============================================================================
// Internal Variables

static int mmc_card_initialized;        // 1: Card is initialized

static const mmc_driver_interface* mmc_hardware_interface; // Hardware Handler

#define MMC_TYPE_NONE       0
#define MMC_TYPE_SDV1       1
#define MMC_TYPE_SDV2       2
#define MMC_TYPE_SDHC       3
static int mmc_card_type;               // Type of inserted card

static int mmc_card_rca;                // Adress of card

static unsigned int mmc_card_sectors;   // Numbers of 512 Byte sectors of the card

static unsigned int mmc_card_cid[4];    // CID of the card

/* MMC/SD commands */
#define GO_IDLE_STATE                  ( 0  | MMC_RESP_NONE | MMC_NOCRC)
#define SEND_OP_COND                   ( 1  | MMC_RESP_SHORT)
#define ALL_SEND_CID                   ( 2  | MMC_RESP_LONG)
#define SET_REL_ADDR                   ( 3  | MMC_RESP_SHORT)
#define SELECT_CARD                    ( 7  | MMC_RESP_SHORT)
#define SEND_IF_COND                   ( 8  | MMC_RESP_SHORT)
#define SEND_CSD                       ( 9  | MMC_RESP_LONG)
#define STOP_TRANS                     ( 12 | MMC_RESP_SHORT)
#define SEND_STATUS                    ( 13 | MMC_RESP_SHORT)
#define SET_BLOCK_LEN                  ( 16 | MMC_RESP_SHORT)
#define READ_BLOCK                     ( 17 | MMC_RESP_SHORT)
#define READ_MULTIPLE_BLOCK            ( 18 | MMC_RESP_SHORT)
#define SET_BLOCK_COUNT                ( 23 | MMC_RESP_SHORT)
#define WRITE_BLOCK                    ( 24 | MMC_RESP_SHORT | MMC_DATA_WRITE)
#define WRITE_MULTIPLE_BLOCK           ( 25 | MMC_RESP_SHORT | MMC_DATA_WRITE)
#define APP_CMD                        ( 55 | MMC_RESP_SHORT)
#define APP_SD_SET_BUS_WIDTH           ( 6  | MMC_RESP_SHORT)
#define APP_SD_SEND_OP_COND            ( 41 | MMC_RESP_SHORT | MMC_NOCRC)
#define APP_SD_SET_CLR_CARD_DETECT     ( 42 | MMC_RESP_SHORT)

/* Valid voltage range. Use 0x00ff8000 means 2.7-3.6 V. */
#define VALID_VOLTAGE_RANGE      0x00ff8000u

// Tell the card we can speak protocol version 2
#define OCR_HCS                  (1 << 30)


// =============================================================================
// Functions


/**
 * Extract number from bitfield
 *
 * @param   src     pointer to bitfield
 * @param   start   bit-start-offset (LSB)
 * @param   len     number of bits to parse
 * @return          extracted value
 */
static unsigned int get_field(unsigned int* src, unsigned int start, unsigned int len)
{
    unsigned int res = 0;

    for (start = start + len - 1; len; len--) {
        res <<= 1;
        res |= ((src[start >> 5]) >> (start & 0x1F)) & 0x01;
        start--;
    }
    return res;
}


/**
 * mmc_waitbusy
 *
 * Wait until the card is not busy any more.
 *
 * @return      0 if OK
 */
static int mmc_waitbusy(void)
{
    unsigned int response[4];
    unsigned int retry;
    unsigned int cmd_retry;

    // we do not like endless loops...
    for (retry = 65535; retry; retry--) {
        for (cmd_retry = 5; ; cmd_retry--) {
            if (!cmd_retry)
                return -1;  // no response from card
            if (!mmc_hardware_interface->command(SEND_STATUS, mmc_card_rca, response, NULL, 0))
                break;      // card has sent a response
        }
        if (response[0] & 0x100)
            return 0;   // card is ready
    }
    return -1;  // card is busy forever
}


/**
 * mmc_cmd
 *
 * Send a command to the card. Do retries.
 *
 * @return:	0 if no error
 */
static int mmc_cmd(unsigned int cmd, unsigned int arg, unsigned int* resp,
                   void* data, unsigned int sectors)
{
    int retry;
    int retval;

    // test for NULL pointer
    if (!mmc_hardware_interface)
        return -1;

    // try to send a command, and do some retries
    for (retry = 3; retry; retry--) {
        // card may be busy from last data transfer - even if it (partly) fails
        if (data) {
            retval = mmc_waitbusy();
            if (retval)
                return retval;
        }
        retval = mmc_hardware_interface->command(cmd, arg, resp, data, sectors);
        if (data && ((sectors > 1) || retval))
            // Stop the last data transfer, sync the card to receive the next command
            (void) mmc_hardware_interface->command(STOP_TRANS, 0, NULL, NULL, 0);
        if (!retval)
            return 0;
    }
    // no luck today
    return retval;
}


/**
 * mmc_acmd
 *
 * Send an application specific command to the card. Do retries.
 *
 * @return	0 if no error
 *
 */
static int mmc_acmd(unsigned int cmd, unsigned int arg, unsigned int* resp)
{
    int retry;
    int retval;

    // test for NULL pointer
    if (!mmc_hardware_interface)
        return -1;

    // try to send an application specific command, and do some retries
    for (retry = 3; retry; retry--) {
        retval = mmc_hardware_interface->command(APP_CMD, mmc_card_rca, NULL, NULL, 0);
        if (!retval) {
            retval = mmc_hardware_interface->command(cmd, arg, resp, NULL, 0);
            if (!retval)
                return 0;
        }
    }
    // no luck today
    return retval;
}


/**
 * mmc_datatrans
 *
 * Transfer a block of data to/from the card.
 *
 * @param write         0 = read, 1 = write
 * @param data          address of data block
 * @param sector        sector number
 * @param count         number of sectors
 * @return              0 if OK
 */
static int mmc_datatrans(int write, void* data, unsigned long sector, unsigned int count)
{
    int retval;
    unsigned int response[4];

    // test for NULL pointer
    if (!mmc_hardware_interface)
        return -1;

    // no data transfer if not initialized
    if (!mmc_card_initialized)
        return -1;

    // test if card is present
    if (!mmc_hardware_interface->inserted())
        return -1;

    while (count) {
        unsigned int sectors = count;
        int multi;
        unsigned int command;
        unsigned long address;
        // don't make bigger transfers as the hardware allows
        if (sectors > mmc_hardware_interface->max_transfer_sectors)
            sectors = mmc_hardware_interface->max_transfer_sectors;
        multi = (sectors > 1);
        if (multi) {
            command = write ? WRITE_MULTIPLE_BLOCK : READ_MULTIPLE_BLOCK;
        } else {
            command = write ? WRITE_BLOCK : READ_BLOCK;
        }
        if (mmc_card_type == MMC_TYPE_SDHC)
            address = sector;
        else
            address = sector * MMC_SECTORSIZE;
        retval = mmc_cmd(command, address, response, data, sectors);
        if (retval)
            return retval;
        data += (MMC_SECTORSIZE * sectors);
        sector += sectors;
        count -= sectors;
    }
    return 0;
}


// ==========================================================================
// Interface to upper level

int mmc_writesector(void* data, unsigned long sector)
{
    return mmc_datatrans(1, data, sector, 1);
}


int mmc_writemultiplesector(void* data, unsigned long sector, int cnt)
{
    return mmc_datatrans(1, data, sector, cnt);
}


int mmc_readsector(void* data, unsigned long sector)
{
    return mmc_datatrans(0, data, sector, 1);
}

int mmc_readmultiplesector(void* data, unsigned long sector, int cnt)
{
    return mmc_datatrans(0, data, sector, cnt);
}

unsigned int mmc_getsectors(void)
{
    // no information if not initialized
    if (!mmc_card_initialized)
        return 0;
    return mmc_card_sectors;
}

// Call this function if a new SD card is inserted
int mmc_init_card(void)
{
    unsigned int response[4];
    unsigned int op_cond_arg;
    int retry;
    int retval;

    // check for NULL pointer
    if (!mmc_hardware_interface)
        return -1;

    // 400 KHz at startup
    mmc_hardware_interface->setclock(400000);

    /* Set card variables to non initialized state. */
    mmc_card_initialized = 0;
    mmc_card_type = MMC_TYPE_NONE;
    mmc_card_rca = 0;

    /* Send CMD0. If a card is installed, its in IDLE state now. */
    /* CMD0 has no response, so don't expect an answer */
    (void) mmc_cmd(GO_IDLE_STATE, 0, NULL, NULL, 0);

    /* Send CMD8 to notify v2 cards that host is v2 capable. */
    retval = mmc_cmd(SEND_IF_COND, 0x1A5, response, NULL, 0);
    if (!retval && (response[0] == 0x1A5))
        mmc_card_type = MMC_TYPE_SDV2;

    /* Ask card for compatible voltage range */
    op_cond_arg = VALID_VOLTAGE_RANGE;
    if (mmc_card_type == MMC_TYPE_SDV2)
        op_cond_arg |= OCR_HCS;     // yes, we can speak V 2.0
    retval = mmc_acmd(APP_SD_SEND_OP_COND, op_cond_arg, response);

    // Check if we have an response (is a valid card installed?)
    if (retval)
        return retval;      // without a SD card, initialisation ends here

    // Valid answer to APP_SD_SEND_OP_COND: we have a SD card
    if (mmc_card_type == MMC_TYPE_NONE)
        mmc_card_type = MMC_TYPE_SDV1;

    /* At this point the SD card is doing it's initialization. This may
    take a while. Unfortunately there is no specified timeout for this.
    5000 is considered a safe amount. */
    for (retry = 5000; ; retry--) {
        if (retry == 0)
            return -1;      // card is busy forever
        if (response[0] & 0x80000000)
            break;          // card is not busy any more
        // check again
        retval = mmc_acmd(APP_SD_SEND_OP_COND, op_cond_arg, response);
    }
    // card was not responding
    if (retval)
        return retval;

    /* Card will use block adressing */
    if (response[0] & OCR_HCS)
        mmc_card_type = MMC_TYPE_SDHC;

    /* Get card identification. */
    retval = mmc_cmd(ALL_SEND_CID, 0, mmc_card_cid, NULL, 0);
    if (retval)
        return retval;

    // set relative address
    retval = mmc_cmd(SET_REL_ADDR, 0, response, NULL, 0);
    if (retval)
        return retval;

    // card in standby state after this command
    // we have a RCA
    mmc_card_rca = response[0] & 0xffff0000;

    /* get CSD block */
    retval = mmc_cmd(SEND_CSD, mmc_card_rca, response, NULL, 0);
    if (retval)
        return retval;

    // Extract needed CSD information
    {   unsigned int csd_version = get_field(response, 126, 2);
        switch (csd_version) {
        case 0:     // SD
            {
                unsigned int csize_m  = get_field(response, 47, 3);
                unsigned int csize    = get_field(response, 62, 12);
                unsigned int r_bl_len = get_field(response, 80, 4);
                mmc_card_sectors = 1 << (csize_m + 2);
                mmc_card_sectors <<= r_bl_len;
                mmc_card_sectors *= (csize+1);
                mmc_card_sectors >>= 9;
            }
            break;
        case 1:     // SDHC
            mmc_card_sectors = (get_field(response, 48, 22) + 1) << 2;
            break;
        }
    }

    /* select card and go to transfer state */
    retval = mmc_cmd(SELECT_CARD, mmc_card_rca, response, NULL, 0);
    if (retval)
        return retval;

    /* a new selected card may be busy (from last command before de-selection) */
    retval = mmc_waitbusy();
    if (retval)
        return retval;

    /* set block length to 512 */
    retval = mmc_cmd(SET_BLOCK_LEN, MMC_SECTORSIZE, NULL, NULL, 0);
    if (retval)
        return retval;

    /* use all 4 bus bits if available */
    if (mmc_hardware_interface->flags & MMC_FLAG_4BIT) {
        if (!mmc_acmd(APP_SD_SET_BUS_WIDTH, 2, NULL)) {
            (void)mmc_hardware_interface->buswidth(4);
        }
    }
    mmc_hardware_interface->setclock(25000000);

    /* all OK */
    mmc_card_initialized = 1;

    return 0;
}


// This is the first call you should made!
void mmc_inithardware(void)
{
    // Init Hardware interface
    mmc_hardware_interface = &lpc178x_driver_interface;
    mmc_hardware_interface->inithw();

    // Init
    mmc_card_initialized = 0;
    mmc_card_type = 0;
    mmc_card_rca = 0;
    mmc_card_sectors = 0;
}

uint32_t mmc_isinit(void)
{
	return mmc_card_initialized;
}
