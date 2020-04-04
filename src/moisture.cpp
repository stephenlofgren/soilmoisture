#include <Arduino.h>
#include "headers/moisture.h"

int SensorPin;
int PowerPin;

void setSensorPin(int pin){
    SensorPin = pin;
}

void setPowerPin(int pin){
    PowerPin = pin;
}

void checkSettings(int reading){
    Settings s = getSettings();
    bool dirty = false;
    if(reading > s.maxReading){
      Serial.println("Setting maxReading " + String(reading));
      s.maxReading = reading;
      dirty = true;
    }
    if(reading < s.minReading){
      Serial.println("Setting minReading " + String(reading));
      s.minReading = reading;
      dirty = true;
    }
    if(dirty){
      storeSettings(s);
    }
}

int takeReading(unsigned int numSamples, unsigned int delayCount) {
  unsigned int Vo, Vsum, Vcount; 
  Vsum = 0;

  digitalWrite(PowerPin, HIGH);
  for(Vcount =0; Vcount < numSamples; Vcount++)
  {
    Vo = analogRead(SensorPin);
    Vsum = Vsum + Vo;
    Serial.println(Vo);
    if(delayCount > 0){
      unsigned long startTime = millis();
      while(millis() - startTime < delayCount){
        Serial.println("Countdown: " + String(delayCount  + startTime - millis()));
      }
    }
  }
  digitalWrite(PowerPin, LOW);

  Vo = Vsum / Vcount;
  Serial.println("Aggregate Reading: " + String(Vo));
  checkSettings(Vo);
  return Vo;
}

///returns positive number for too wet
///returns negative number for too dry
///0 is just right
///expressed as a percentage of the range
float readMoisture() {

  int Vo = takeReading(10);
  Settings s = getSettings();
  float overunder = (float)(s.targetReading - Vo) / (s.maxReading - s.minReading);
  Serial.println("Overunder " + String(overunder));
  return overunder; 
}

