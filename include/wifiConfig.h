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


bool connectToWifi(String ssid, String pwd);
bool setStaticIp(String ip_addr, String gw_addr, String mask);
// bool setStaticIp();

//EXTERNAL FUNCTIONS
String scanNetworks();
String getHostName();
String getMacAddress();
String getIpAddress();
String getSSID();
String getMode();

void dnsProcessNext();
void initWifi(bool ap_mode);

#endif