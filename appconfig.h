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
  /* const gpio_num_t can1_cs  = (gpio_num_t)5;  //can1 CS line
  const gpio_num_t can1_int = (gpio_num_t)27; //can1 INT line //keep this pin disconnected. if connected WiFi is not working. */
  const gpio_num_t led      = (gpio_num_t)2;  //status led
  /* int8_t temp_clk  = (gpio_num_t)5; //K-Type termo sensor MAX6675
  int8_t temp_do   = (gpio_num_t)19;//K-Type termo sensor MAX6675
  int8_t temp_cs   = (gpio_num_t)23;//K-Type termo sensor MAX6675 */
  const gpio_num_t digipotCS  = (gpio_num_t)23; //digital potentiometer CS line
  const gpio_num_t digipotSCK =  (gpio_num_t)5; //digital potentiometer SCK line
  const gpio_num_t digipotSDO = (gpio_num_t)19; //digital potentiometer SDO line
  const gpio_num_t digipotSDI = (gpio_num_t)18; //digital potentiometer SDI line
  const gpio_num_t digipotSHDN = (gpio_num_t)4; //digital potentiometer SHDN line
  const gpio_num_t mosfetG4 = (gpio_num_t)32; //mosfet gate 1 - not yet hw supported
  const gpio_num_t mosfetG4 = (gpio_num_t)25; //mosfet gate 2 - not yet hw supported
  const gpio_num_t mosfetG4 = (gpio_num_t)26; //mosfet gate 3 - not yet hw supported
  const gpio_num_t mosfetG4 = (gpio_num_t)27; //mosfet gate 4 - not yet hw supported
  const gpio_num_t mosfetG5 = (gpio_num_t)22; //mosfet gate 5
  const gpio_num_t mosfetG6 = (gpio_num_t)21; //mosfet gate 6
  const gpio_num_t mosfetG7 = (gpio_num_t)12; //mosfet gate 7
  const gpio_num_t mosfetG8 = (gpio_num_t)13; //mosfet gate 8
  
};

struct MqttSettings {
  const char*     server   = "192.168.1.1";
  const uint16_t  port     = 1883;
  const char*     username = "";
  const char*     password = "";

};

struct Intervals {
  const int statusPublish = 1000; //interval at which status is published to MQTT
  const int Can2Mqtt = 1000;      //send CAN messages to MQTT every n secconds. Accumulate messages until. Set this to 0 for forwarding all CAN messages to MQTT as they are received.
  const int CANsend = 10;         //interval at which to send CAN Messages to car bus network (milliseconds)
};

extern WiFiSettings wifiSettings;
extern PinsSettings pinsSettings;
extern MqttSettings mqttSettings;
extern Intervals intervals;

#endif /* APPCONFIG_H_ */
