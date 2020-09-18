#include "appconfig.h"
#include "status.h"
#include <Arduino.h>
#include "PWMMosfet.h"

PWMMosfet::PWMMosfet(){
    init();
}

void PWMMosfet::init() {

}

void PWMMosfet::setup() {
  pinMode(pinsSettings.mosfetG5, OUTPUT);
  pinMode(pinsSettings.mosfetG6, OUTPUT);
  ledcAttachPin(pinsSettings.mosfetG7, 1); // assign a pins to a channel
  ledcAttachPin(pinsSettings.mosfetG8, 2); 

  // Initialize channels
  // channels 0-15, resolution 1-16 bits, freq limits depend on resolution
  // ledcSetup(uint8_t channel, uint32_t freq, uint8_t resolution_bits);
  ledcSetup(1, 200, 7); // 200 Hz PWM, 7-bit resolution (0-127) - for pump
  ledcSetup(2, 200, 7); 
}

void PWMMosfet::handle() {
  // set switch state - check if we need to invert value like for pumps
  digitalWrite(pinsSettings.mosfetG5, status.mosfetG5);
  digitalWrite(pinsSettings.mosfetG6, status.mosfetG6);
  
  ledcWrite(1, status.mosfetG7); // set the speed of pump - write inverted value
  ledcWrite(2, status.mosfetG8); 
}
