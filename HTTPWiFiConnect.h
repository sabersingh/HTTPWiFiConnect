/*
  HTTPWiFiConnect.h - Library to connect ESP8266 boards to WiFi using simple HTTP GET request.
  Created by Simerneet Singh(A.K.A Saber), August 13, 2018.
  Relased into the Public Domain
*/
#ifndef HTTPWiFiConnect_h
#define HTTPWiFiConnect_h

#include <Arduino.h>
#include "ESP8266WiFi.h"
#include "ESP8266WebServer"
#include "FS.h"
#include "HTTPWiFiConnect.h"

class HTTPWiFiConnect{
  public:
    HTTPWiFiConnect(String WiFiName);
    String indexPage;
    void handleServer();

  private:
    String wifiName;
    bool checkWiFi();
    void startServer();
    void launchAP();
    void configWiFi();
};

#endif