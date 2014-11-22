#ifndef MMC_DRIVER_H
#define MMC_DRIVER_H

#include <inttypes.h>

int mmc_writesector(void* data, unsigned long sector);
int mmc_writemultiplesector(void* data, unsigned long sector, int cnt);
int mmc_readsector(void* data, unsigned long sector);
int mmc_readmultiplesector(void* data, unsigned long sector, int cnt);
unsigned int mmc_getsectors(void);
int mmc_init_card(void);
void mmc_inithardware(void);
uint32_t mmc_isinit(void);

#endif
