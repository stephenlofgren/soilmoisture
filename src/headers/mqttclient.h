#ifndef MQTT_H
    #define MQTT_H
#ifdef TARGET_ESP8266
#include <ESP8266WiFi.h>
#include <Ticker.h>
#else
#endif
    void mqttSetup();
    void mqttLoop(int reading);
#endif