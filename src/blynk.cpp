#include <BlynkSimpleEsp8266.h>
#include "headers/blynk.h"
#include "headers/settings.h"
    
void blynkSetup(){
    _sensorSettings = getSettings();
    Serial.print("Connecting to Blynk with key: ");
    Serial.println(_sensorSettings.blynkKey);
    Blynk.config(_sensorSettings.blynkKey);
    Serial.println("Blynk Running");
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
    Serial.print("Connecting to Blynk");
    if(Blynk.connect()){
      Blynk.virtualWrite(_sensorSettings.valuePin, moisture);
      Blynk.virtualWrite(_sensorSettings.messagePin, interpretReading(moisture));
    }
    else
    {
      Serial.print("Blynk Not Running");
    }
    
}

