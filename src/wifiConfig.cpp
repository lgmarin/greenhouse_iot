#include <wifiConfig.h>

DNSServer dnsServer;

String             host_name;
long               lastScanMillis;
long               currentMillis;
bool               ap_mode = true;

IPAddress          static_ip;
IPAddress          static_gw;
IPAddress          static_mask;

bool configuremDNS()
{
    if (!MDNS.begin(host_name)) {
        Serial.print(F("\n[ERROR]: mDNS service error."));
        return false;
    }
    // Add Web Server service to mDNS
    MDNS.addService("http", "tcp", 80);
    Serial.print(F("\n[INFO]: mDNS service started. Host: ")); Serial.print(host_name);
    return true;
}


void dnsProcessNext()
{
    dnsServer.processNextRequest();
}


bool openCaptivePortal()
{
    Serial.print(F("\n[INFO]: Starting soft-AP..."));
    if(WiFi.softAP(host_name)){
        Serial.print(F("\n[SUCCESS]: Captive Portal Started at IP: ")); Serial.print(WiFi.softAPIP());
        if (!dnsServer.start(53, "*", WiFi.softAPIP()))
            Serial.print(F("\n[INFO]: Failed to start DNS service."));
        return true;
    }
    return false;
}


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
            json += "\"ssid\":"+WiFi.SSID(i);
            json += ",\"quality\":\""+String(WiFi.RSSI(i))+"\"";     
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

bool setStaticIp()
{
    if(!WiFi.config(Wifi_config.IP_config.ip_addr, Wifi_config.IP_config.gw_addr, Wifi_config.IP_config.mask)){
        Serial.print(F("\n[ERROR]: Unable to configure wifi. Using Dynamic IP."));
        return false;
    }
    Serial.print(F("\n[INFO]: Using static IP..."));
    return true;
}

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

    WiFi.mode(WIFI_STA);

    Serial.print(F("\n[INFO]: Connecting to WiFi..."));
    WiFi.begin(ssid, pwd);

    if(WiFi.waitForConnectResult(WIFI_RETRY_TIMEOUT) != WL_CONNECTED) {
        Serial.print(F("\n[ERROR]: Failed to connect."));
        return false;
    }
    Serial.print(F("\n[SUCCESS]: CONNECTED: Mode: STA, SSID: ")); Serial.print(WiFi.SSID());
    Serial.print(F(" IP: ")); Serial.print(WiFi.localIP());
    return true;
}

void initWifi()
{
    if (initFS())
    {
        if (loadDeviceConfig())
        {
            ap_mode = Device_config.ap_mode;

            if (String(Device_config.host_name) == "")
            {
                host_name = String(DEFAULT_HOSTNAME);
            } 
            else 
            {
                host_name = String(Device_config.host_name);
            }
        }
        else
        {
            ap_mode = true;
        }
        
        
        if (loadWifiConfig())
        {           
            if (Wifi_config.dyn_ip)
            {
                if(!connectToWifi(Wifi_config.WiFi_cred.wifi_ssid, Wifi_config.WiFi_cred.wifi_pw))
                    ap_mode = false;
            } else {
                if(!connectToWifi(Wifi_config.WiFi_cred.wifi_ssid, Wifi_config.WiFi_cred.wifi_pw))
                    ap_mode = false;
            }
        }
    }
    else
    {
        host_name = String(DEFAULT_HOSTNAME);
        ap_mode = true;
    }
    

    if (ap_mode)
    {
        openCaptivePortal();
    }
}