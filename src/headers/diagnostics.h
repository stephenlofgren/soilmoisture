#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <ESP8266WiFi.h>
#include <Arduino.h>  // for type definitions
#include "headers/settings.h"

void printWifiStatus();
Settings printSettings();
char* printWiFiDisconnectReason(int reasonCode);
void printSettings(Settings s);

#endif
