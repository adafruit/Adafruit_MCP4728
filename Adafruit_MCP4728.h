/*!
 *  @file Adafruit_MCP4728.h
 *
 * 	I2C Driver for the Adafruit MCP4728 4-Channel 12-Bit I2C DAC library
 *
 * 	This is a library for the Adafruit MCP4728 breakout:
 * 	https://www.adafruit.com/products/44XX
 *
 * 	Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing products from
 * 	Adafruit!
 *
 *
 *	BSD license (see license.txt)
 */

#ifndef _ADAFRUIT_MCP4728_H
#define _ADAFRUIT_MCP4728_H

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_BusIO_Register.h>


#define MCP4728_I2CADDR_DEFAULT 0x60 ///< MCP4728 default i2c address
	// '0b 010 00 000'
#define MCP4728_CH_A_MULTI_IB 0x40 ///< A fake register for channel A
#define MCP4728_CH_B_MULTI_IB 0x42 ///< A fake register for channel B
#define MCP4728_CH_C_MULTI_IB 0x44 ///< A fake register for channel C
#define MCP4728_CH_D_MULTI_IB 0x46 ///< A fake register for channel D 

// '0b 010 11 000'
#define MCP4728_CH_A_SINGLE_EEPROM 0x58 ///< A fake register for channel A
#define MCP4728_CH_B_SINGLE_EEPROM 0x5A ///< A fake register for channel B
#define MCP4728_CH_C_SINGLE_EEPROM 0x5C ///< A fake register for channel C
#define MCP4728_CH_D_SINGLE_EEPROM 0x5E ///< A fake register for channel D

// '0b 010 10 000'
#define MCP4728_CH_A_MULTI_EEPROM 0x50 ///< A fake register for channel A

/**
 * @brief Example enum values
 *
 * Allowed values for `setChannelGain`.
 */
typedef enum gain {
  MCP4728_GAIN_1X,
  MCP4728_GAIN_2X,
} MCP4728_gain_t;


/**
 * @brief Example enum values
 *
 * Allowed values for `setChannelGain`.
 */
typedef enum channel {
  MCP4728_CHANNEL_D,
  MCP4728_CHANNEL_C,
  MCP4728_CHANNEL_B,
  MCP4728_CHANNEL_A,
} MCP4728_channel_t;

/*!
 *    @brief  Class that stores state and functions for interacting with
 *            the MCP4728 I2C Digital Potentiometer
 */
class Adafruit_MCP4728 {
public:

  Adafruit_MCP4728();
  bool begin(uint8_t i2c_address=MCP4728_I2CADDR_DEFAULT, TwoWire *wire = &Wire);

  MCP4728_gain_t getChannelGain(MCP4728_channel_t channel);
  void setChannelGain(MCP4728_channel_t channel, MCP4728_gain_t new_gain);
  void setChannelValue(MCP4728_channel_t channel, uint16_t new_value);

private:
  bool _init(void);

  Adafruit_I2CDevice *i2c_dev;
};

#endif
