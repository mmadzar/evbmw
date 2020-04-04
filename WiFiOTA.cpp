#include "appconfig.h"
#include "status.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include "WiFiOTA.h"

long lastOTAmillis = 0;
WiFiSettings wifiSettings;
PinsSettings pinsSettings;
WiFiMulti WiFiMulti;

WiFiOTA::WiFiOTA()
{
}

void WiFiOTA::setupWiFi() {
  delay(10);
  Serial.print("Conecting to WiFi... ");
  // WiFi setup
  WiFiMulti.addAP(wifiSettings.ssid, wifiSettings.password);
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
}

void WiFiOTA::setupOTA() {
  // OTA configuration
  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);
  ArduinoOTA.setHostname(wifiSettings.hostname);
  ArduinoOTA.setPassword(wifiSettings.passwordOTA);
  ArduinoOTA
  .onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  })
  .onEnd([]() {
    Serial.println("\nEnd. Restarting...");
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  })
  .onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //blink for success
  digitalWrite(pinsSettings.led, HIGH);
  delay(100);
  digitalWrite(pinsSettings.led, LOW);
}

void WiFiOTA::handleOTA() {
  //check for OTA every half second
  if ((status.currentMillis - lastOTAmillis) > 500) {
    ArduinoOTA.handle();
    lastOTAmillis = status.currentMillis;
  }
}
