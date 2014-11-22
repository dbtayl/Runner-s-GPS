/**
 *  \file  lpc178x_mmc.c
 *  \brief LPC178x mmc/sd driver.
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

#include "lpc178x_mmc.h"
#include "LPC177x_8x.h"
#include "system_LPC177x_8x.h"
#include "lpc_types.h"
#include "lpc177x_8x_mci.h"
#include "lpc177x_8x_gpdma.h"
#include "lpc177x_8x_clkpwr.h"


//=============================================================================
// Support
/* Transform ns into clock cycles (runtime, only 32bit multiplications):
 * Valid input range: Clock < 995 MHz, ns = 0..1000000 (1ms)
 *
 * a) Divide Clock by 16. This gives enough headroom for step b).
 * b) Multiply Clock by 69. This will adjust for decimal/binary divisor.
 *    This computation will overflow for a clock > 995 MHz!
 *    This computation will give results 0,5% larger than the real value.
 * c) Divide Clock by 1048576 (2^20). This will give enough headroom for step d).
 * d) Multiply Clock by ns. This will not overflow for ns = 0..1000000 (1ms).
 * e) Divide Clock by 4096 (2^12). This will give value in clocks.
 * f) Add 1 to account for rounding.
 * (Use runtime computations because frequencies may change in runtime).
 */
#define NS_2_CPU_CLKS(ns) ((((((SystemCoreClock >> 4)*69) >> 20)*(ns))>>12)+1)

// Delay loop for short busy waits
__attribute__( ( always_inline ) ) static inline void wait_clocks( unsigned int clocks)
{
   clocks >>= 1;          // 2 clocks per loop iteration
   while(clocks--) {
      asm("":::"memory"); // hint for gcc: do not remove this loop!
   }
}



//=============================================================================
// Configuration

#define SD_PWR_ACTIVE_LEVEL     0   // SD_PWR pin: 0 = active LOW, 1 = active HIGH

// Port Pin Definitions
#define SD_CLK   P0_19               // or P0_19
#define SD_CMD   P0_20               // or P0_20
#define SD_PWR   P0_21               // or P0_21
#define SD_DAT_0 P0_22               // or P0_22
#define SD_DAT_1 P2_11               // or P2_11
#define SD_DAT_2 P2_12              // or P2_12
#define SD_DAT_3 P2_13              // or P2_13

#define DMA_CHANNEL  0              // used DMA channel

#define MAXSPEED_BYTEDMA    10000000    // DMA controller in Byte mode is slow...

//=============================================================================
// Interface to Card Detect / Write Protect

// return != 0 if inserted
static int lpc178x_mmc_inserted(void)
{
    return 1;	// TODO implementation-specific
}


// return != 0 if write protected
static int lpc178x_mmc_protected(void)
{
    return 0;	// TODO implementation-specific
}


//=============================================================================
// internal

// Preprocessor magic: get pointer to DMA channel registers
#define CONCATx(a,b) a##b
#define CONCAT(a,b) CONCATx(a,b)
#define LPC_GPDMACHANNEL CONCAT(LPC_GPDMACH,DMA_CHANNEL)

// actual requested speed setting
static long lpc178x_mmc_freqency = 400000;
static int  lpc178x_mmc_slowdown = 0;

// set the clock
static void lpc178x_mmc_clock(long frequency)
{
    LPC_MCI->CLOCK = (LPC_MCI->CLOCK & ~0xFF) | (PeripheralClock / (frequency *2)) | 0x300;   // Clock enabled and PwrSave
}

// Wait after/before a register access.
// (3 x MCLK + 2 x PCLK)
static inline void wait_register(void) {
    wait_clocks(16);   // schould be sufficient in most cases
}


/**
 * Set the MCI clock
 *
 * @param frequency     Frequenz in Hz
 */
static void lpc178x_mmc_setclock(long frequency)
{
    lpc178x_mmc_freqency = frequency;
    lpc178x_mmc_clock(frequency);
}


// Set the buswidth for data transfer
// @param  width    1 or 4
static void lpc178x_mmc_buswidth(unsigned int width)
{
    if (width == 1) {
        LPC_MCI->CLOCK &=  ~(1 << 11);
    }
    if (width == 4) {
        LPC_MCI->CLOCK |=  (1 << 11);
    }
    wait_register();
}


