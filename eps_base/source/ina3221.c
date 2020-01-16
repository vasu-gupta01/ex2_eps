/*
 * ina3221.c
 *
 *  Created on: Jan 16, 2020
 *      Author: jdlazaru
 */



//
//   SDL_Arduino_INA3221 Library
//   SDL_Arduino_INA3221.cpp Arduino code - runs in continuous mode
//   Version 1.2
//   SwitchDoc Labs   September 2019
//
//



/*
    Initial code from INA219 code (Basically just a core structure left)
    @author   K.Townsend (Adafruit Industries)
    @license  BSD (see BSDlicense.txt)
*/


//THINGS TO DO
//
//wireWriteRegister --- DONE
//wireReadRegister --- DONE
//INA3221SetConfig --- DONE
//SDL_Arduino_INA3221
//begin --- DONE
//getBusVoltage_raw --- DONE
//getShuntVoltage_raw --- DONE
//getShuntVoltage_mV --- DONE
//getBusVoltage_V --- DONE
//getCurrent_mA --- DONE
//getManufID --- DONE
//
//Change cpp stuff to c
//
//
//

//#if ARDUINO >= 100
// #include "Arduino.h"
//#else
// #include "WProgram.h"
//#endif

//#include <Wire.h>
#include "i2c.h"
#include "reg_i2c.h"
#include "ina3221.h"

//#include "SDL_Arduino_INA3221.h"

#define I2CBASE i2cREG1
/**************************************************************************/
/*!
    @brief  Sends a single command byte over I2C
*/
/**************************************************************************/
void wireWriteRegister(uint8_t reg, uint16_t value)
{
  //Wire.beginTransmission(INA3221_i2caddr);
  i2cSetSlaveAdd(I2CBASE, INA3221_ADDRESS);
//  #if ARDUINO >= 100
//    Wire.write(reg);                       // Register
//    Wire.write((value >> 8) & 0xFF);       // Upper 8-bits
//    Wire.write(value & 0xFF);              // Lower 8-bits
//  #else
//    Wire.send(reg);                        // Register
//    Wire.send(value >> 8);                 // Upper 8-bits
//    Wire.send(value & 0xFF);               // Lower 8-bits
//  #endif
  i2cSendByte(I2CBASE, reg);
  i2cSend(I2CBASE,2,value);

  //Wire.endTransmission();
}

/**************************************************************************/
/*!
    @brief  Reads a 16 bit values over I2C
*/
/**************************************************************************/
void wireReadRegister(uint8_t reg, uint16_t *value)
{

  //Wire.beginTransmission(INA3221_i2caddr);
    i2cSetSlaveAdd(I2CBASE, INA3221_ADDRESS);

//  #if ARDUINO >= 100
//    Wire.write(reg);                       // Register
//  #else
//    Wire.send(reg);                        // Register
//  #endif
    i2cSendByte(I2CBASE,reg);

  //Wire.endTransmission();

  delay(1); // Max 12-bit conversion time is 586us per sample

//  Wire.requestFrom(INA3221_i2caddr, (uint8_t)2);
//  #if ARDUINO >= 100
//    // Shift values to create properly formed integer
//    *value = ((Wire.read() << 8) | Wire.read());
//  #else
//    // Shift values to create properly formed integer
//    *value = ((Wire.receive() << 8) | Wire.receive());
//  #endif

  //does direction need to be set?
  //does a function need to be called in place of requestFrom?
  i2cReceive(I2CBASE, 2, value);//make sure value is ok not being a pointer
}

//
void INA3221SetConfig(void)
{


  // Set Config register to take into account the settings above
  uint16_t config = INA3221_CONFIG_ENABLE_CHAN1 |
                    INA3221_CONFIG_ENABLE_CHAN2 |
                    INA3221_CONFIG_ENABLE_CHAN3 |
                    INA3221_CONFIG_AVG1 |
                    INA3221_CONFIG_VBUS_CT2 |
                    INA3221_CONFIG_VSH_CT2 |
                    INA3221_CONFIG_MODE_2 |
                    INA3221_CONFIG_MODE_1 |
                    INA3221_CONFIG_MODE_0;

  wireWriteRegister(INA3221_REG_CONFIG, config);
}

/**************************************************************************/
/*!
    @brief  Instantiates a new SDL_Arduino_INA3221 class
*/
/**************************************************************************/
//void SDL_Arduino_INA3221(uint8_t addr, float shuntresistor) {
//
//    INA3221_i2caddr = addr;
//    INA3221_shuntresistor = shuntresistor;
//
//}

/**************************************************************************/
/*!
    @brief  Setups the HW (defaults to 32V and 2A for calibration values)
*/
/**************************************************************************/
void begin() {
  //Wire.begin();
  i2cInit();
  // Set chip to known config values to start
  INA3221SetConfig();

   // Serial.print("shut resistor="); Serial.println(INA3221_shuntresistor);
   // Serial.print("address="); Serial.println(INA3221_i2caddr);

}

/**************************************************************************/
/*!
    @brief  Gets the raw bus voltage (16-bit signed integer, so +-32767)
*/
/**************************************************************************/
int16_t getBusVoltage_raw(int channel) {
  uint16_t value;
  wireReadRegister(INA3221_REG_BUSVOLTAGE_1+(channel -1) *2, &value);

//    Serial.print("BusVoltage_raw=");
//    Serial.println(value,HEX);

  // Shift to the right 3 to drop CNVR and OVF and multiply by LSB
  return (int16_t)(value );
}

/**************************************************************************/
/*!
    @brief  Gets the raw shunt voltage (16-bit signed integer, so +-32767)
*/
/**************************************************************************/
int16_t getShuntVoltage_raw(int channel) {
  uint16_t value;
  wireReadRegister(INA3221_REG_SHUNTVOLTAGE_1+(channel -1) *2, &value);

   // Serial.print("ShuntVoltage_raw=");
   // Serial.println(value,HEX);

  return (int16_t)value;
}



/**************************************************************************/
/*!
    @brief  Gets the shunt voltage in mV (so +-168.3mV)
*/
/**************************************************************************/
float getShuntVoltage_mV(int channel) {
  int16_t value;
  value = getShuntVoltage_raw(channel);
  return value * 0.005;
}

/**************************************************************************/
/*!
    @brief  Gets the shunt voltage in volts
*/
/**************************************************************************/
float getBusVoltage_V(int channel) {
  int16_t value = getBusVoltage_raw(channel);
  return value * 0.001;
}

/**************************************************************************/
/*!
    @brief  Gets the current value in mA, taking into account the
            config settings and current LSB
*/
/**************************************************************************/
float getCurrent_mA(int channel) {
    float valueDec = getShuntVoltage_mV(channel)/SHUNT_RESISTOR_VALUE;

  return valueDec;
}


/**************************************************************************/
/*!
    @brief  Gets the Manufacturers ID
*/
/**************************************************************************/
int getManufID()
{
  uint16_t value;
  wireReadRegister(0xFE, &value);
  return value;

}
