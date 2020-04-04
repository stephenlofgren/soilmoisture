#include "headers/utilities.h"
#ifdef TARGET_ESP8266
  #include <ESP8266WiFi.h>
#endif
#ifdef TARGET_ESP32
  #include <WiFi.h>
#endif

char chipName[9];
char wakeReason[50];

char* decodeWiFiStatus(wl_status_t status){
  if (status == WL_NO_SHIELD){
    return (char*)"WL_NO_SHIELD";
  }
  else if(status == WL_IDLE_STATUS){
    return (char*)"WL_IDLE_STATUS";
  }
  else if (status == WL_NO_SSID_AVAIL){
    return (char*)"WL_NO_SSID_AVAIL";
  }
  else if (status == WL_SCAN_COMPLETED){
    return (char*)"WL_SCAN_COMPLETED";
  }
  else if (status == WL_CONNECTED){
    return (char*)"WL_CONNECTED";
  }
  else if (status == WL_CONNECT_FAILED){
    return (char*)"WL_CONNECT_FAILED";
  }
  else if (status == WL_CONNECTION_LOST){
    return (char*)"WL_CONNECTION_LOST";
  }
  else if (status == WL_DISCONNECTED){
    return (char*)"WL_DISCONNECTED";
  }
  else return (char*)status;
}

char* getWiFiDisconnectReason(int reasonCode){
  if(reasonCode == 1){
    return (char*)"WIFI_DISCONNECT_REASON_UNSPECIFIED";
  }
  if(reasonCode == 2){
    return (char*)"WIFI_DISCONNECT_REASON_AUTH_EXPIRE";
  }
  if(reasonCode == 3){
    return (char*)"WIFI_DISCONNECT_REASON_AUTH_LEAVE";
  }
  if(reasonCode == 4){
    return (char*)"WIFI_DISCONNECT_REASON_ASSOC_EXPIRE";
  }
  if(reasonCode == 5){
    return (char*)"WIFI_DISCONNECT_REASON_ASSOC_TOOMANY";
  }
  if(reasonCode == 6){
    return (char*)"WIFI_DISCONNECT_REASON_NOT_AUTHED";
  }
  if(reasonCode == 7){
    return (char*)"WIFI_DISCONNECT_REASON_NOT_ASSOCED";
  }
  if(reasonCode == 8){
    return (char*)"WIFI_DISCONNECT_REASON_ASSOC_LEAVE";
  }
  if(reasonCode == 9){
    return (char*)"WIFI_DISCONNECT_REASON_ASSOC_NOT_AUTHED";
  }
  if(reasonCode == 10){
    return (char*)"WIFI_DISCONNECT_REASON_DISASSOC_PWRCAP_BAD";
  }
  if(reasonCode == 11){
    return (char*)"WIFI_DISCONNECT_REASON_DISASSOC_SUPCHAN_BAD";
  }
  if(reasonCode == 13){
    return (char*)"WIFI_DISCONNECT_REASON_IE_INVALID";
  }
  if(reasonCode == 14){
    return (char*)"WIFI_DISCONNECT_REASON_MIC_FAILURE";
  }
  if(reasonCode == 15){
    return (char*)"WIFI_DISCONNECT_REASON_4WAY_HANDSHAKE_TIMEOUT";
  }
  if(reasonCode == 16){
    return (char*)"WIFI_DISCONNECT_REASON_GROUP_KEY_UPDATE_TIMEOUT";
  }
  if(reasonCode == 17){
    return (char*)"WIFI_DISCONNECT_REASON_IE_IN_4WAY_DIFFERS";
  }
  if(reasonCode == 18){
    return (char*)"WIFI_DISCONNECT_REASON_GROUP_CIPHER_INVALID";
  }
  if(reasonCode == 19){
    return (char*)"WIFI_DISCONNECT_REASON_PAIRWISE_CIPHER_INVALID";
  }
  if(reasonCode == 20){
    return (char*)"WIFI_DISCONNECT_REASON_AKMP_INVALID";
  }
  if(reasonCode == 21){
    return (char*)"WIFI_DISCONNECT_REASON_UNSUPP_RSN_IE_VERSION";
  }
  if(reasonCode == 22){
    return (char*)"WIFI_DISCONNECT_REASON_INVALID_RSN_IE_CAP";
  }
  if(reasonCode == 23){
    return (char*)"WIFI_DISCONNECT_REASON_802_1X_AUTH_FAILED";
  }
  if(reasonCode == 24){
    return (char*)"WIFI_DISCONNECT_REASON_CIPHER_SUITE_REJECTED";
  }
  if(reasonCode == 200){
    return (char*)"WIFI_DISCONNECT_REASON_BEACON_TIMEOUT";
  }
  if(reasonCode == 201){
    return (char*)"WIFI_DISCONNECT_REASON_NO_AP_FOUND";
  }
  if(reasonCode == 202){
    return (char*)"WIFI_DISCONNECT_REASON_AUTH_FAIL";
  }
  if(reasonCode == 203){
    return (char*)"WIFI_DISCONNECT_REASON_ASSOC_FAIL";
  }
  if(reasonCode == 204){
    return (char*)"WIFI_DISCONNECT_REASON_HANDSHAKE_TIMEOUT";
  }
  return (char*)reasonCode;
}

