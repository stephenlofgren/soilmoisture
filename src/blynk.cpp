#include <BlynkSimpleEsp8266.h>
#include "headers/blynk.h"
#include "headers/settings.h"

void blynkSetup(){
    _sensorSettings = getSettings();
    println("Connecting to Blynk with key: " 
      + String(_sensorSettings.blynkKey));
    Blynk.config(_sensorSettings.blynkKey);
    println("Blynk Running");
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

void blynkLoop(int moisture){
    if(Blynk.connect()){
      Blynk.virtualWrite(_sensorSettings.valuePin, moisture);
      println(interpretReading(moisture));
    }
    else
    {
      Blynk.connect();
      println("Connecting to Blynk");
    }
    
}

void println(String message){
  Serial.println(message);
  WidgetTerminal terminal(_sensorSettings.messagePin);
  terminal.println(_sensorSettings.chipName + (String)" - " + message);
  terminal.flush();
  //Blynk.virtualWrite(_sensorSettings.messagePin, message);
}
