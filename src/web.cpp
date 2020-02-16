#include "headers/web.h"
#include "headers/diagnostics.h"
#include <ver.h>
#include <string>

using namespace std; // stringstream, like almost everything, is in std

AsyncWebServer server(80);
void (*iSettingsChanged)(Settings settings);

void setSettingsChangedHandler(void (*f)(Settings settings)){
    iSettingsChanged = f;
}

void webSetupHandlers(){
  server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request){
    //request->send(200, "text/plain", String(ESP.getFreeHeap()));
    Settings s = getSettings();
    String json = getJson(s);
    Serial.println("-------------------------------------------------------------------------");
    Serial.println(json);
    Serial.println("-------------------------------------------------------------------------");

    AsyncWebServerResponse *response =
      request->beginResponse_P(200, "text/json", json.c_str());
    request->send(response);

  });
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){
    //request->send(200, "text/plain", String(ESP.getFreeHeap()));
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html_gz, index_html_gz_len);
    response->addHeader("Content-Encoding", "gzip");
    //response->addHeader("Last-Modified", _last_modified);
    request->send(response);

  });

  server.on("/reset", HTTP_POST, [](AsyncWebServerRequest *request){
    resetSettings();
  });

  server.on("/index.html", HTTP_POST, [](AsyncWebServerRequest *request){
    int i;
    int params = request->params();

    Settings newSettings;
    strcpy(newSettings.key, (char*)"set");
    strcpy(newSettings.version, VERSION_SHORT);
    
    for(i=0;i<params;i++){
      AsyncWebParameter* p = request->getParam(i);
      if(p->isPost()){
        Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
        //need to pull post params and make a settings object
        if(p->name() == "ssid"){
          p->value().toCharArray(newSettings.ssid, 32);
          Serial.printf("ssid changed to %s\n", newSettings.ssid );
        }
        if(p->name() == "chipName"){
          p->value().toCharArray(newSettings.chipName, 32);
          Serial.printf("chipName changed to %s\n", newSettings.chipName );
        }
        else if(p->name() == "password"){
          p->value().toCharArray(newSettings.password, 32);
          Serial.printf("password changed to %s\n", newSettings.password );
        }
        else if(p->name() == "accessPointName"){
          p->value().toCharArray(newSettings.accessPointName, 32);
          Serial.printf("ap changed to %s\n", newSettings.accessPointName );
        }
        else if(p->name() == "blynkKey"){
          p->value().toCharArray(newSettings.blynkKey, 33);
          Serial.printf("blynkKey changed to %s\n", newSettings.blynkKey );
        }
        else if(p->name() == "sleepInterval"){
          newSettings.sleepInterval = atoi((char*)p->value().c_str());
          Serial.printf("sleepInterval changed to %d\n", newSettings.sleepInterval );
        }
        else if(p->name() == "valuePin"){
          newSettings.valuePin = atoi((char*)p->value().c_str());
          Serial.printf("valuePin changed to %d\n", newSettings.valuePin );
        }
        else if(p->name() == "messagePin"){
          newSettings.messagePin = atoi((char*)p->value().c_str());
          Serial.printf("messagePin changed to %d\n", newSettings.messagePin );
        }
        else if(p->name() == "mqttServer"){
          p->value().toCharArray(newSettings.mqttServer, 32);
          Serial.printf("mqttServer changed to %s\n", newSettings.mqttServer );
        }
        else if(p->name() == "mqttUser"){
          p->value().toCharArray(newSettings.mqttUser, 32);
          Serial.printf("mqttUser changed to %s\n", newSettings.mqttUser );
        }
        else if(p->name() == "mqttPassword"){
          p->value().toCharArray(newSettings.mqttPassword, 32);
          Serial.printf("mqttPassword changed to %s\n", newSettings.mqttPassword );
        }
      }
      else{
        Serial.printf("_QUERY[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
  }
  Serial.print("Received Settings - ");

  printSettings(newSettings);
  storeSettings(newSettings);
  iSettingsChanged(newSettings);
  request->send(200, "text/plain", "Success");
  });

  server.onNotFound([](AsyncWebServerRequest *request){
    Serial.printf("NOT_FOUND: ");
    if(request->method() == HTTP_GET)
      Serial.printf("GET");
    else if(request->method() == HTTP_POST)
      Serial.printf("POST");
    else if(request->method() == HTTP_DELETE)
      Serial.printf("DELETE");
    else if(request->method() == HTTP_PUT)
      Serial.printf("PUT");
    else if(request->method() == HTTP_PATCH)
      Serial.printf("PATCH");
    else if(request->method() == HTTP_HEAD)
      Serial.printf("HEAD");
    else if(request->method() == HTTP_OPTIONS)
      Serial.printf("OPTIONS");
    else
      Serial.printf("UNKNOWN");
    Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());

    if(request->contentLength()){
      Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
      Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
    }

    int headers = request->headers();
    int i;
    for(i=0;i<headers;i++){
      AsyncWebHeader* h = request->getHeader(i);
      Serial.printf("_HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
    }

    int params = request->params();
    for(i=0;i<params;i++){
      AsyncWebParameter* p = request->getParam(i);
      if(p->isFile()){
        Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
      } else if(p->isPost()){
        Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
      } else {
        Serial.printf("_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
    }

    request->send(404);
  });
}

void webSetup(){
  //MDNS.addService("http","tcp",80);
  server.rewrite("/", "/index.html");
  webSetupHandlers();
  server.begin();
}
