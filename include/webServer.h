#ifndef webServer_h
#define webServer_h

#include <LittleFS.h>
#include <definitions.h>
#include <wifiConfig.h>
#include <readSensors.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

void setupWebServer();
void eventsLoop();

#endif