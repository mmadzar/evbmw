#ifndef CANBUS_H_
#define CANBUS_H_

#include <Arduino.h>
#include <esp32_can.h>
#include <CAN_config.h>
#include "appconfig.h"
#include "MqttPubSub.h"

class CanBus {
  private:
    PinsSettings pinsSettings;
    void init();
    CAN_device_t CAN_cfg;          // CAN Config
    long previousMillis = 0;       // will store last time a CAN Message was send
    const int interval = 1000;     // interval at which send CAN Messages (milliseconds)
    const int rx_queue_size = 128; // Receive Queue size was 10
    void printFrame(CAN_FRAME &message, int canNo);
    int byteArray[2];
  public:
    CanBus();
    byte CAN0message[2048];
    //char CAN1message[512];
    bool CAN0messageEmpty=true;
    //bool CAN1messageEmpty=true;
    void setup();
    void handle();
    void sendMessageSet();
};

#endif /* CANBUS_H_ */
