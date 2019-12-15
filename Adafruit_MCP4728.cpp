
/*!
 *  @file Adafruit_MCP4728.cpp
 *
 *  @mainpage Adafruit MCP4728 4-Channel 12-Bit I2C DAC library
 *
 *  @section intro_sec Introduction
 *
 * 	I2C Driver for the Adafruit MCP4728 4-Channel 12-Bit I2C DAC library
 *
 * 	This is a library for the Adafruit MCP4728 breakout:
 * 	https://www.adafruit.com/product/44XX
 *
 * 	Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing products from
 * 	Adafruit!
 *
 *  @section dependencies Dependencies
 *  This library depends on the Adafruit BusIO library
 *
 *
 *  @section author Author
 *
 *  Bryan Siepert for Adafruit Industries
 *
 * 	@section license License
 *
 * 	BSD (see license.txt)
 *
 * 	@section  HISTORY
 *
 *     v1.0 - First release
 */

#include "Adafruit_MCP4728.h"
#include "Arduino.h"
#include <Wire.h>

/*!
 *    @brief  Instantiates a new MCP4728 class
 */
Adafruit_MCP4728::Adafruit_MCP4728(void) {}

/*!
 *    @brief  Sets up the hardware and initializes I2C
 *    @param  i2c_address
 *            The I2C address to be used.
 *    @param  wire
 *            The Wire object to be used for I2C connections.
 *    @return True if initialization was successful, otherwise false.
 */
boolean Adafruit_MCP4728::begin(uint8_t i2c_address, TwoWire *wire) {
  i2c_dev = new Adafruit_I2CDevice(i2c_address, wire);

  if (!i2c_dev->begin()) {
    return false;
  }

    return true;
}

/**
 * @brief Sets the input register for a given channel to the specified settings
 *
 * @param channel The channel to update
 * @param new_value The new value to assign
 * @param new_vref Optional vref setting - Defaults to `MCP4728_VREF_VDD`
 * @param new_gain Optional gain setting - Defaults to `MCP4728_GAIN_1X`
 * @param new_pd_mode Optional power down mode setting - Defaults to
 * `MCP4728_PD_MOOE_NORMAL`
 * @param udac Optional UDAC setting - Defaults to `false`, latching immediately.
 *
 * Set to `true` to latch when the LDAC pin is pulled low
 */

void Adafruit_MCP4728::setChannelValue(
    MCP4728_channel_t channel, uint16_t new_value, MCP4728_vref_t new_vref,
    MCP4728_gain_t new_gain, MCP4728_pd_mode_t new_pd_mode, bool udac) {

  // build the setter header/ "address"
  // 0 1 0 0 0 DAC1 DAC0 UDAC[A]
  uint8_t sequential_write_cmd = MCP4728_MULTI_IR_CMD;
  sequential_write_cmd |= (channel << 1);
  sequential_write_cmd |= udac;

  // VREF PD1 PD0 Gx D11 D10 D9 D8 [A] D7 D6 D5 D4 D3 D2 D1 D0 [A]
  new_value |= (new_vref << 15);
  new_value |= (new_pd_mode << 13);
  new_value |= (new_gain << 12);

  Adafruit_BusIO_Register
      input_reg = // because it only writes to the input register
      Adafruit_BusIO_Register(i2c_dev, sequential_write_cmd, 2, MSBFIRST);

  input_reg.write(new_value);
}

/**
 * @brief Saves the DAC's input register settings to the internal EEPROM,
 * makeing them the default values when the ADC is powered on
 *
 * @return true if the write was successful
 * @return false if there was an error writing to the I2C bus
 */
bool Adafruit_MCP4728::saveToEEPROM(void) {
  uint8_t input_buffer[24];
  uint8_t output_buffer[9];

  i2c_dev->read(input_buffer, 24);

  // build header byte 0 1 0 1 0 DAC1 DAC0 UDAC [A]
  uint8_t eeprom_write_cmd = MCP4728_MULTI_EEPROM_CMD; // 0 1 0 1 0 xxx
  eeprom_write_cmd |=
      (MCP4728_CHANNEL_A << 1); // DAC1 DAC0, start at channel A obvs
  eeprom_write_cmd |= 0;        // UDAC ; yes, latch please
  // First byte is the write command+options
  output_buffer[0] = eeprom_write_cmd;

  // copy the incoming input register bytes to the outgoing buffer
  // Channel A
  output_buffer[1] = input_buffer[1];
  output_buffer[2] = input_buffer[2];
  // Channel B
  output_buffer[3] = input_buffer[7];
  output_buffer[4] = input_buffer[8];
  // Channel C
  output_buffer[5] = input_buffer[13];
  output_buffer[6] = input_buffer[14];
  // Channel D
  output_buffer[7] = input_buffer[19];
  output_buffer[8] = input_buffer[20];

  if (!i2c_dev->write(output_buffer, 9)){
    return false;
  }
  return true;
}
