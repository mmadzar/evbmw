#ifndef THROTTLESENSOR_H_
#define THROTTLESENSOR_H_

class ThrottleSensor {
  private:
    int potValues[1000];
    int potValuesIndex = 0;
    int sumPot = 0;

    int potValue = 0;
    int tempPotValue = 0; //used to store value read from device
    int rpmValue = 0;
    void init();
    int getPotValue(int readValue);
  public:
    ThrottleSensor();
    int handleThrottleSensor();
};

#endif /* THROTTLESENSOR_H_ */
