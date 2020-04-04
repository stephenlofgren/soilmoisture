#include "headers/blynk.h"
#ifdef TARGET_ESP8266
#include <BlynkSimpleEsp8266.h>
#endif
#ifdef TARGET_ESP32
    #include <BlynkSimpleEsp32.h>
#endif
#include "headers/settings.h"
#include <string> 

void blynkSetup(){
    _sensorSettings = getSettings();
    println("Connecting to Blynk with key: " 
      + String(_sensorSettings.blynkKey));
    Blynk.config(_sensorSettings.blynkKey);
    println("Blynk Running");
}

void blynkLoop(float reading){
    while(!Blynk.connected()){
      Blynk.connect();
      println("Waiting for Blynk Connection");
      delay(1000);
}

    if(Blynk.connected()){
      println("Connected to Blynk");
      String msg = "Moisture Reading: " + String(reading) + " on pin:" + String(_sensorSettings.valuePin);
      Blynk.virtualWrite(_sensorSettings.valuePin, reading);
      println(msg);
    }
}

void println(String message){
  Serial.println(message);
  if(Blynk.connected()){
  WidgetTerminal terminal(_sensorSettings.messagePin);
  terminal.println(_sensorSettings.chipName + (String)" - " + message);
  terminal.flush();
  }
}
