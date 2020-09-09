#ifndef DIGIPOT_H_
#define DIGIPOT_H_

#include "appconfig.h"
#include <SPI.h>

/* tehniq3
  /
  MCP4261
  1
  00
  Code
  Issues
  Pull requests
  Actions
  Projects
  Wiki
  Security
  Insights
  MCP4261/MCP4261_mega_lcd1602_i2c_test.ino
  @tehniq3
  tehniq3 Update MCP4261_mega_lcd1602_i2c_test.ino
  Latest commit 7224e33 on Jul 7, 2016
  History
  1 contributor
  195 lines (159 sloc)  5.35 KB
*/
/*
  Matthew McMillan
  @matthewmcmillan
  http://matthewcmcmillan.blogspot.com
  Created 12 Mar 2014
  Digital Pot Control (MCP4251)

  This example controls a Microchip MCP4251 digital potentiometer.
  The MCP4251 has 2 potentiometer channels. Each channel's pins are labeled:
      A - connect this to voltage
      W - this is the pot's wiper, which changes when you set it
      B - connect this to ground.
  The MCP4251 also has Terminal Control Registers (TCON) which allow you to
  individually connect and disconnect the A,W,B terminals which can be useful
  for reducing power usage or motor controls.

  A value of 0 is no resistance
  A value of 128 is approximately half ohms
  A value of 255 is maxim of resistance

  The MCP4251 is SPI-compatible. To command it you send two bytes,
  one with the memory address and one with the value to set at that address.
  The MCP4251 has few different memory addresses for wipers and tcon (Terminal Control)
      Wiper 0 write
      Wiper 0 read
      Wiper 1 write
      Wiper 1 read
      TCON write
      TCON read

  The circuit:
       All A pins of MCP4251 connected to +5V
       All B pins of MCP4251 connected to ground
       W0 pin to A0
       W1 pin to A1
       VSS - to GND
       VDD - to +5v
       + SHDN - to digital pin 7 (and a 4.7k pull down resistor)
       + CS - to digital pin 10  (SS pin) or 53 at Mega
       SDI - to digital pin 11 (MOSI pin) or 51 at Mega
       SDO - to digital pin 12 (MISO pin) or 50 at mega
       CLK - to digital pin 13 (SCK pin) or 52 at Mega

  created 12 Mar 2014

  Thanks to Heather Dewey-Hagborg and Tom Igoe for their original tutorials
  adapted by Nicu FLORICA (aka niq_ro) - http://www.tehnic.go.ro
  & http://www.arduinotehniq.com/
*/



class DigiPot {
  private:
    PinsSettings pinsSettings;
    void afisare0(byte lev);
    void afisare1(byte lev);
    void digitalPotWrite(int address, int value);
  public:
    DigiPot();
    void init();
    void setup();
    void handle();
};

#endif 
