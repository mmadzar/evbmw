#include "appconfig.h"
#include "MqttPubSub.h"
#include <ArduinoJson.h>
#include "status.h"

StaticJsonDocument<512> doc;
MqttPubSub globalMqttPubSub;
long lastReconnectAttempt=0;

MqttPubSub::MqttPubSub() {
}

void MqttPubSub::setup() {
  client.setServer(mqttSettings.server, mqttSettings.port);
  client.setClient(espClient);
  client.setCallback(callback);
  reconnect();
}

bool MqttPubSub::reconnect() {
  // Attempt to connect
  if (connect(wifiSettings.hostname, mqttSettings.username, mqttSettings.password)) {
    Serial.println("Connected to MQTT.");
    // Subscribe
    client.subscribe("vehicle/in");
    digitalWrite(pinsSettings.led, HIGH);
    delay(100);
    digitalWrite(pinsSettings.led, LOW);
    delay(100);
    digitalWrite(pinsSettings.led, HIGH);
    delay(100);
    digitalWrite(pinsSettings.led, LOW);
    publishStatus(false);
  }
  return client.connected();
}

bool MqttPubSub::connect(const char* id, const char* username, const char* password) {
  if (username == "") {
    return client.connect(id);
  } else {
    return client.connect(id, username, password);
  }
}

void MqttPubSub::publishStatus(bool waitForInterval) {
  if (!waitForInterval || ((millis() - lastMillis) > intervals.statusPublish)) {
    lastMillis = millis();
    doc.clear();
    doc["rpm"] = status.rpm;
    doc["manual_rpm_control"] = status.manual_rpm_control;
    doc["coolant_temp"] = status.coolant_temp;
    doc["temp_sensor"] = status.temp_sensor;
    doc["receivedCount"] = status.receivedCount;
    doc["missedSend"] = status.missedSend;
    doc["uptime"] = (status.currentMillis - status.bootedMillis) / 1000;
    doc["currentMillis"] = status.currentMillis;
    char buffer[512];
    serializeJson(doc, buffer);
    client.publish("vehicle/out", buffer);
    status.missedSend = 0;
  } else {
    status.missedSend++;
  }
}

boolean MqttPubSub::publishCanMessage(char const* message) {
  return client.publish("vehicle/can", message);
}


void MqttPubSub::callback(char* topic, byte* message, unsigned int length) {
  //executed only on subscribed topics
  deserializeJson(doc, message);
  if (doc["idx"] == 1) {
    bool statusChanged = false;
    if (doc.containsKey("rpm")) {
      status.rpm = doc["rpm"];
      status.manual_rpm_control = true;
      statusChanged = true;
    }
    if (doc.containsKey("coolant_temp")) {
      status.coolant_temp = doc["coolant_temp"];
      statusChanged = true;
    }
    if (statusChanged) {
      globalMqttPubSub.publishStatus(false);
    }
  }
}

void MqttPubSub::handle() {
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 3000) {
      digitalWrite(pinsSettings.led, HIGH);
      delay(200);
      digitalWrite(pinsSettings.led, LOW);
      Serial.println("Reconnecting to MQTT...");
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    // Client connected
    client.loop();
  }
}
