/*
 * dac8552.h
 *
 *  Created on: Mar 17, 2020
 *      Author: vasug
 */

#ifndef INCLUDE_DAC8552_H_
#define INCLUDE_DAC8552_H_

#include "spi.h"


#define DAC_A               0
#define DAC_B               1

#define SPI_BASE_ADDR       spiREG3     //TODO: change to the appropriate spiREG.

#define CONFIG_DAT_SIZE     8U
#define DAT_SIZE            16U


void dacInit(void);
void dacWrite(uint16 *dat, int chan);

#endif /* INCLUDE_DAC8552_H_ */
