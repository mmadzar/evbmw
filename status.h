#ifndef STATUS_H_
#define STATUS_H_

#include <stdint.h>

struct Status {
  int idx           = 1; //deviceId
  int rpm           = 600; //minimum value to keep DSC working
  int coolant_temp  = 90;
  bool manual_rpm_control = false;
  long receivedCount = 0;
  long missedSend = 0;
  long bootedMillis = 0;
  long currentMillis = 0;
};

extern Status status;

#endif /* STATUS_H_ */
