#ifndef STATUS_H_
#define STATUS_H_

#include <stdint.h>
#include <Arduino.h>

struct Status {
  int idx           = 1; //deviceId
  int rpm           = 600; //minimum value to keep DSC working
  int coolant_temp  = 90;
  double temp_sensor = 0;
  bool manual_rpm_control = false;
  long receivedCount = 0;
  long missedSend = 0;
  long bootedMillis = 0;
  long currentMillis = 0;
  int mosfetG5 = 0; //as switch
  int mosfetG6 = 0; //as switch
  /*
 configured in MQTT client / 128 bit value - percentage 
  1 / 0-1.2 - 0-1%   - stop 0%
  5 / 2-8(126-128) - 1-7%   - and 98-100% - emergency mode (cca 95% of speed) 1-6% - and 96-100%
  13 / 11-15 - 8-12%  - stop/error reset 7-12%
  17..63..108 / 17-108 - 13-85% - min to max speed
  120 / 114-124 - 89-97% - max speed 86-95% */
  /*
   *  name v1  v2  iv1 iv2 128v1 128v2 cv1 cv2 picked
      stop  0 1 100 99  128 126,72  128 127 127
      emergency mode 95%  2 7 98  93  125,44  119,04  125 120 123
      stop/error reset  8 12  92  88  117,76  112,64  117 113 115
      min-max speed (min..mid..max) 13  85  87  15  111,36  19,2  111 20  111..65..20
      max speed 89  97  11  3 14,08 3,84  14  4 10
      emergency mode 95%  98  100 2 0 2,56  0 2 1 1

   */
  int mosfetG7 = 100; 
  int mosfetG8 = 100;
  int digipot1 = 0;
  int digipot2 = 1;
};

extern Status status;

#endif /* STATUS_H_ */
