#ifndef MQTTPUBSUB_H_
#define MQTTPUBSUB_H_

#include <WiFi.h>
#include <PubSubClient.h>
#include "appconfig.h"
#include "status.h"

class MqttPubSub {
  private:
    WiFiClient espClient;
    PubSubClient client;
    MqttSettings mqttSettings;
    WiFiSettings wifiSettings;
    bool connect(const char* id, const char* username, const char* password);
    bool reconnect();
    static void callback(char* topic, byte* message, unsigned int length);
  public:
    int lastMillis = 0;
    MqttPubSub();
    void publishStatus(bool waitForInterval);
    bool publishCanMessage(const char* message);
    void setup();
    void handle();
};

extern MqttPubSub globalMqttPubSub;

#endif /* MQTTPUBSUB_H_ */
