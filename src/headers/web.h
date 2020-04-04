#ifndef WEB_H
#define WEB_H

#include <ver.h>
#ifdef TARGET_ESP8266
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include "headers/settings.h"
#include "headers/blynk.h"
#include "headers/moisture.h"
#include "static/index.html.gz.h"

void setSettingsChangedHandler(void (*f)(Settings settings));

void webSetupHandlers();

void webSetup();

#endif
