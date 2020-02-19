#ifndef UTILITIES_H
#define UTILITIES_H

#include <ESP8266WiFi.h>
#include <Arduino.h>  // for type definitions
#include "headers/settings.h"

void printWifiStatus();
Settings printSettings();
char* printWiFiDisconnectReason(int reasonCode);
void printSettings(Settings s);
String uint64ToString(uint64_t input);
#endif
