#ifndef TempSensor_H_
#define TempSensor_H_

class TempSensor {
  private:
    double tempValue = 0;
  public:
    double lastValue = 0;
    TempSensor();
    double handleTempSensor();
};

#endif /* TempSensor_H_ */
