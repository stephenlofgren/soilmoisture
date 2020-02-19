#include "headers/utilities.h"
#include "headers/settings.h"

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

char* printWiFiDisconnectReason(int reasonCode){
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


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID:");
  Serial.print(WiFi.SSID());
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("; IP Address:");
  Serial.print(ip);

  Serial.print("; Status:");
  Serial.print(decodeWiFiStatus(WiFi.status()));

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("; (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void printSettings(Settings s){
  Serial.print("Conf Key: ");
  Serial.println(s.key);
  Serial.print(" SSID: ");
  Serial.println(s.ssid);
  Serial.print(" pwd: ");
  Serial.println(s.password);
  Serial.print(" ap: ");
  Serial.println(s.accessPointName);
  Serial.print(" blynkKey: ");
  Serial.println(s.blynkKey);
  Serial.print(" sleepInterval: ");
  Serial.println(s.sleepInterval);
  Serial.print(" ver: ");
  Serial.println(s.version);
  Serial.print(" valuePin: ");
  Serial.println(s.valuePin);
  Serial.print(" messagePin: ");
  Serial.println(s.messagePin);
  Serial.print(" mqttServer: ");
  Serial.println(s.mqttServer);
  Serial.print(" mqttUser: ");
  Serial.println(s.mqttUser);
  Serial.print(" mqttPassword: ");
  Serial.println(s.mqttPassword);
 }
Settings printSettings(){
  _sensorSettings = getSettings();
  printSettings(_sensorSettings);
  return _sensorSettings;
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

