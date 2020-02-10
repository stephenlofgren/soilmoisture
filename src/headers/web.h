#ifndef WEB_H

#define WEB_H
#include "headers/settings.h"
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <mdns.h>

#include "static/index.html.gz.h"

void setSettingsChangedHandler(void (*f)(Settings settings));

void webSetupHandlers();

void webSetup();

#endif
