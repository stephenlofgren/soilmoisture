#ifndef SETTINGS_H
#define SETTINGS_H
  #include <Arduino.h>  // for type definitions

  #define CONFIG_START 32

  typedef struct {
    char key[8];
    char chipName[32];
    char version[32];
    char ssid[32];
    char password[32];
    char accessPointName[32];
    char blynkKey[33];
    int sleepInterval;
    int valuePin;
    int messagePin;
    char mqttServer[32];
    char mqttUser[32];
    char mqttPassword[32];
  } Settings, *pSettings;

  extern Settings _sensorSettings;

  void clearEeprom();

  void resetSettings();

  Settings createSettings(char key[8]
      , char chipName[32]
      , char ssid[32]
      , char password[32]
      , char accessPointName[32]
      , char blynkKey[33]
      , int sleepInterval
      , int valuePin
      , int messagePin
      , char mqttServer[32]
      , char mqttUser[32]
      , char mqttPassword[32]);

  Settings loadSettings();

  void storeSettings(Settings s);

  Settings getSettings();

  bool settingsSet();

  String getJson(Settings s);

#endif