String getWifiStatusString() {
  IPAddress ip = WiFi.localIP();
  long rssi = WiFi.RSSI();

  String message = "SSID:" + WiFi.SSID() 
    + "; IP Address:" + ip.toString() 
    + "; Status:" + decodeWiFiStatus(WiFi.status())
    + "; (RSSI):" + rssi 
    + " dBm";
  return message;
}

String uint64ToString(uint64_t input) {
  String result = "";
  uint8_t base = 10;

  do {
    char c = input % base;
    input /= base;

    if (c < 10)
      c +='0';
    else
      c += 'A' - 10;
    result = c + result;
  } while (input);
  return result;
}

char* getChipName(){
#ifdef TARGET_ESP8266
  uint64_t chipid = ESP.getChipId();
#endif
#ifdef TARGET_ESP32
  uint64_t chipid = ESP.getEfuseMac();
#endif
  itoa(chipid, chipName, 16);
  return chipName;
}

bool didWakeFromDeepSleep(){
  char* reason = getWakeupReason();
  bool result =  strcmp(reason, "Deep-Sleep Wake") == 0;
  return result;
}

char* getWakeupReason(){
  #ifdef TARGET_ESP32
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : 
      println("Wakeup caused by external signal using RTC_IO"); 
      strcpy_P(wakeReason, PSTR("External System"));
      break;
    case ESP_SLEEP_WAKEUP_EXT1 : 
      println("Wakeup caused by external signal using RTC_CNTL");
      strcpy_P(wakeReason, PSTR("External System"));
      break;
    case ESP_SLEEP_WAKEUP_TIMER :
      println("Wakeup caused by timer");
      strcpy_P(wakeReason, PSTR("Deep-Sleep Wake"));
      break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : 
      println("Wakeup caused by touchpad"); 
      strcpy_P(wakeReason, PSTR("Sensor"));
      break;
    case ESP_SLEEP_WAKEUP_ULP :
      println("Sensor");
      strcpy_P(wakeReason, PSTR("Sensor"));
      break;
    default : 
      sprintf (wakeReason, "Other: %d\n", wakeup_reason);
      println(String(wakeReason));
      break;
  }
  #endif
  #ifdef TARGET_ESP8266
  switch (ESP.getResetInfoPtr()->reason) {
    case REASON_DEFAULT_RST:
      // do something at normal startup by power on
      strcpy_P(wakeReason, PSTR("Power on"));
      break;
     
    case REASON_WDT_RST:
      // do something at hardware watch dog reset
      strcpy_P(wakeReason, PSTR("Hardware Watchdog"));     
      break;
     
    case REASON_EXCEPTION_RST:
      // do something at exception reset
      strcpy_P(wakeReason, PSTR("Exception"));     
      break;
     
    case REASON_SOFT_WDT_RST:
      // do something at software watch dog reset
      strcpy_P(wakeReason, PSTR("Software Watchdog"));
      break;
     
    case REASON_SOFT_RESTART:
      // do something at software restart ,system_restart
      strcpy_P(wakeReason, PSTR("Software/System restart"));
      break;
     
    case REASON_DEEP_SLEEP_AWAKE:
      // do something at wake up from deep-sleep
      strcpy_P(wakeReason, PSTR("Deep-Sleep Wake"));
      break;
     
    case REASON_EXT_SYS_RST:
      // do something at external system reset (assertion of reset pin)
      strcpy_P(wakeReason, PSTR("External System"));
      break;
     
    default: 
      // do something when reset occured for unknown reason
      strcpy_P(wakeReason, PSTR("Other"));     
      break;
  }
  char buffer [30];
  sprintf (buffer, "\n\nReason for reboot: %s\n", wakeReason);
  println(String(buffer));
  println("----------------------------------------------");
  #endif
  return wakeReason;
}

