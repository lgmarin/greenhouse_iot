#include <wifiConfig.h>

DNSServer dnsServer;

long               lastScanMillis;
long               currentMillis;
bool               dns_started = false;

bool configuremDNS()
{
    if (!MDNS.begin(String(Device_config.host_name))) {
        Serial.print(F("\n[ERROR]: mDNS service error."));
        return false;
    }
    // Add Web Server service to mDNS
    MDNS.addService("http", "tcp", 80);
    Serial.print(F("\n[INFO]: mDNS service started. Host: ")); Serial.print(String(Device_config.host_name));
    return true;
}

void updatemDNS()
{
    MDNS.update();
}

/**
 * @brief Start DNS server (Captive Portal Mode)
 * 
 * @param soft_ip Soft Access Point IP Address
 * @return true If DNS service started successfully
 * @return false If DNS service failed to start
 */
bool startDNSServer(IPAddress soft_ip)
{
    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);

    if (!dnsServer.start(53, "*", soft_ip))
    {
        Serial.print(F("\n[ERROR]: Failed to start DNS service."));
        dns_started = false;
        return false;
    }
    dns_started = true;
    return true;
}

bool openCaptivePortal()
{
    WiFi.persistent(false);
    WiFi.disconnect();
    WiFi.hostname(String(Device_config.host_name));
    WiFi.mode(WIFI_AP);
    WiFi.persistent(true);
    delay(500);

    Serial.print(F("\n[INFO]: Starting Captive Portal..."));
    bool result = WiFi.softAP(String(Device_config.host_name));

    if(result)
    {
        Serial.print(F("\n[SUCCESS]: Captive Portal Started at IP: ")); Serial.print(WiFi.softAPIP());
        startDNSServer(WiFi.softAPIP());
        configuremDNS();
        return true;
    }
    return false;
}

bool connectToAp()
{
    Serial.print(F("\n[INFO]: Starting soft-AP..."));

    if(WiFi.softAP(String(Device_config.host_name)))
    {
        Serial.print(F("\n[SUCCESS]: AP started at IP: ")); Serial.print(WiFi.softAPIP());
        configuremDNS();
        return true;
    }
    
    return false;    
}

/**
 * @brief Set the Static IP in WiFi Config
 * 
 * @param ip_addr Static IP Address
 * @param gw_addr Static Gateway IP Address
 * @param mask Subnet Mask
 * @return true If configured correctly
 * @return false If not configured correctly
 */
bool setStaticIp(String ip_addr, String gw_addr, String mask)
{
    IPAddress          st_ip, st_gw, st_mask;

    if (st_ip.fromString(ip_addr) && st_gw.fromString(gw_addr) && st_mask.fromString(mask))
    {
        if(!WiFi.config(st_ip, st_gw, st_mask)){
            Serial.print(F("\n[ERROR]: Unable to configure wifi. Using Dynamic IP."));
            return false;
        }
        Serial.print(F("\n[INFO]: Using static IP..."));
        return true;      
    }
    Serial.print(F("\n[ERROR]: Invalid IP address. Using Dynamic IP."));
    return false;         
}

/**
 * @brief Set the Static in WiFi Config from internal configuration
 * 
 * @return true If configured correctly
 * @return false If not configured correctly
 */
bool setStaticIp()
{
    if(!WiFi.config(Wifi_config.IP_config.ip_addr, Wifi_config.IP_config.gw_addr, Wifi_config.IP_config.mask)){
        Serial.print(F("\n[ERROR]: Unable to configure wifi. Using Dynamic IP."));
        return false;
    }
    Serial.print(F("\n[INFO]: Using static IP..."));
    return true;
}

/**
 * @brief Save Wifi Credentials to local storage
 * 
 * @param ssid Station SSID
 * @param pwd Station password
 * @param dyn_ip Use dynamic IP
 * @param ip_addr Static IP Address
 * @param gw_addr Static Gateway IP Address
 * @param mask Subnet Mask
 * @return true If correctly saved in flash
 * @return false If failed to saved in flash
 */
bool saveWifiCredentials(String ssid, String pwd, bool dyn_ip, String ip_addr, String gw_addr, String mask)
{
    IPAddress          st_ip, st_gw, st_mask;

    if (ssid != "" && pwd != "")
    {
        if (st_ip.fromString(ip_addr) && st_gw.fromString(gw_addr) && st_mask.fromString(mask) && !dyn_ip)
        {
            if(storeWifiConfig(ssid, pwd, dyn_ip, st_ip.fromString(ip_addr), st_gw.fromString(gw_addr), st_mask.fromString(mask)))
            {
                return true;
            }

            return false;
        } 
        else
        {
            IPAddress st_ip(0,0,0,0);
            IPAddress st_gw(0,0,0,0);
            IPAddress st_mask(0,0,0,0);

            if(storeWifiConfig(ssid, pwd, dyn_ip, st_ip, st_gw, st_mask))
            {
                return true;
            }
            return false;
        }
    }

    Serial.print(F("\n[ERROR]: Empty SSID or Password not valid!"));
    return false;
}

