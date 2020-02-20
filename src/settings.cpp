#include "headers/settings.h"
#include "headers/utilities.h"
#include <ver.h>
#include <EEPROM.h>

bool eepromStarted = false;
Settings _sensorSettings;
char chipName[9];

char* getChipName(){
  itoa(ESP.getChipId(), chipName, 16);
  return chipName;
}

void startEeprom(){
  EEPROM.begin(512);
  eepromStarted = true;
  
  //if settings are empty set set to default
  Settings s = getSettings();
  if(strcmp(s.key, "")==0){
    Serial.println("key blank setting default");
    resetSettings();
  }

  //if we installed a new version let's start with a new settings
  //if(strcmp(VERSION_SHORT,s.version) != 0){
  //  Serial.print(VERSION_SHORT);
  //  Serial.print(" does not equal ");
  //  Serial.println(s.version);
  //  resetSettings();
  //  s = getSettings();
  //  printSettings(s);
  //}
}

void clearEeprom(){
  Serial.println("Clearing eeprom");
  if(!eepromStarted){
    startEeprom();
  }
  for (uint i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  printSettings();
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
    , char mqttPassword[32]){
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
    //Serial.print("Eeprom wrote ");
    Serial.println(i);
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
    //Serial.print("Eeprom read ");
    //Serial.println(i);
    return i;
}

void storeSettings(Settings s){
  Serial.print("Stored Settings - ");
  printSettings(s);
  EEPROM_writeAnything(CONFIG_START, s);
  EEPROM.commit();
}

Settings getSettings(){
  Settings s;
  EEPROM_readAnything(CONFIG_START, s);
  //Serial.print("Read in Settings - ");
  //printSettings(s);
  _sensorSettings = s;
  return s;
}

void resetSettings(){
  Serial.println("Settings Default");
  int sleep = ESP.deepSleepMax() / (uint64)60000000;
  Serial.println(sleep);
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
    , empty);
    printSettings(settings);
    storeSettings(settings);
    Serial.println("test");
}

bool settingsSet(){
  Settings s = getSettings();

  if(strcmp(s.key,"")==0){
    Serial.println("settings not yet set");
    return false;
  }
  else{
    Serial.printf("key set: %s\n", s.key);
    return true;
  }
}

String getJson(Settings s){
      String json = String("{\"key\":\"")           + s.key
    + String("\", \n \"chipName\":\"") + s.chipName
    + String("\", \n \"ssid\":\"")                + s.ssid
    + String("\", \n \"accessPointName\":\"")     + s.accessPointName
    + String("\", \n \"blynkKey\":\"")     + s.blynkKey
    + String("\", \n \"sleepInterval\":\"")     + s.sleepInterval
    + String("\", \n \"maxSleepInterval\":\"")     + uint64ToString(ESP.deepSleepMax() / (uint64)60000000)
    + String("\", \n \"valuePin\":\"")     + s.valuePin
    + String("\", \n \"messagePin\":\"")     + s.messagePin
    + String("\", \n \"mqttServer\":\"")     + s.mqttServer
    + String("\", \n \"mqttUser\":\"")     + s.mqttUser
    + String("\", \n \"mqttPassword\":\"")     + s.mqttPassword
    + "\"}";
    return json;
}