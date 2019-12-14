// Basic demo for configuring the MCP4728 4-Channel 12-bit I2C DAC
#include <Adafruit_MCP4728.h>
#include <Wire.h>

Adafruit_MCP4728 mcp;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MCP4728 test!");

  // Try to initialize!
  if (!mcp.begin()) {
    Serial.println("Failed to find MCP4728 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MCP4728 Found!");
  /*
   * @param channel The channel to update
   * @param new_value The new value to assign
   * @param new_gain Optional gain setting - Defaults to `MCP4728_GAIN_1X`
   * @param new_vref Optional vref setting - Defaults to `MCP4728_VREF_VDD`
   * @param new_pd_mode Optional power down mode setting - Defaults to
   * `MCP4728_PD_MOOE_NORMAL`
   * @param udac Optional UDAC setting - Defaults to `false`, latching (nearly).
   * Set to `true` to latch when the UDAC pin is pulled low
   *
   */
  mcp.setChannelValue(MCP4728_CHANNEL_A, 0);
  mcp.setChannelValue(MCP4728_CHANNEL_B, 1024, MCP4728_VREF_INTERNAL,
                      MCP4728_GAIN_2X);
  mcp.setChannelValue(MCP4728_CHANNEL_C, 2048, MCP4728_VREF_INTERNAL,
                      MCP4728_GAIN_2X);
  mcp.setChannelValue(MCP4728_CHANNEL_D, 0);
  delay(1000);
  mcp.saveToEEPROM();
}

void loop() { delay(1000); }