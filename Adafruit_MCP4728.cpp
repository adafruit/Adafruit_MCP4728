
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

#include "Arduino.h"
#include <Wire.h>
#include "Adafruit_MCP4728.h"

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

  // read in data from eeproms?
  // i2c_device->read(uint8_t *buffer, size_t len, bool stop) {
    /*
    set the value for each channel
    set the Vref for a channel
    set the gain
    set the power down state
    */
  return true;
}

/**
 * @brief Gets the gain setting for a given channel
 *
 * @param channel The chanel to return the gain setting for
 * @return MCP4728_gain_t The given channel's gain setting
 */
MCP4728_gain_t Adafruit_MCP4728::getChannelGain(MCP4728_channel_t channel){
    // Adafruit_BusIO_Register example_register =
    //   // Adafruit_I2CDevice pointer, address, number of bytes
    //   Adafruit_BusIO_Register(i2c_dev, MCP4728_EXAMPLE_REG, 2);

    // Adafruit_BusIO_RegisterBits example_bitfield =
    //   // register pointer, number of bits, shift
    //   Adafruit_BusIO_RegisterBits(&example_register, 3, 1);
    // write(example_value);
    // return (MCP4728_example_t)example_bitfield.read();
    return MCP4728_GAIN_2X;
}

/**
 * @brief Sets the gain for a given channel
 *
 * @param channel The channel to set the gain for
 * @param new_gain The gain setting to be applied to the given channel.
 *
 * **Note that setting the gain for a channel will only have an effect if
 * the channel's Vref is set to `MCP_VREF_INTERNAL`**
 *
 *
 */
void Adafruit_MCP4728::setChannelGain(MCP4728_channel_t channel, MCP4728_gain_t new_gain){


    // Adafruit_BusIO_RegisterBits example_bitfield =
    //   // register pointer, number of bits, shift
    //   Adafruit_BusIO_RegisterBits(&example_register, 3, 1);
    // example_bitfield.write(example_value);
}


/**
 * @brief Set the value for the given channel
 * 
 * @param channel the `MCP4728_channel_t` channel to update
 * @param new_value The new value to be set
 */
void Adafruit_MCP4728::setChannelValue(MCP4728_channel_t channel, uint16_t new_value){

  uint8_t channel_address = 0;
  switch(channel){
    case MCP4728_CHANNEL_A: channel_address = MCP4728_CH_A_MULTI_IB; break;
    case MCP4728_CHANNEL_B: channel_address = MCP4728_CH_B_MULTI_IB; break;
    case MCP4728_CHANNEL_C: channel_address = MCP4728_CH_C_MULTI_IB; break;
    case MCP4728_CHANNEL_D: channel_address = MCP4728_CH_D_MULTI_IB; break;
  }

  // [S] 1 1 0 0 A2 A1 A0 0 [A] //auto by i2c_device
  // 0 1 0 0 0 DAC1 DAC0 UDAC[A] //auto by register obj
  // VREF PD1 PD0 Gx D11 D10 D9 D8 [A]
  // D7 D6 D5 D4 D3 D2 D1 D0 [A]

  Adafruit_BusIO_Register input_reg =
    // Adafruit_I2CDevice pointer, address, number of bytes
    Adafruit_BusIO_Register(i2c_dev, channel_address, 2, MSBFIRST);

  input_reg.write(new_value);
}