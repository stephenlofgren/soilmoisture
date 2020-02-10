#ifndef SETTINGS_H
#define SETTINGS_H

#include <EEPROM.h>
#include <Arduino.h>  // for type definitions

#define CONFIG_START 32

typedef struct {
  char key[32];
  char version[32];
  char ssid[32];
  char password[32];
  char accessPointName[32];
  char blynkKey[32];
  int sleepInterval;
  int valuePin;
  int messagePin;
} Settings, *pSettings;

void clearEeprom();

void resetSettings();

Settings createSettings(char key[32]
    , char ssid[32]
    , char password[32]
    , char accessPointName[32]
    , char blynkKey[32]
    , int sleepInterval
    , int valuePin
    , int messagePin);

Settings loadSettings();

void storeSettings(Settings s);

Settings getSettings();

bool settingsSet();

String getJson(Settings s);

#endif
