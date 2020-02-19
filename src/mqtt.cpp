#include "headers/mqtt.h"
#include "headers/settings.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClient wifiClient;
PubSubClient client;

void mqttSetup()
{
    _sensorSettings = getSettings();
    client.setClient(wifiClient);
    client.setServer(_sensorSettings.mqttServer, 1883);
}
 
void mqttLoop(int moistureReading)
{
    if(!client.connected())
    {
        client.connect(String(ESP.getChipId()).c_str(), _sensorSettings.mqttUser, _sensorSettings.mqttPassword);
        delay(500);
    }
    Serial.println("Publishing MQTT");
    
    String topic = String("ESP") + _sensorSettings.chipName + String("/moisture/");
    client.publish(topic.c_str(), String(moistureReading).c_str(), true);
        client.loop();

}