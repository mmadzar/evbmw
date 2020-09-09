#include <Arduino.h>
#include "appconfig.h"
#include "ThrottleSensor.h"

ThrottleSensor::ThrottleSensor()
{
  init();
}

void ThrottleSensor::init() {
  for (int i = 0; i < 50; i++)
  {
    potValues[i] = 0;
  }
}

//Adds value into array of 50 and returns average
int ThrottleSensor::getPotValue(int readValue) {
  potValuesIndex++;
  if (potValuesIndex >= 50) {
    potValuesIndex = 0;
  }
  sumPot = sumPot - potValues[potValuesIndex] + readValue;
  potValues[potValuesIndex] = readValue;
  return sumPot / 50;
}

//returns -1 if no change - handles minimum value and conversion to proper range
int ThrottleSensor::handleThrottleSensor() {
  return 800; // analogRead(pinsSettings.pot);
  /*
  //Handle Potentiometer
  tempPotValue = getPotValue(analogRead(pinsSettings.pot));
  //ignore small changes
  if (tempPotValue > 0 && abs(tempPotValue - potValue) > pinsSettings.potValueChangeIgnore) {
    potValue = tempPotValue;

    //return value between 600 and 7000.
    rpmValue = potValue * 3.5;
    if (rpmValue < 600) {
      rpmValue = 600;
    }
    return rpmValue;
  }
  else {
    return -1;
  }*/
}