// Send a command to SD card
static int lpc178x_mmc_command(unsigned int cmd, unsigned int arg, unsigned int* resp,
                               void* data, unsigned int sectors)
{
    unsigned int command;
    unsigned int write = (cmd & MMC_DATA_WRITE);
    unsigned int datactl = 0;
    unsigned int error;
    unsigned int errorpattern;

    // set argument
    LPC_MCI->ARGUMENT = arg;

    // set command
    command = cmd & 0x3F;
    if (cmd & MMC_RESP_LONG)
        command |= 0xC0;
    if (cmd & MMC_RESP_SHORT)
        command |= 0x40;
    command |= 0x400;

    // clear response
    if (resp)
        resp[0] = resp[1] = resp[2] = resp[3] = 0;

    // Set error pattern
    errorpattern =  MCI_DATA_CRC_FAIL | MCI_CMD_TIMEOUT | MCI_DATA_TIMEOUT | MCI_TX_UNDERRUN
                  | MCI_RX_OVERRUN | MCI_START_BIT_ERR;
    if (!(cmd & MMC_NOCRC))
        errorpattern |= MCI_CMD_CRC_FAIL;

    // clear status register
    LPC_MCI->CLEAR = 0x7FF;

    // Setup data transfer
    if (data) {
        // Can we do word transfer in memory?
        unsigned int memory_bsize = GPDMA_BSIZE_8;
        unsigned int memory_width = GPDMA_WIDTH_WORD;
        if ((uint32_t)data & 0x03) {
            // no, we are byte aligned.
            memory_bsize = GPDMA_BSIZE_32;
            memory_width = GPDMA_WIDTH_BYTE;
            // if external memory bandwidth is limited, we may have to adjust
            // the maximum speed.
            if ((!lpc178x_mmc_slowdown) && (lpc178x_mmc_freqency > MAXSPEED_BYTEDMA)) {
                lpc178x_mmc_clock(MAXSPEED_BYTEDMA);
                lpc178x_mmc_slowdown = 1;
            }
        } else {
            // If memory is word aligned, allow maximum speed
            if (lpc178x_mmc_slowdown) {
                lpc178x_mmc_clock(lpc178x_mmc_freqency);
                lpc178x_mmc_slowdown = 0;
            }
        }

        LPC_MCI->DATALEN = sectors * MMC_SECTORSIZE;
        // Setting of Data Timeout
        if (write)
            LPC_MCI->DATATMR = 0x3FFFFFF;   // 32* NAC
        else
            LPC_MCI->DATATMR = 0x1FFFFF;    // Worst case is 1EE81C
                                            // = 10ms * 8.0 * 25 MHz + 255 * 100 Clocks
        datactl = MCI_DATACTRL_ENABLE | MCI_DATACTRL_DMA_ENABLE | (9 << 4);     // 512 Bytes
        if (!write)
            datactl |= MCI_DATACTRL_DIR_FROM_CARD;
        // Setup DMA
        LPC_GPDMA->Config = GPDMA_DMACConfig_E;
        LPC_GPDMA->IntTCClear = (1 << DMA_CHANNEL);
        LPC_GPDMA->IntErrClr  = (1 << DMA_CHANNEL);
        LPC_GPDMACHANNEL->CLLI = 0;
        LPC_SC->DMAREQSEL &= ~(1 << GPDMA_CONN_MCI);

        if (write) {
            __DSB();    // make sure all data is in memory before starting DMA
            LPC_GPDMACHANNEL->CSrcAddr = (uint32_t)data;
            LPC_GPDMACHANNEL->CDestAddr = (uint32_t)&LPC_MCI->FIFO;
            LPC_GPDMACHANNEL->CControl  = GPDMA_DMACCxControl_TransferSize(0)     // control by MCI
                                    | GPDMA_DMACCxControl_SBSize(memory_bsize)
                                    | GPDMA_DMACCxControl_DBSize(GPDMA_BSIZE_8)
                                    | GPDMA_DMACCxControl_SWidth(memory_width)
                                    | GPDMA_DMACCxControl_DWidth(GPDMA_WIDTH_WORD)
                                    | GPDMA_DMACCxControl_SI;
            LPC_GPDMACHANNEL->CConfig   = GPDMA_DMACCxConfig_E
                                    | GPDMA_DMACCxConfig_SrcPeripheral(0)
                                    | GPDMA_DMACCxConfig_DestPeripheral(GPDMA_CONN_MCI)
                                    | GPDMA_DMACCxConfig_TransferType(5)      // see table 686 U.M.
                                    | GPDMA_DMACCxConfig_L;
        } else {
            LPC_GPDMACHANNEL->CSrcAddr  = (uint32_t)&LPC_MCI->FIFO;
            LPC_GPDMACHANNEL->CDestAddr = (uint32_t)data;
            LPC_GPDMACHANNEL->CControl  = GPDMA_DMACCxControl_TransferSize(0)     // control by MCI
                                    | GPDMA_DMACCxControl_SBSize(GPDMA_BSIZE_8)
                                    | GPDMA_DMACCxControl_DBSize(memory_bsize)
                                    | GPDMA_DMACCxControl_SWidth(GPDMA_WIDTH_WORD)
                                    | GPDMA_DMACCxControl_DWidth(memory_width)
                                    | GPDMA_DMACCxControl_DI;
            LPC_GPDMACH0->CConfig   = GPDMA_DMACCxConfig_E
                                    | GPDMA_DMACCxConfig_SrcPeripheral(GPDMA_CONN_MCI)
                                    | GPDMA_DMACCxConfig_DestPeripheral(0)
                                    | GPDMA_DMACCxConfig_TransferType(6)      // see table 686 U.M.
                                    | GPDMA_DMACCxConfig_L;
        }
        // Wait until channel is enabled
        while (!(LPC_GPDMACHANNEL->CConfig & GPDMA_DMACCxConfig_E)) {
        }
    }

    // start of command and start of data engine must be time-aligned
    // (to avoid data engine time-out)
    __disable_irq();
    // Start data engine on READ before command.
    // (avoid overflow of the FIFO).
    if (!write)
        LPC_MCI->DATACTRL = datactl;
    // start command
    LPC_MCI->COMMAND = command;
    // Wait until command engine starts
    while (!LPC_MCI->STATUS) {
        // do nothing
    }
    // wait until command sent
    while (LPC_MCI->STATUS & MCI_CMD_ACTIVE) {
        // Do nothing
    }
    error = LPC_MCI->STATUS & errorpattern;
    if (!error && write)
        // For write data commands, data engine MUST start AFTER the command was sent.
        LPC_MCI->DATACTRL = datactl;
    // End of time-critical section
    __DSB();
    __enable_irq();

    // Wait until DMA transfer complete.
    if (data) {
        while (!error) {
            unsigned int status = LPC_MCI->STATUS;
            error = status & errorpattern;
            if (status & MCI_DATA_END)
                break;
        }
        // MCI is ready. DMA controller is working on the last transfer.
        while (!error) {
            // Do we have an error from the DMA controller?
            if (LPC_GPDMA->RawIntErrStat & (1 << DMA_CHANNEL))
                error = (1 << 30);
            // Is the DMA channel running?
            if (!(LPC_GPDMACHANNEL->CConfig & (GPDMA_DMACCxConfig_A | GPDMA_DMACCxConfig_E)))
                // ready
                break;
        }

        // stop/abort DMA channel
        LPC_GPDMACHANNEL->CConfig = 0;
    }

    // clear command code
    LPC_MCI->COMMAND = 0;
    // Clear data control
    LPC_MCI->DATACTRL = 0;

    // Have we got a response?
    if (error & MCI_CMD_TIMEOUT)
        return -1;

    // Copy response
    if (resp) {
        if (cmd & MMC_RESP_SHORT)
            *resp   = LPC_MCI->RESP0;
        if (cmd & MMC_RESP_LONG) {
            // response[0] should contain bits 31..0
            *resp++ = LPC_MCI->RESP3;
            *resp++ = LPC_MCI->RESP2;
            *resp++ = LPC_MCI->RESP1;
            *resp   = LPC_MCI->RESP0;
        }
    }
    return error ? -1 : 0;
}


