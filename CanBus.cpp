#include "appconfig.h"
#include <Arduino.h>
#include <esp32_can.h>
#include <CAN_config.h>
#include <ArduinoJson.h>
#include "status.h"
#include "CanBus.h"

StaticJsonDocument<512> docJ;
CAN_FRAME frames[3];
long displayValue = 0;
Status status;

CanBus::CanBus() {
  init();
}

void CanBus::init() {
  CAN0.setCANPins(pinsSettings.can0_rx, pinsSettings.can0_tx);
  CAN1.setCSPin(pinsSettings.can1_cs);
  CAN1.setINTPin(pinsSettings.can1_int);
}

void CanBus::setup() {
  // Init CAN Modules
  if (CAN0.begin(500000)) {
    Serial.println("Builtin CAN Init OK.");
  } else {
    Serial.println("BuiltIn CAN Init Failed!");
  }
  //CAN0.setCallback(0, handleCAN0CB);
  CAN0.watchFor(); //let everything through

  // Initialize MCP2515 CAN controller at the specified speed
  if (CAN1.begin(500000)) {
    Serial.println("MCP2515 Init OK.");
  } else {
    Serial.println("MCP2515 Init Failed!");
  }
  /* we are using CAN1 only for sending
     CAN1.setCallback(0, handleCAN1CB);
     CAN1.watchFor(); //let everything through
  */

  //init frames for sending
  CAN_FRAME txFrame;
  txFrame.rtr = 0;
  txFrame.extended = false;
  txFrame.length = 8;

  //lamps off
  txFrame.id = 585;
  txFrame.data.uint8[0] = 0x00; //no engine, cruise or EML
  txFrame.data.uint8[1] = 0x00; // hfc.slice(0, 2)
  txFrame.data.uint8[2] = 0x00; // hfc.slice(2, 4)
  txFrame.data.uint8[3] = 0x00; //no overheat light
  txFrame.data.uint8[4] = 0x7E;
  txFrame.data.uint8[5] = 0x10;
  txFrame.data.uint8[6] = 0x00;
  txFrame.data.uint8[7] = 0x18;
  frames[0] = txFrame;

  //RPM - test 3k rpm * 6.4 = 4b00
  txFrame.id = 0x316;
  txFrame.data.uint8[0] = 0x05;
  txFrame.data.uint8[1] = 0x62;
  txFrame.data.uint8[2] = 0x00; //rpm 0,2
  txFrame.data.uint8[3] = 0x00; //rpm 2,4
  txFrame.data.uint8[4] = 0x65;
  txFrame.data.uint8[5] = 0x12;
  txFrame.data.uint8[6] = 0x00;
  txFrame.data.uint8[7] = 0x62;
  frames[1] = txFrame;

  //Temp - temp needle to beginning of red section [0xE6] - celsius*2
  txFrame.id = 0x329;
  txFrame.data.uint8[0] = 0xD9;
  txFrame.data.uint8[1] = 0xE6;
  txFrame.data.uint8[2] = 0xB2;
  txFrame.data.uint8[3] = 0x19;
  txFrame.data.uint8[4] = 0x00;
  txFrame.data.uint8[5] = 0xEE;
  txFrame.data.uint8[6] = 0x00;
  txFrame.data.uint8[7] = 0x00;
  frames[2] = txFrame;
}

void CanBus::handle() {
  CAN_FRAME frame;
  if (CAN0.read(frame)) {
    // Print message
    status.receivedCount++;
    docJ.clear();
    docJ["idx"] = 2;
    docJ["CAN"] = 0; //message received on CAN0
    docJ["id"] = String(frame.id, HEX);
    docJ["len"] = String(frame.length, DEC);
    docJ["ext"] = frame.extended;
    JsonArray data = docJ.createNestedArray("data");
    for (int i = 0; i < frame.length; i++) {
      data.add(String(frame.data.byte[i], HEX));
    }
    serializeJson(docJ, CAN0message);
    CAN0messageEmpty = false;
  }
  if (CAN1.read(frame)) {
    // Print message
    status.receivedCount++;
    docJ.clear();
    docJ["idx"] = 2;
    docJ["CAN"] = 1; //message received on CAN1
    docJ["id"] = String(frame.id, HEX);
    docJ["len"] = String(frame.length, DEC);
    docJ["ext"] = frame.extended;
    JsonArray data = docJ.createNestedArray("data");
    for (int i = 0; i < frame.length; i++) {
      data.add(String(frame.data.byte[i], HEX));
    }
    serializeJson(docJ, CAN1message);
    CAN1messageEmpty = false;
  }
}

void CanBus::sendMessageSet() {
  if (status.currentMillis - previousMillis >= intervals.CANsend) {
    previousMillis = status.currentMillis;

    displayValue = status.rpm * 6.4;
    frames[1].data.uint8[2] = (int)((displayValue & 0X000000FF)); //rpm 0,2
    frames[1].data.uint8[3] = (int)((displayValue & 0x0000FF00) >> 8 ); //rpm 2,4

    displayValue = status.coolant_temp * 2;
    frames[2].data.uint8[1] = (int)((displayValue & 0X000000FF));
    /*Serial.print("*** ");
      Serial.print(status.rpm);
      Serial.print("*** ");
      Serial.println(status.coolant_temp);*/
    /* When sending from CAN1 to CAN0 connections to mqtt fail
        and ultimatelly restarts - probably missing INT pin */
    CAN1.sendFrame(frames[0]);
    CAN1.sendFrame(frames[1]);
    CAN1.sendFrame(frames[2]);
    //printFrame(&frames[1], 0);
  }
}
