#include <ESP8266WiFi.h>
#include <Arduino.h>
#include "headers/main.h"
#include "headers/settings.h"
#include "headers/web.h"
#include "headers/moisture.h"
#include <ver.h>
#include "headers/utilities.h"
#include "headers/mqtt.h"
#include "headers/blynk.h"

void (*loopfcnPtr)();

void moistureSetup(){
  setSensorPin(A0);
}

void wifiSetup(){
  Settings s = getSettings();
  if(settingsSet()){
    println("Connecting to Wifi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(s.ssid, s.password);
    while(WiFi.status() != WL_CONNECTED){
      delay(500);
    }
    println("Wifi Connected");
  }
  else{
    println("Starting Accesspoint");
    WiFi.softAP(s.accessPointName);
    webSetup();
  }
}

void loopConfig(){
  println("Starting Accesspoint");
  Settings s = getSettings();
  WiFi.mode(WIFI_OFF);
  WiFi.softAP(s.accessPointName);
  webSetup();
  println("configuring");
  while(true){
    delay(100);
  }
}

void loopRun(){
  println("starting loop");
  _sensorSettings = getSettings();
  digitalWrite(D1, HIGH);
  float moisture = readMoisture();
  if(moisture > -1) //probe attached
  {
    blynkLoop(moisture);
    mqttLoop(moisture);
    digitalWrite(D5, LOW);

    uint64 sleep = (uint64)_sensorSettings.sleepInterval * 60000000; 
    //If we set the value too high use the max allowed value
    if(sleep > ESP.deepSleepMax() * .95)
      sleep = ESP.deepSleepMax() *.95;

    println("Going to sleep for " 
      + uint64ToString(sleep) 
      + "/" 
      + uint64ToString(ESP.deepSleepMax()) 
      + " microseconds");
    ESP.deepSleep(sleep);
    delay(1000);
  }
  else // probe detached put in config mode
  {
    loopfcnPtr = loopConfig;
  }
}
void setup() {
  pinMode(D1, OUTPUT); 
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  delay(1000);
  pinMode(D0, INPUT); 
  int doConf = digitalRead(D0);
  
  if(settingsSet() && doConf == HIGH){
    loopfcnPtr = loopRun;
    wifiSetup();
    blynkSetup();
    mqttSetup();
  }
  else{
    loopfcnPtr = loopConfig;
  }
}

void loop() {
  loopfcnPtr();
}


