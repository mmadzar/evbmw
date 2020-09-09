#include <Arduino.h>
#include "DigiPot.h"
#include "status.h"

const int slaveSelectPin = pinsSettings.digipotCS;
const int shutdownPin = pinsSettings.digipotSHDN;
const int wiper0writeAddr = 0;  //B00000000;
const int wiper1writeAddr = 16; //B00010000;
const int tconwriteAddr = 64;   //B01000000;
/* const int tcon_0off_1on = 240;  //B11110000;
  const int tcon_0on_1off = 15;   //B00001111;
  const int tcon_0off_1off = 0;   //B00000000;
  const int tcon_0on_1on = 255;   //B11111111; */

int scurt = 500;
int lung = 3000;
int lastWrite = 0;

DigiPot::DigiPot() {
  init();
}

void DigiPot::init() {

}

void DigiPot::setup() {
  pinMode (slaveSelectPin, OUTPUT);
  pinMode (shutdownPin, OUTPUT);
  digitalWrite(shutdownPin, HIGH);
  SPI.begin();

  digitalWrite(pinsSettings.led, 128);
  delay(500);
  digitalWrite(pinsSettings.led, LOW);
}

void DigiPot::handle() {
  //done on setup digitalWrite(shutdownPin, HIGH); //Turn off shutdown
  if ((lastWrite - status.currentMillis) < -1000) { //execute every 1 sec
    lastWrite = status.currentMillis;
    digitalPotWrite(wiper0writeAddr, status.digipot1);
    digitalPotWrite(wiper1writeAddr, status.digipot2);
  }
}

// This function takes care of sending SPI data to the pot.
void DigiPot::digitalPotWrite(int address, int value) {
  // take the SS pin low to select the chip:
  digitalWrite(slaveSelectPin, LOW);
  //  send in the address and value via SPI:
  SPI.transfer(address);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin, HIGH);
}
