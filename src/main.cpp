#include "headers/main.h"

void (*loopfcnPtr)();

void pinSetup(){
  pinMode(DO_CONFIG, INPUT_PULLUP);
  pinMode(SENSOR_READING, INPUT);
  pinMode(SENSOR_POWER, OUTPUT);
  setSensorPin(SENSOR_READING);
  setPowerPin(SENSOR_POWER);
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
    String msg = "Wifi Connected " + WiFi.localIP().toString();
    println(msg);
  }
  else{
    println("Starting Accesspoint");
    WiFi.softAP(s.accessPointName);
    webSetup();
  }
}

void sleep(uint64_t sleepSeconds = 0){
  uint64_t sleep = sleepSeconds * 60000000;
  #ifdef TARGET_ESP8266
  if(sleep == 0){
    sleep = (uint64)_sensorSettings.sleepInterval * 60000000; 
  }
  //If we set the value too high use the max allowed value
  if(sleep > ESP.deepSleepMax() * .95)
    sleep = ESP.deepSleepMax() *.95;
  if(sleep > millis() * 1001){
    sleep = sleep - millis() * 1000;
  }
  println("Going to sleep for " + uint64ToString(sleep) + " microseconds");
  ESP.deepSleep(sleep);
  #endif
  #ifdef TARGET_ESP32
  if(sleep == 0)
    sleep = (uint64_t)1440 * 60000000;
  esp_sleep_enable_timer_wakeup(sleep);
  println("Going to sleep for " + uint64ToString(sleep) + " microseconds");
  esp_deep_sleep_start();
  #endif
}

void setupConfig(){
  println("Starting Accesspoint");
  Settings s = getSettings();
  WiFi.mode(WIFI_OFF);
  WiFi.softAP(s.accessPointName);
  webSetup();
}

void loopConfig(){
  println("Configuring");
  Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
  delay(600000);
  if(WiFi.softAPgetStationNum() == 0){
    sleep((uint64_t)1440);
  }
}

void loopRun(){
  println("starting loop");
  _sensorSettings = getSettings();
  float moisture = readMoisture();
  digitalWrite(SENSOR_POWER, LOW);
  if(moisture > -1) //probe attached
  {
    wifiSetup();
    blynkSetup();
    mqttSetup();
    blynkLoop(moisture);
    mqttLoop(moisture);
    sleep();
    delay(1000);
  }
  else // probe detached put in config mode
  {
    sleep();
  }
}

bool shouldSleep(){
  pinMode(DO_CONFIG, INPUT); 
  int doConf = digitalRead(DO_CONFIG);
  return doConf == LOW && didWakeFromDeepSleep();
}

bool shouldConfig(){
  if(!settingsSet())
    return true;
  int doConf = digitalRead(DO_CONFIG);
  Serial.println("Switched to Config Mode: " + String(doConf));
  return doConf == LOW && !didWakeFromDeepSleep();
}

void setup() {
  Serial.setDebugOutput(true);
  Serial.begin(115200);
  pinSetup();
  delay(1000);
  if(shouldSleep()){
    println("Going to Sleep Right away");
    sleep((uint64_t)1440);//8266 will end up sleeping max
    return;
  }
  
  if(shouldConfig()){
    println("Starting Configuration");
    setupConfig();
    loopfcnPtr = loopConfig;
    return;
  }
  loopfcnPtr = loopRun;
}

void loop() {
  loopfcnPtr();
}


