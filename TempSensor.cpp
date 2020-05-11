#include <Arduino.h>
#include "max6675.h"
#include "appconfig.h"
#include "TempSensor.h"

MAX6675 thermocouple(pinsSettings.temp_clk, pinsSettings.temp_cs, pinsSettings.temp_do);

TempSensor::TempSensor()
{
}
  
//returns 1000 if no change
double TempSensor::handleTempSensor() {
  tempValue = thermocouple.readCelsius();
  lastValue = tempValue;
  return tempValue;
  /* if (tempValue != lastValue) {
    lastValue = tempValue;
    return lastValue;
  }
  else {
    return 1000;
  }*/
}
