#include <Arduino.h>
#include "appconfig.h"
#include <ArduinoOTA.h>
#include "WiFiOTA.h"
#include "MqttPubSub.h"
#include "CanBus.h"
#include <string.h>
#include "ThrottleSensor.h"
#include "TempSensor.h"

WiFiOTA wota;
PinsSettings pins;
Intervals intervals;
CanBus can;
MqttPubSub mqtt;
ThrottleSensor tps;
TempSensor temps;

long lastCAN2mqtt = 0;
int tempPotValue = 0;
bool firstRun = true;
long lastTempRead = 0;

void setup() {
  status.bootedMillis = millis();
  Serial.begin(115200);
  pinMode(pins.led, OUTPUT);
  wota.setupWiFi();
  wota.setupOTA();
  mqtt.setup();
  can.setup();
}

void loop() {
  status.currentMillis = millis();
  wota.handleOTA();

  //Send can messages
  can.sendMessageSet();

  //watch for messages
  can.handle();
  if (!can.CAN0messageEmpty) {
    publishCANmessage(can.CAN0message);
    can.CAN0messageEmpty = true;
  }
  /* if (!can.CAN1messageEmpty) {
    publishCANmessage(can.CAN1message);
    can.CAN1messageEmpty = true;
  } */

  //Read analog sensor value
  tempPotValue = tps.handleThrottleSensor();
  if (tempPotValue != -1 && (!status.manual_rpm_control || tempPotValue > 3000)) {
    status.manual_rpm_control = false;
    if (status.rpm != tempPotValue) {
      status.rpm = tempPotValue;
      mqtt.publishStatus(false);
    }
  }

  if(status.currentMillis-lastTempRead>500) {
    status.temp_sensor = temps.handleTempSensor();
    lastTempRead=status.currentMillis;
  }

  mqtt.publishStatus(true);

  //Receive MQTT messages
  mqtt.handle();
}

void publishCANmessage(char* msg) {
  if ((status.currentMillis - lastCAN2mqtt) > intervals.Can2Mqtt) { //set this at 0 to forward all CAN messages to MQTT
    lastCAN2mqtt = status.currentMillis;
    mqtt.publishCanMessage(msg);
  }
}
