#include <configManager.h>
#include <Arduino.h>

Wifi_Config        Wifi_config;
Device_Config      Device_config;

const char* wifi_config_file = "/wifi.cfg";
const char* device_config_file = "/device.cfg";

///
///     LITTLEFS Management functions
///


/*!
 *  @brief  Initialize LitteFS.
 *  @return Returns true if initialized successfully.
 */
bool initFS() {
  if (!LittleFS.begin()) {
    Serial.print(F("\n[ERROR]: An error has occurred while mounting LittleFS"));
    return false;
  }
  else{
    Serial.print(F("\n[INFO]: LittleFS mounted successfully"));
    return true;
  }
}

int calcChecksum(uint8_t* address, uint16_t sizeToCalc)
{
  uint16_t checkSum = 0;

  for (uint16_t index = 0; index < sizeToCalc; index++)
  {
    checkSum += * ( ( (byte*) address ) + index);
  }
  return checkSum;
}

bool loadConfigData(void *str_Config, size_t size, char* filename)
{
  File file = LittleFS.open(filename, "r");
  memset(str_Config, 0, size);

  if (file)
  {
    file.readBytes((char *) str_Config, size);
    file.close();
    return true;
  }
  else
  {
    return false;
  }
}

bool saveConfigData(void *str_Config, size_t size, char* filename)
{
  File file = LittleFS.open(filename, "w");

  if (file)
  {
    file.write((uint8_t*) str_Config, size);
    file.close();
    return true;
  }
  else
  {
    return false;
  }
}

bool removeConfigData(char* filename)
{
  if (LittleFS.exists(filename))
  {
    if (LittleFS.remove(filename))
    {
      Serial.print(F("\n[INFO]: File removed!"));
      return true;
    }
    Serial.print(F("[ERROR]: File couldn't be removed! Removal error."));
    return false;
  }
  Serial.print(F("[ERROR]: File couldn't be removed! Not found."));
  return false;
}

/*!
 *  @brief  Copy the content of a string to a allocated char array.
 *  @param  string Input string.
 *  @param  char_loc Destination char pointer.
 */
void storeString(String string, char* char_loc)
{
  if (strlen(string.c_str()) < sizeof(char_loc) - 1)
    strcpy(char_loc, string.c_str());
  else
    strncpy(char_loc, string.c_str(), sizeof(char_loc) - 1);
}


///
///     WIFI CONFIGURATION FILE
///

/*!
 *  @brief  Load WifiConfiguration saved in LitteFS.
 *  @return Returns true if configuration loaded successfully.
 */
bool loadWifiConfig()
{
  if(loadConfigData(&Wifi_config, sizeof(Wifi_config), (char*) wifi_config_file))
  {
    if ( Wifi_config.checksum != calcChecksum( (uint8_t*) &Wifi_config, sizeof(Wifi_config) - sizeof(Wifi_config.checksum) ) )
    {
      Serial.print(F("\n[ERROR]: Wifi Credentials checksum wrong!"));
      return false;
    }

    // Don't permit NULL SSID and password len < MIN_AP_PASSWORD_SIZE (8)
    if ( (String(Wifi_config.WiFi_cred.wifi_ssid) == "") || (String(Wifi_config.WiFi_cred.wifi_pw) == "") )
    {
      Serial.print(F("\n[ERROR]: Wifi SSID or Password is empty!"));
      return false;
    }
    Serial.print(F("\n[INFO]: Wifi Config File Read. Checksum ok."));
    return true; 
  }
  else
  {
    Serial.print(F("\n[ERROR]: Could not read Wifi Config File."));
    return false;
  }
}

/*!
 *  @brief  Store WifiConfiguration into LitteFS.
 *  @return Returns true if configuration saved successfully.
 */
