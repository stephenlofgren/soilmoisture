#include "headers/blynk.h"
#include "headers/settings.h"
#include "headers/mqttclient.h"
#include "headers/utilities.h"

#include <AsyncMqttClient.h>
AsyncMqttClient mqttClient;
int waitcounter;

//only needed for esp8266
#ifdef TARGET_ESP8266
Ticker mqttReconnectTimer;
Ticker wifiReconnectTimer;
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
#else
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;
#endif

WiFiClient wifiClient;

void connectToMqtt() {
  println("Connecting to MQTT...");
  mqttClient.connect();
}

bool waitForMqttConnection(){
    if(!mqttClient.connected()){
        mqttClient.connect();
        int connectionTimer = 10;
        while(!mqttClient.connected() && connectionTimer > 0){
            delay(1000);
            connectionTimer = connectionTimer - 1;
        }
    }
    return mqttClient.connected();
}

void connectToWifi() {
  _sensorSettings = getSettings();
  if(!WiFi.isConnected()){
    println("Connecting to Wi-Fi...");
    WiFi.begin(_sensorSettings.ssid, _sensorSettings.password);
  }
  else{
    connectToMqtt();
  }
}

#ifdef TARGET_ESP8266
//should handle connection prior to entering mqttloop
void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  println("Connected to Wi-Fi.");
  connectToMqtt();
}
#endif

//since the app doesn't maintain a connection we might not need this
#ifdef TARGET_ESP8266
void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  println("Disconnected from Wi-Fi.");
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, connectToWifi);
}
#endif

void WiFiEvent(WiFiEvent_t event) {
    #ifdef TARGET_ESP32
    Serial.printf("[WiFi-event] event: %d\n", event);
    switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
        println("WiFi connected");
        println("IP address: ");
        println(String(WiFi.localIP()));
        connectToMqtt();
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        println("WiFi lost connection");
        xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
		xTimerStart(wifiReconnectTimer, 0);
        break;
    }
    #endif
}

void onMqttConnect(bool sessionPresent) {
  println("Connected to MQTT.");
  println("Session present: " + String(sessionPresent));
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  println("Disconnected from MQTT.");

  if (WiFi.isConnected()) {
#ifdef TARGET_ESP8266
    mqttReconnectTimer.once(2, connectToMqtt);
#endif
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  println("Subscribe acknowledged.");
  println("  packetId: " + String(packetId) + "  qos: " + String(qos));
}

void onMqttUnsubscribe(uint16_t packetId) {
  println("Unsubscribe acknowledged.");
  println("  packetId: " + String(packetId));
 }

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  println("Publish received.");
  println("  topic: " + String(topic));
  println("  qos: " + String(properties.qos));
  println("  dup: " + String(properties.dup));
  println("  retain: " + String(properties.retain));
  println("  len: " + String(len));
  println("  index: " + String(index));
  println("  total: " + String(total));
}

void onMqttPublish(uint16_t packetId) {
  println("Publish acknowledged.");
  println("  packetId: " + String(packetId));
  waitcounter = 0;
}

void mqttSetup()
{
    _sensorSettings = getSettings();
#ifdef TARGET_ESP8266
    wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
    wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
#endif
    mqttClient.onConnect(onMqttConnect);
    mqttClient.onDisconnect(onMqttDisconnect);
    mqttClient.onSubscribe(onMqttSubscribe);
    mqttClient.onUnsubscribe(onMqttUnsubscribe);
    mqttClient.onMessage(onMqttMessage);
    mqttClient.onPublish(onMqttPublish);
    mqttClient.setCredentials( _sensorSettings.mqttUser, _sensorSettings.mqttPassword);
    mqttClient.setServer(_sensorSettings.mqttServer, 1883);
    mqttClient.connect();
}


void mqttLoop(int reading)
{
    if(!waitForMqttConnection()){
        println("MQTT Connect Timeout");
        return;
    }
    println("Publishing MQTT");
    
    String topic = String("ESP") + _sensorSettings.chipName + "/moisture/";
    waitcounter = 10;
    String payload = String(reading);
    println(payload);
    println(topic + " " + payload.c_str());
    int packetId = mqttClient.publish(topic.c_str(), 1, false, payload.c_str(), strlen(payload.c_str()), true);
    while(waitcounter > 0){
        waitForMqttConnection();
        delay(1000);
        println("wait 1");
        waitcounter = waitcounter - 1;
    }
}