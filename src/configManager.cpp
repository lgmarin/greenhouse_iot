#include <configManager.h>
#include <Arduino.h>

Wifi_Config        Wifi_config;
Device_Config      Device_config;

// Initialize LittleFS
void initFS() {
  if (!LittleFS.begin()) {
    Serial.print(F("\n[ERROR]: An error has occurred while mounting LittleFS"));
  }
  else{
    Serial.print(F("\n[INFO]: LittleFS mounted successfully"));
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
 *  @brief  Load WifiConfiguration saved in LitteFS.
 *  @return Returns true if configuration loaded successfully.
 */
bool loadWifiConfig()
{
  if(loadConfigData(&Wifi_config, sizeof(Wifi_config), (char*) WIFI_CFG_FILE))
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
  if (saveConfigData(&Wifi_config, sizeof(Wifi_config), (char*) WIFI_CFG_FILE))
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
  if(removeConfigData((char*) WIFI_CFG_FILE))
    return true;

  return false;
}