#ifndef wifiConfig_h
#define wifiConfig_h

#include <definitions.h>
#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

bool openCaptivePortal();
String scanNetworks();
bool configuremDNS();
bool connectToWifi(String ssid, String pwd, String ip = "", String gw = "", String mask = "");
void dnsProcessNext();

#endif