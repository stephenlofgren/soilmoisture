#ifndef MAIN_H
#define MAIN_H

#ifdef TARGET_ESP8266
#include <ESP8266WiFi.h>
#endif
#ifdef TARGET_ESP32
#include <WiFi.h>
#include <esp_sleep.h>
#endif

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <ver.h>

#include "headers/settings.h"
#include "headers/web.h"
#include "headers/moisture.h"
#include "headers/utilities.h"
#include "headers/mqttclient.h"
#include "headers/blynk.h"

void loopRun();
void loopConfig();


#ifdef TARGET_ESP8266
    int SENSOR_POWER = D1;
    int DO_CONFIG = D0;
    int SENSOR_READING = A0;
#endif
#ifdef TARGET_ESP32
    int SENSOR_POWER = A3;
    int DO_CONFIG = A0;
    int SENSOR_READING = A4;
#endif
#endif