// Initialize MCI hardware
static int lpc178x_mmc_inithw(void)
{
    int i;

    LPC_SC->PCONP |= CLKPWR_PCONP_PCSDC;        // Enable clock to the MCI block
    // RESET the MCI controller
    LPC_SC->RSTCON0 = 1 << 28;
    wait_clocks(256);
    LPC_SC->RSTCON0 = 0;
    wait_clocks(256);
    LPC_MCI->CLOCK &= ~(1 << 8);                // Disable clock output
    wait_register();
    LPC_MCI->POWER = 0x00;                      // Disable power to SD card
    wait_register();
    LPC_SC->PCONP |= CLKPWR_PCONP_PCGPDMA;      // Enable DMA controller

#if SD_PWR_ACTIVE_LEVEL                         // Set power active level
    LPC_SC->SCS |= 0x08;
#else
    LPC_SC->SCS &= ~0x08;
#endif

    /* Disable all interrupts for now */
    LPC_MCI->MASK0 = 0;

    // Setup port functions
    LPC_IOCON->SD_CLK   = 0x32;
    LPC_IOCON->SD_CMD   = 0x32;
    LPC_IOCON->SD_PWR   = 0x32;
    LPC_IOCON->SD_DAT_0 = 0x32;
    LPC_IOCON->SD_DAT_1 = 0x32;
    LPC_IOCON->SD_DAT_2 = 0x32;
    LPC_IOCON->SD_DAT_3 = 0x32;

    /*set up clocking default mode, clear any registers as needed */
    LPC_MCI->COMMAND = 0;
    LPC_MCI->DATACTRL = 0;
    LPC_MCI->CLEAR = 0x7FF;     /* clear all pending interrupts */

    LPC_MCI->POWER = 0x02;      /* power up */
    wait_register();

    // Power up waiting time
    for (i = 0; i < 10000; i++)
        wait_clocks(NS_2_CPU_CLKS(1000)); // 1us

    LPC_MCI->POWER |= 0x01;     /* bit 1 is set already, from power up to power on */
    wait_register();

    return 0;
}


const mmc_driver_interface lpc178x_driver_interface = {
        &lpc178x_mmc_inserted,
        &lpc178x_mmc_inithw,
        &lpc178x_mmc_protected,
        &lpc178x_mmc_setclock,
        &lpc178x_mmc_command,
        &lpc178x_mmc_buswidth,
        MMC_FLAG_4BIT,
        127                     /* DataLength.max = 0xFFFF */
};