bool storeWifiConfig(String SSID, String password, bool dyn_ip, IPAddress ip, IPAddress gw, IPAddress mask)
{
  memset(&Wifi_config, 0, sizeof(Wifi_config));

  //storeString(SSID, Wifi_config.WiFi_cred.wifi_ssid);
  
  //SAVE SSID
  if (strlen(SSID.c_str()) < sizeof(Wifi_config.WiFi_cred.wifi_ssid) - 1)
    strcpy(Wifi_config.WiFi_cred.wifi_ssid, SSID.c_str());
  else
    strncpy(Wifi_config.WiFi_cred.wifi_ssid, SSID.c_str(), sizeof(Wifi_config.WiFi_cred.wifi_ssid) - 1);

  //SAVE PWD
  if (strlen(password.c_str()) < sizeof(Wifi_config.WiFi_cred.wifi_pw) - 1)
    strcpy(Wifi_config.WiFi_cred.wifi_pw, password.c_str());
  else
    strncpy(Wifi_config.WiFi_cred.wifi_pw, password.c_str(), sizeof(Wifi_config.WiFi_cred.wifi_pw) - 1);  

  if ((String(Wifi_config.WiFi_cred.wifi_ssid) = "") && (String(Wifi_config.WiFi_cred.wifi_pw) = ""))
    Serial.println(F("[WARNING]: Null SID or Password!"));

  //SAVE IP
  if (dyn_ip)
  {
    Wifi_config.IP_config.ip_addr = ip;
    Wifi_config.IP_config.gw_addr = gw;
    Wifi_config.IP_config.mask = mask;
  }
  
  //Calculate checksum and save credentials
  Wifi_config.checksum = calcChecksum((uint8_t*) &Wifi_config, sizeof(Wifi_config) - sizeof(Wifi_config.checksum));
  if (saveConfigData(&Wifi_config, sizeof(Wifi_config), (char*) wifi_config_file))
  {
    Serial.print(F("\n[INFO]: Wifi Credentials file saved!"));
    return true;
  }

  Serial.print(F("\n[ERROR]: Could not store Wifi Config File"));
  return false;
}

/*!
 *  @brief  Remove WifiConfiguration from LitteFS.
 *  @return Returns true if configuration removed successfully.
 */
bool removeWifiConfig()
{
  if(removeConfigData((char*) wifi_config_file))
    return true;

  return false;
}

///
///     DEVICE CONFIGURATION FILE
///

/*!
 *  @brief  Load Device Configuration saved in LitteFS.
 *  @return Returns true if configuration loaded successfully.
 */
bool loadDeviceConfig()
{
  if(loadConfigData(&Device_config, sizeof(Device_config), (char*) device_config_file))
  {
    if ( Device_config.checksum != calcChecksum( (uint8_t*) &Device_config, sizeof(Device_config) - sizeof(Device_config.checksum) ) )
    {
      Serial.print(F("\n[ERROR]: Device config checksum wrong!"));
      return false;
    }

    // Don't permit NULL SSID and password len < MIN_AP_PASSWORD_SIZE (8)
    if ( (String(Device_config.host_name) == "") )
    {
      Serial.print(F("\n[ERROR]: Hostname is empty, using default!"));
      storeString(String(DEFAULT_HOSTNAME), Wifi_config.WiFi_cred.wifi_ssid); 
    }
    Serial.print(F("\n[INFO]: Device Config File Read. Checksum ok."));
    return true; 
  }
  else
  {
    Serial.print(F("\n[ERROR]: Could not read device Config File."));
    return false;
  }
}

/*!
 *  @brief  Store Device Configuration into LitteFS.
 *  @return Returns true if configuration saved successfully.
 */
bool storeDeviceConfig(String host_name, String air_v, String wat_v, bool apmode)
{
  memset(&Device_config, 0, sizeof(Device_config));

  //SAVE HOSTNAME
  if (strlen(host_name.c_str()) < sizeof(Device_config.host_name) - 1)
    strcpy(Device_config.host_name, host_name.c_str());
  else
    strncpy(Device_config.host_name, host_name.c_str(), sizeof(Device_config.host_name) - 1);

  if ((String(Device_config.host_name) = ""))
    Serial.println(F("[WARNING]: Null hostname!"));

  //Calculate checksum and save credentials
  Device_config.checksum = calcChecksum((uint8_t*) &Device_config, sizeof(Device_config) - sizeof(Device_config.checksum));
  if (saveConfigData(&Device_config, sizeof(Device_config), (char*) device_config_file))
  {
    Serial.print(F("\n[INFO]: Device config file saved!"));
    return true;
  }

  Serial.print(F("\n[ERROR]: Could not store Devices Config File"));
  return false;
}

/*!
 *  @brief  Remove Device Configuration from LitteFS.
 *  @return Returns true if configuration removed successfully.
 */
bool removeDeviceConfig()
{
  if(removeConfigData((char*) device_config_file))
    return true;

  return false;
}

void defaultDeviceConfig()
{
  Serial.print(F("\n[WARNING]: USING DEFAULT VALUES!"));
  storeString(String(DEFAULT_HOSTNAME), Device_config.host_name);
  Device_config.air_value = DEF_CAL_AIR;
  Device_config.wat_value = DEF_CAL_WAT;
  Device_config.ap_mode = DEFAULT_TO_AP;  
}

/*!
 *  @brief  Initialize LittleFS and try to read Device Configuration.
 *  @return Returns true if configuration configuration successfully loaded.
 */
bool initDeviceConfiguration()
{
  if (!initFS())
  {
    defaultDeviceConfig();
    return false;
  }
  
  if (!loadDeviceConfig())
  {
    defaultDeviceConfig();
    return false;
  }

  return true;
}