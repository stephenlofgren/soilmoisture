#include <ESP8266WiFi.h>
#include <Arduino.h>
#include "headers/main.h"
#include "headers/settings.h"
#include "headers/web.h"
#include "headers/moisture.h"
#include <ver.h>
#include "headers/diagnostics.h"
#include "headers/mqtt.h"
#include "headers/blynk.h"

void (*loopfcnPtr)();

void moistureSetup(){
  setSensorPin(A0);
}

void wifiSetup(){
  Settings s = printSettings();
  if(settingsSet()){
    Serial.println("Connecting to Wifi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(s.ssid, s.password);
    while(WiFi.status() != WL_CONNECTED){
      delay(500);
    }
    Serial.print("Wifi Connected");
  }
  else{
    Serial.println("Starting Accesspoint");
    WiFi.softAP(s.accessPointName);
    webSetup();
  }
}

void loopConfig(){
  Serial.println("Starting Accesspoint");
  Settings s = getSettings();
  WiFi.mode(WIFI_OFF);
  WiFi.softAP(s.accessPointName);
  webSetup();
  Serial.println("configuring");
  while(true){
    delay(100);
  }
}

void loopRun(){
  Serial.println("starting loop");
  _sensorSettings = printSettings();
  digitalWrite(D1, HIGH);
  float moisture = readMoisture();
  if(moisture > -1) //probe attached
  {
    blynkLoop(moisture);
    mqttLoop(moisture);
    digitalWrite(D5, LOW);

    Serial.print("Going to sleep for ");
    Serial.print(_sensorSettings.sleepInterval);
    Serial.println(" minutes");
    ESP.deepSleep(_sensorSettings.sleepInterval / 60000000);
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
  Serial.print("D0 ");
  Serial.println(doConf);

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


