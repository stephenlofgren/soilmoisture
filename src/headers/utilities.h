#ifndef UTILITIES_H
#define UTILITIES_H

#include <ESP8266WiFi.h>
#include <Arduino.h>  // for type definitions
#include "headers/settings.h"

char* decodeWiFiStatus(wl_status_t status);
char* getWiFiDisconnectReason(int reasonCode);
String getWifiStatusString();
String uint64ToString(uint64_t input);
#endif
