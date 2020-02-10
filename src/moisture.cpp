#include <Arduino.h>
#include "headers/moisture.h"

int SensorPin;
int Vo, Vsum, Vcount; 

void setSensorPin(int pin){
    SensorPin = pin;
}

float readMoisture() {
  Vsum = 0;
  for(Vcount =0; Vcount < 500; Vcount++)
  {
    Vo = analogRead(SensorPin);
    Vsum = Vsum + Vo;
    delay(10);
  }
  
  Vo = Vsum / Vcount;
  Serial.print("reading ");
  Serial.println(Vo);
  return Vo;
}

