#ifndef wifiConfig_h
#define wifiConfig_h

#include <definitions.h>
#include <configManager.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>

// extern String             host_name;
// extern long               lastScanMillis;
// extern long               currentMillis;
// extern bool               ap_mode = true;

bool openCaptivePortal();
String scanNetworks();
bool configuremDNS();
bool connectToWifi(String ssid, String pwd);
void dnsProcessNext();

bool setStaticIp(String ip_addr, String gw_addr, String mask);
bool setStaticIp();

void initWifi();

#endif