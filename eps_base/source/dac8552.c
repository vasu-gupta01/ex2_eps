/*
 * dac8552.c
 *
 *  Created on: Mar 17, 2020
 *      Author: vasug
 *
 *
 *
 */

#include "dac8552.h"
#include "gio.h"

spiDAT1_t sConfDat;


/*
 *  @brief : Initialize spi and dac8552 driver.
 *
 *
 */
void dacInit(void) {
    spiInit();

    sConfDat.CS_HOLD = false;
    sConfDat.WDEL    = false;
    sConfDat.DFSEL   = SPI_FMT_0;
    sConfDat.CSNR    = 0x00;
}


/*
 * @brief          : Send data to dac.
 *
 * @param[in] dat  : data bits to send to dac.
 *
 * @param[in] chan : chan = 0 (DAC A) or chan = 1 (DAC B)
 *
 */
void dacWrite(uint16 *dat, int chan) {

    uint8 control_bits;

    switch(chan) {
        case DAC_A:
            control_bits = 0x10;
            break;

        case DAC_B:
            control_bits = 0x24;
            break;

        default:
            //Set DAC A by default.
            control_bits = 0x10;
            break;
    };

    //Set SYNC low
    gioSetBit(SPI_PORT_ADDR, SPI_CS_ADDR, LOW);

    //send control bits
    spiSendData(SPI_BASE_ADDR, &sConfDat, sizeof(control_bits), &control_bits);
    spiTransmitData(SPI_BASE_ADDR, &sConfDat, sizeof(control_bits), &control_bits);

    //send data bits
    spiSendData(SPI_BASE_ADDR, &sConfDat, sizeof(*dat), dat);
    spiTransmitData(SPI_BASE_ADDR, &sConfDat, sizeof(*dat), dat);

    //Set SYNC high
    gioSetBit(SPI_PORT_ADDR, SPI_CS_ADDR, HIGH);
}

