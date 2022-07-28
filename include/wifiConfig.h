#ifndef wifiConfig_h
#define wifiConfig_h

#include <definitions.h>
#include <configManager.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>


bool connectToWifi(String ssid, String pwd);
bool setStaticIp(String ip_addr, String gw_addr, String mask);
bool saveWifiCredentials(String ssid, String pwd, bool dyn_ip = true, String ip_addr = "0.0.0.0", String gw_addr = "0.0.0.0", String mask = "0.0.0.0");

String scanNetworks();
String getHostName();
String getMacAddress();
String getIpAddress();
String getSSID();
String getMode();
String wifiInfo();

void dnsProcessNext();
void updatemDNS();
void initWifi(bool ap_mode);

#endif