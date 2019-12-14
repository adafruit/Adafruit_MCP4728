
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

  return _init();
}

boolean Adafruit_MCP4728::_init(void) {

  uint8_t buffer[24];
  // bool Adafruit_I2CDevice::read(uint8_t *buffer, size_t len, bool stop)
  i2c_dev->read(buffer, 24);

  printChannel(MCP4728_CHANNEL_A, buffer);
  printChannel(MCP4728_CHANNEL_B, buffer);
  printChannel(MCP4728_CHANNEL_C, buffer);
  printChannel(MCP4728_CHANNEL_D, buffer);

  // }
  return true;
}

/**
 * @brief print the contents of the ADC's output buffer for a given channel
 *
 * @param channel The channel to print the values for
 * @param buffer A buffer holding the  DAC's current settings
 */
void Adafruit_MCP4728::printChannel(MCP4728_channel_t channel,
                                    uint8_t *buffer) {

  uint8_t buffer_offset = 0;
  switch (channel) {
  case MCP4728_CHANNEL_A:
    buffer_offset = 0;
    break;
  case MCP4728_CHANNEL_B:
    buffer_offset = 6;
    break;
  case MCP4728_CHANNEL_C:
    buffer_offset = 12;
    break;
  case MCP4728_CHANNEL_D:
    buffer_offset = 18;
    break;
  }
  uint8_t in_header = buffer[0 + buffer_offset];
  uint16_t in_value =
      buffer[1 + buffer_offset] << 8 | buffer[2 + buffer_offset];

  bool a_vref = (in_value & 1 << 15) >> 15;
  MCP4728_pd_mode_t a_pd = (in_value & 0b11 << 13) >> 13;
  MCP4728_gain_t a_gain = (in_value & 1 << 12) >> 12;
  uint16_t value = in_value & 4095;
  Serial.println("******************* BUFFERAZ ****************");
  Serial.print("A->INB: ");
  Serial.print(in_header, BIN);
  Serial.print(" ");
  Serial.println(in_value, BIN);
  Serial.println("*********************************************");

  Serial.print("Value is ");
  Serial.println(value);
  Serial.print("Vref is ");
  switch (a_vref) {
  case MCP4728_VREF_VDD:
    Serial.println("VDD");
    break;
  case MCP4728_VREF_INTERNAL:
    Serial.println("Internal 2.048V Reference");
    break;
  }
  Serial.print("Gain is ");
  switch (a_gain) {
  case MCP4728_GAIN_1X:
    Serial.println("1X");
    break;
  case MCP4728_GAIN_2X:
    Serial.println("2X");
    break;
  }
  Serial.print("Power Down state is ");
  switch (a_pd) {
  case MCP4728_PD_MOOE_NORMAL:
    Serial.println("Normal");
    break;
  case MCP4728_PD_MOOE_GND_1K:
    Serial.println("Grounded w/ 1K");
    break;
  case MCP4728_PD_MOOE_GND_100K:
    Serial.println("Grounded w/ 100K");
    break;
  case MCP4728_PD_MOOE_GND_500K:
    Serial.println("Grounded w/ 500K");
    break;
  }
  Serial.println("*********************************************");
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
 * @param udac Optional UDAC setting - Defaults to `false`, latching (nearly).
 * Set to `true` to latch when the UDAC pin is pulled low
 */

void Adafruit_MCP4728::setChannelValue(
    MCP4728_channel_t channel, uint16_t new_value, MCP4728_vref_t new_vref,
    MCP4728_gain_t new_gain, MCP4728_pd_mode_t new_pd_mode, bool udac) {

  // build the setter header/ "address"
  // 0 1 0 0 0 DAC1 DAC0 UDAC[A]
  uint8_t sequential_write_cmd = 0b01000000;
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
 * makeing them defaults
 *
 */
void Adafruit_MCP4728::saveToEEPROM(void) {
  uint8_t buffer[24];
  i2c_dev->read(buffer, 24);

  printChannel(MCP4728_CHANNEL_A, buffer);
  printChannel(MCP4728_CHANNEL_B, buffer);
  printChannel(MCP4728_CHANNEL_C, buffer);
  printChannel(MCP4728_CHANNEL_D, buffer);

  // pretty sure we can just send back out the bytes that came in from the input
  // register
}
