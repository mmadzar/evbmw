#include <Arduino.h>
#include "appconfig.h"
#include <ArduinoOTA.h>
#include "WiFiOTA.h"
#include "MqttPubSub.h"
#include "CanBus.h"
#include "DigiPot.h"
#include "PWMMosfet.h"
#include <string.h>
//#include "ThrottleSensor.h"
//#include "TempSensor.h"

WiFiOTA wota;
PinsSettings pins;
Intervals intervals;
CanBus can;
MqttPubSub mqtt;
DigiPot digiPot;
PWMMosfet pwmMosfet;
//ThrottleSensor tps;
//TempSensor temps;

long lastCAN2mqtt = 0;
int tempPotValue = 800;
bool firstRun = true;
int lastDigipot1 = 11;
int lastDigipot2 = 11;

//long lastTempRead = 0;

void setup() {
  status.bootedMillis = millis();
  Serial.begin(115200);
  pinMode(pins.led, OUTPUT);
  wota.setupWiFi();
  wota.setupOTA();
  digiPot.setup();
  mqtt.setup();
  can.setup();
  pwmMosfet.setup();
  status.digipot1 = lastDigipot1;
  status.digipot2 = lastDigipot2;
}

void loop() {
  status.currentMillis = millis();
  wota.handleOTA();

  //Send can messages
  can.sendMessageSet();
  pwmMosfet.handle();
 
  //watch for messages
  can.handle();
  if (!can.CAN0messageEmpty) {
    publishCANmessage(can.CAN0message);
  }

  //Read analog sensor value
  //tempPotValue = tps.handleThrottleSensor();

  //Receive MQTT messages
  mqtt.handle();
  digiPot.handle();

  mqtt.publishStatus(true);
}

void publishCANmessage(char* msg) {
  if ((status.currentMillis - lastCAN2mqtt) > intervals.Can2Mqtt) {
    lastCAN2mqtt = status.currentMillis;
    mqtt.publishCanMessage(msg);
    can.CAN0messageEmpty = true;
  }
}
