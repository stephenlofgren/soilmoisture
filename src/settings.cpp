#include "headers/settings.h"
#include "headers/diagnostics.h"
#include <ver.h>

bool eepromStarted = false;
Settings _loadedSettings;

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
  if(strcmp(VERSION_SHORT,s.version) != 0){
    Serial.print(VERSION_SHORT);
    Serial.print(" does not equal ");
    Serial.println(s.version);
    resetSettings();
    s = getSettings();
    printSettings(s);
  }
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

Settings createSettings(char key[32]
    , char ssid[32]
    , char password[32]
    , char accessPointName[32]
    , char blynkKey[32]
    , int sleepInterval
    , int valuePin
    , int messagePin){
  Settings settings;
  strcpy(settings.key, key);
  strcpy(settings.version, VERSION_SHORT);
  strcpy(settings.ssid, ssid);
  strcpy(settings.password, password);
  strcpy(settings.accessPointName, accessPointName);
  strcpy(settings.blynkKey, blynkKey);
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
    Serial.print("Eeprom wrote ");
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
    Serial.print("Eeprom read ");
    Serial.println(i);
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
  _loadedSettings = s;
  return s;
}

void resetSettings(){
  Serial.println("Settings Default");
  Settings settings = createSettings((char*)"default"
    , (char*)"*******"
    , (char*)"*******"
    , (char*)"WifiMoisture"
    , (char*)"*******"
    , 30
    , 1
    , 2);
    storeSettings(settings);
    delay(10000);
}

bool settingsSet(){
  Settings s = getSettings();

  if(strcmp(s.key,"default")==0){
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
    + String("\", \n \"ssid\":\"")                + s.ssid
    + String("\", \n \"accessPointName\":\"")     + s.accessPointName
    + String("\", \n \"blynkKey\":\"")     + s.blynkKey
    + String("\", \n \"sleepInterval\":\"")     + s.sleepInterval
    + String("\", \n \"valuePin\":\"")     + s.valuePin
    + String("\", \n \"messagePin\":\"")     + s.messagePin
    + "\"}";
    return json;
}
