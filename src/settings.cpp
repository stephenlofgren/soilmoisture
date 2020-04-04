#include "headers/blynk.h"
#include "headers/settings.h"
#include "headers/utilities.h"
#include <ver.h>
#include <EEPROM.h>

bool eepromStarted = false;
Settings _sensorSettings;

void startEeprom(){
  EEPROM.begin(1024);
  eepromStarted = true;
  
  //if settings are empty set set to default
  Settings s = getSettings();
  if(strcmp(s.key, "")==0){
    println("key blank setting default");
    resetSettings();
  }
}

void clearEeprom(){
  println("Clearing eeprom");
  if(!eepromStarted){
    startEeprom();
  }
  Serial.println("length " + String(EEPROM.length()));
  for (uint i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}

Settings createSettings(char key[8]
    , char chipName[32]
    , char ssid[32]
    , char password[32]
    , char accessPointName[32]
    , char blynkKey[31]
    , int sleepInterval
    , int valuePin
    , int messagePin
    , char mqttServer[32]
    , char mqttUser[32]
    , char mqttPassword[32]
    , int maxReading
    , int targetReading
    , int minReading){
  Settings settings;
  strcpy(settings.key, key);
  strcpy(settings.chipName, chipName);
  strcpy(settings.version, VERSION_SHORT);
  strcpy(settings.ssid, ssid);
  strcpy(settings.password, password);
  strcpy(settings.accessPointName, accessPointName);
  strcpy(settings.blynkKey, blynkKey);
  strcpy(settings.mqttServer, mqttServer);
  strcpy(settings.mqttUser, mqttUser);
  strcpy(settings.mqttPassword, mqttPassword);
  settings.sleepInterval = sleepInterval;
  settings.valuePin = valuePin;
  settings.messagePin = messagePin;
  settings.maxReading = maxReading;
  settings.targetReading = targetReading;
  settings.minReading = minReading;
  return settings;
}

template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
    if(!eepromStarted){
      startEeprom();
    }
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          EEPROM.write(ee++, *p++);
    return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
    if(!eepromStarted){
      startEeprom();
    }
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          *p++ = EEPROM.read(ee++);
    return i;
}

void storeSettings(Settings s){
  println("Stored Settings:");
  println(getJson(s));
  EEPROM_writeAnything(CONFIG_START, s);
  EEPROM.commit();
  println("Settings Stored");
}

Settings getSettings(){
  Settings s;
  EEPROM_readAnything(CONFIG_START, s);
  _sensorSettings = s;
  return s;
}

void resetSettings(){
  #ifdef TARGET_ESP8266
  int sleep = (ESP.deepSleepMax() / (uint64)60000000) * .95;
  #endif
  #ifdef TARGET_ESP32
  int sleep = 200; //best I can tell esp32 doesn't support a sleep max calculation I might be able to rebuild calculation for both
  #endif
  char* empty = (char*)"";
  Settings settings = createSettings(empty
    , getChipName()
    , (char*)"*******"
    , (char*)"*******"
    , (char*)"WifiMoisture"
    , (char*)"*******"
    , sleep
    , 1
    , 2
    , empty
    , empty
    , empty
    , 0
    , 500
    , 1000);
    storeSettings(settings);
    println("Settings Default " + getJson(settings));
}

bool settingsSet(){
  Settings s = getSettings();

  if(strcmp(s.key,"")==0){
    println("settings not yet set");
    return false;
  }
  else{
    printf("key set: %s\n", s.key);
    return true;
  }
}

String getJson(Settings s){
  #ifdef TARGET_ESP8266
    int sleep = ESP.deepSleepMax() / (uint64)60000000;
  #endif
  #ifdef TARGET_ESP32
    int sleep = 200;
  #endif
      String json = String("{\"key\":\"")           + s.key
    + String("\", \n \"chipName\":\"") + s.chipName
    + String("\", \n \"ssid\":\"")                + s.ssid
    + String("\", \n \"accessPointName\":\"")     + s.accessPointName
    + String("\", \n \"blynkKey\":\"")     + s.blynkKey
    + String("\", \n \"sleepInterval\":\"")     + s.sleepInterval
    + String("\", \n \"maxSleepInterval\":\"")     + uint64ToString(sleep)
    + String("\", \n \"valuePin\":\"")     + s.valuePin
    + String("\", \n \"messagePin\":\"")     + s.messagePin
    + String("\", \n \"mqttServer\":\"")     + s.mqttServer
    + String("\", \n \"mqttUser\":\"")     + s.mqttUser
    + String("\", \n \"mqttPassword\":\"")     + s.mqttPassword
    + String("\", \n \"maxReading\":\"")     + s.maxReading
    + String("\", \n \"targetReading\":\"")     + s.targetReading
    + String("\", \n \"minReading\":\"")     + s.minReading
    + "\"}";
    return json;
}