/**
 * @brief Connect to a Wifi Station
 * 
 * @param ssid Station SSID
 * @param pwd Station Password
 * @return true If WiFi returns a successful connection
 * @return false If WiFi fails to connect
 */
bool connectToWifi(String ssid, String pwd) 
{
    if (ssid == "" || pwd == "")
    {
        Serial.print(F("\n[ERROR]: Empty SSID or Password."));
        return false;
    }

    Serial.print(F("\n[INFO]: Connecting to station: ")); Serial.print(ssid);

    WiFi.disconnect();
    delay(500);

    //WiFi.mode(WIFI_STA);

    Serial.print(F("\n[INFO]: Connecting to WiFi..."));
    WiFi.begin(ssid, pwd);

    if(WiFi.waitForConnectResult(WIFI_RETRY_TIMEOUT) != WL_CONNECTED) {
        Serial.print(F("\n[ERROR]: Failed to connect."));
        return false;
    }

    Serial.print(F("\n[SUCCESS]: CONNECTED: Mode: STA, SSID: ")); Serial.print(WiFi.SSID());
    Serial.print(F(" IP: ")); Serial.print(WiFi.localIP());
    configuremDNS();
    return true;
}

/**
 * @brief Execute the next request for DNS loop
 * 
 */
void dnsProcessNext()
{
    if (dns_started)
    {
        dnsServer.processNextRequest();
    }    
}

/**
 * @brief Scan for networks nearby and return a JSON list
 * 
 * @return String JSON list of networks
 */
String scanNetworks()
{
    String json;
    bool canScan;
    int n = WiFi.scanComplete();

    currentMillis = millis();

    if (currentMillis - lastScanMillis > SCAN_PERIOD)
    {
        canScan = true;
        Serial.print(F("\n[INFO]: Scanning networks... "));
        lastScanMillis = currentMillis;
    }
        //json +=  "[";
        json += "{\"networks\": [";
        if(n == -2 && canScan){
        // Scan not triggered, and not in the waiting timer
        WiFi.scanNetworks(true);
    }
    else if(n)
    {
        for (int i = 0; i < n; ++i)
        {
            if(i) json += ",";
            json += "{";
            json += "\"SSID\":\""+WiFi.SSID(i)+"\"";
            json += ",\"Quality\":\""+String(WiFi.RSSI(i))+"\"";     
            json += "}";
        }

        WiFi.scanDelete();

        if(WiFi.scanComplete() == -2)
        {
            WiFi.scanNetworks(true);
        }
    }
    //json += "]";
    json += "] }";

    return json;
}

/**
 * @brief Initialize the WiFi manager
 * 
 * @param ap_mode bool Set the AP mode ON or OFF
 */
void initWifi(bool ap_mode)
{
    if (loadWifiConfig() && ap_mode == false)
    {           
        if (Wifi_config.dyn_ip)
        {
            setStaticIp();   
            if(!connectToWifi(String(Wifi_config.WiFi_cred.wifi_ssid), String(Wifi_config.WiFi_cred.wifi_pw)))
                ap_mode = true;
        } else {
            if(!connectToWifi(String(Wifi_config.WiFi_cred.wifi_ssid), String(Wifi_config.WiFi_cred.wifi_pw)))
                ap_mode = true;
        }
    }
    
    if (ap_mode) //IF Everything fails or preferred mode is AP
    {
        connectToAp();
    }
}

String getIpAddress()
{
    if (WiFi.getMode() == 2 || WiFi.getMode() == 3)
    {
        return String(WiFi.softAPIP().toString());
    } 
    return String(WiFi.localIP().toString());   
}

String getMacAddress()
{
    if (WiFi.getMode() == 2)
    {
        return WiFi.softAPmacAddress();
    }
    return WiFi.macAddress();
}

String getSSID()
{
    if (WiFi.getMode() == 2)
    {
        return String(Device_config.host_name);
    } 
    return WiFi.SSID(); 
}

String getHostName()
{
    return String(Device_config.host_name);
}

String getMode()
{
    switch (WiFi.getMode())
    {
    case WIFI_STA:
      return "Conectado";
      break;
    case WIFI_AP:
      return "Ponto de Acesso";
      break;
    case WIFI_OFF:
      return "Desligado";
      break;
    case WIFI_AP_STA:
      return "AP+STA";
      break;
    default:
      return "Desconhecido";
      break;
    }
}

String wifiInfo()
{
    String infoText;
    if (WiFi.getMode() == 2 || WiFi.getMode() == 3)
    {
        infoText = "AP: " + WiFi.softAPIP().toString();
        return infoText;
    }

    infoText = "STA: " + WiFi.localIP().toString();
    return infoText;
}