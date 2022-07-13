#ifndef configManager_h
#define configManager_h

#include <definitions.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>

// CONFIGURATIONS STRUCTS

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
  WiFi_Credential   WiFi_cred;
  IP_Config         IP_config;
  bool              dyn_ip = false;
  uint16_t          checksum;
} Wifi_Config;

extern Wifi_Config        Wifi_config;

typedef struct
{
  char      host_name[32];
  bool      ap_mode = false;
  int       air_value;
  int       wat_value;
  uint16_t  checksum;
} Device_Config;

extern Device_Config      Device_config;


// EXTERNAL FUNCTIONS
bool initFS();

bool loadWifiConfig();
bool storeWifiConfig(String SSID, String password, bool dyn_ip, IPAddress ip, IPAddress gw, IPAddress mask);
bool removeWifiConfig();

bool loadDeviceConfig();
bool storeDeviceConfig(String host_name, String air_v, String wat_v, bool apmode);
bool removeDeviceConfig();

bool initDeviceConfiguration();


#endif