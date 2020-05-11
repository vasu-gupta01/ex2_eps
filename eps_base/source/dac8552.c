/*
 * dac8552.c
 *
 *  Created on: Mar 17, 2020
 *      Author: vasug
 *
 *  Functions to implement:
 *      dacInit           DONE
 *      dacSetVal         DONE and tested.
 *      dacSetPowerDown   TODO
 *      dacGetPowerDown   TODO
 *
 */

#include "dac8552.h"
#include "gio.h"

spiDAT1_t sConfDat;

/*
 *  @brief : Initialize spi, gio (used for CS pin), and spiDAT1_t structure to be used for data transfer.
 *
 *
 */
void dacInit(void) {
    gioInit();
    spiInit();

    gioSetDirection(SPI_PORT_ADDR, 0xFFFFFFFF);  //sets all gio pins to output.

    sConfDat.CS_HOLD = TRUE;
    sConfDat.WDEL    = TRUE;
    sConfDat.DFSEL   = SPI_FMT_0;
    sConfDat.CSNR    = 0xFF;
}


/*
 * @brief          : Set DAC Vout value.
 *
 * @param[in] dat  : Data bits to send to dac.
 *
 * @param[in] chan : chan = 0 (send to DAC A) or chan = 1 (send to DAC B).
 *
 */
void dacSetVal(uint16 val, int chan) {
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

    uint16 buffer[2];
    uint16 temp_H, temp_L;

    temp_H  = (val >> 8) & 0x00FF;          //set bits val[15...8] to temp_H[7...0]
    temp_H |= (uint16)(control_bits << 8);  //set bits control_bits[7...0] to temp_H[15...8]

    temp_L = (val << 8);  //set bits val[7...0] to temp_L[15...8]

    buffer[0] = temp_H;
    buffer[1] = temp_L;

    gioSetBit(SPI_PORT_ADDR, SPI_CS_ADDR, LOW);             //set DAC SYNC line low
    spiTransmitData(SPI_BASE_ADDR, &sConfDat, 2, buffer);   //send control data and value to DAC
    gioSetBit(SPI_PORT_ADDR, SPI_CS_ADDR, HIGH);            //set DAC SYNC line high
}

