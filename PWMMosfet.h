#ifndef PWMMOSFET_H_
#define PWMMOSFET_H_

#include <Arduino.h>


class PWMMosfet {
  private:
    PinsSettings pinsSettings;
    void init();
  public:
    PWMMosfet();
    void setup();
    void handle();
};

#endif /* PWMMOSFET_H_ */