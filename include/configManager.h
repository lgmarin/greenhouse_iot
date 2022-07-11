#ifndef configManager_h
#define configManager_h

#include <definitions.h>
#include <LittleFS.h>

typedef struct
{
  char wifi_ssid[32];
  char wifi_pw  [64];
} WiFi_Credential;

typedef struct
{
  IPAddress ip_addr;
  IPAddress gw_addr;
  IPAddress mask;
} IP_Config;

typedef struct
{
  WiFi_Credential  WiFi_Cred;
  IP_Config IP_config;
  char host_name[32];
  bool dyn_ip = false;
  bool ap_mode = false;
  uint16_t checksum;
} Wifi_Config;

void initFS();
bool loadConfigData();
void saveConfigData();

#endif