#ifndef UTILITIES_H
#define UTILITIES_H
#ifdef TARGET_ESP8266
#include <ESP8266WiFi.h>
#endif
#ifdef TARGET_ESP32
    #include <WiFi.h>
#endif
#include "headers/blynk.h"
#include "headers/settings.h"
#include <Arduino.h>  // for type definitions

char* decodeWiFiStatus(wl_status_t status);
char* getWiFiDisconnectReason(int reasonCode);
String getWifiStatusString();
String uint64ToString(uint64_t input);
char* getChipName();
bool didWakeFromDeepSleep();
char* getWakeupReason();

#endif
