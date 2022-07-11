#ifndef wifiConfig_h
#define wifiConfig_h

#include <definitions.h>
#include <configManager.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>

bool openCaptivePortal();
String scanNetworks();
bool configuremDNS();
bool connectToWifi(String ssid, String pwd, bool static_ip = false);
void dnsProcessNext();

void initWifi();

#endif