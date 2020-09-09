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
  ledcAttachPin(pinsSettings.mosfetG5, 1); // assign a pins to a channel
  ledcAttachPin(pinsSettings.mosfetG6, 2); 
  ledcAttachPin(pinsSettings.mosfetG7, 3); 
  ledcAttachPin(pinsSettings.mosfetG8, 4); 

  // Initialize channels
  // channels 0-15, resolution 1-16 bits, freq limits depend on resolution
  // ledcSetup(uint8_t channel, uint32_t freq, uint8_t resolution_bits);
  ledcSetup(1, 1000, 7); // 1 kHz PWM, 7-bit resolution (0-127) - for switch
  ledcSetup(2, 1000, 7); 
  ledcSetup(3, 200, 7); // 200 Hz PWM, 7-bit resolution (0-127) - for pump
  ledcSetup(4, 200, 7); 
}

void PWMMosfet::handle() {
  // set switch state - check if we need to invert value like for pumps
  if(status.mosfetG5>0) {
    ledcWrite(1, status.mosfetG5); 
  } else {
    ledcWrite(1, 127);
  }
  if(status.mosfetG6>0) {
    ledcWrite(2, status.mosfetG6); 
  } else {
    ledcWrite(2, 127);
  }

  ledcWrite(3, status.mosfetG7); // set the speed of pump - write inverted value
  ledcWrite(4, status.mosfetG8); 
}
