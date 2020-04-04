#ifndef APPCONFIG_H_
#define APPCONFIG_H_

#include <stdint.h>
#include <driver/gpio.h>

struct WiFiSettings {
  const char* ssid        = "JM2";
  const char* password    = "wifipassword";
  const char* passwordOTA = "otapassword";
  const char* hostname    = "ESP32_01";
};

struct PinsSettings {
  const gpio_num_t pot      = (gpio_num_t)36; //throttle pedal potentiometer
  const int potValueChangeIgnore =        25; //ignore all difference when sending to mqtt below this threshold, this should keep throttle sensor value steady at no pressure
  const gpio_num_t can0_rx  = (gpio_num_t)16; //can0 transciever rx line
  const gpio_num_t can0_tx  = (gpio_num_t)17; //can0 transciever tx line
  const gpio_num_t can1_cs  = (gpio_num_t)5;  //can1 CS line
  const gpio_num_t can1_int = (gpio_num_t)27; //can1 INT line //keep this pin disconnected. if connected WiFi is not working.
  const gpio_num_t led      = (gpio_num_t)2;  //status led
};

struct MqttSettings {
  const char*     server   = "192.168.45.109";
  const uint16_t  port     = 1883;
  const char*     username = "";
  const char*     password = "";

};

struct Intervals {
  const int statusPublish = 1000; //interval at which status is published
  const int Can2Mqtt = 1000;      //send CAN messages to MQTT every n secconds. Set to 0 forward all
  const int CANsend = 10;         // interval at which send CAN Messages (milliseconds)
};

extern WiFiSettings wifiSettings;
extern PinsSettings pinsSettings;
extern MqttSettings mqttSettings;
extern Intervals intervals;

#endif /* APPCONFIG_H_ */
