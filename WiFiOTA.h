#ifndef WIFIOTA_H_
#define WIFIOTA_H_

#include "appconfig.h"
#include <WiFi.h>
#include <ArduinoOTA.h>

class WiFiOTA {
  private:
  public:
    WiFiOTA();
    void setupWiFi();
    void setupOTA();
    void handleOTA();
};

#endif /* WIFIOTA_H_ */
