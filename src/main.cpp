#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Arduino.h>
#include "headers/main.h"
#include "headers/settings.h"
#include "headers/web.h"
#include "headers/moisture.h"
#include <ver.h>
#include "headers/diagnostics.h"

void (*loopfcnPtr)();

void moistureSetup(){
  setSensorPin(A0);
}

void wifiSetup(){
  Settings s = getSettings();
  if(settingsSet()){
    Serial.println("Connecting to Wifi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(s.ssid, s.password);
  }
  else{
    Serial.println("Starting Accesspoint");
    WiFi.softAP(s.accessPointName);
    webSetup();
  }
}

void blynkSetup(){
  Serial.println("Connecting to Blynk");
  Settings s = getSettings();
  Blynk.begin(s.blynkKey, s.ssid, s.password);
}

void config(){
  Serial.println("Starting Accesspoint");
  Settings s = getSettings();
  WiFi.softAP(s.accessPointName);
  webSetup();
  Serial.println("configuring");
  delay(60 * 5 * 1000);
}

void loopConfig(){
  config();
  //if user sets the settings switch to run mode
  if(settingsSet()){
    Serial.println("Entering running mode");
    wifiSetup();
    loopfcnPtr = loopRun;
  }
}

String interpretReading(float moistureValue)
{
  const int AirValue = 823;   //you need to replace this value with Value_1
  const int WaterValue = 421;  //you need to replace this value with Value_2
  int intervals = (AirValue - WaterValue)/3;
  if(moistureValue < (WaterValue + intervals))
  {
    return "Very Wet";
  }
  else if(moistureValue >= (WaterValue + intervals) && moistureValue < (AirValue - intervals))
  {
    return"Wet";
  }
  else
  {
    return "Dry";
  }
}

void loopRun(){
  Serial.println("running");
  Settings s = getSettings();
  digitalWrite(D1, HIGH);
  float moisture = readMoisture();
  if(moisture > -1) //probe attached
  {
    wifiSetup();
    blynkSetup();
    Blynk.run();
    Serial.println("Blynk Running");
    Blynk.virtualWrite(s.valuePin, moisture);
    Blynk.virtualWrite(s.messagePin, interpretReading(moisture));
    WiFi.mode(WIFI_OFF);
    digitalWrite(D5, LOW);

    Serial.print("Goint to sleep for ");
    Serial.print(s.sleepInterval);
    Serial.println(" seconds");
    ESP.deepSleep(s.sleepInterval * 1000000);
  }
  else // probe detached put in config mode
  {
    loopfcnPtr = loopConfig;
  }
}
void setup() {
  pinMode(D1, OUTPUT); 
  pinMode(D4, INPUT_PULLUP);
  int doConf = digitalRead(D4);
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  WiFi.mode(WIFI_OFF);
  if(settingsSet() || doConf == LOW){
    loopfcnPtr = loopRun;
  }
  else{
    loopfcnPtr = loopConfig;
  }
}

void loop() {
  loopfcnPtr();
}


