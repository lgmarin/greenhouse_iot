#include <wifiConfig.h>

DNSServer dnsServer;

const char*        host_name;
const char*        ssid;
const char*        password;

long               lastScanMillis;
long               currentMillis;

Wifi_Config        Wifi_config;

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


bool connectToWifi(String ssid, String pwd, String ip, String gw, String mask) 
{
    if (ssid == "" || pwd == "")
    {
        Serial.print(F("\n[ERROR]: Empty SSID or Password."));
        return false;
    }

    Serial.print(F("\n[INFO]: Connecting to station: ")); Serial.print(ssid);

    if (ip != "" && gw != "" && mask != "")
    {
        IPAddress wifi_ip;
        IPAddress wifi_gw;
        IPAddress wifi_mask;
        if (wifi_ip.fromString(ip) && wifi_gw.fromString(gw) && wifi_mask.fromString(mask))
        {
            if(!WiFi.config(wifi_ip, wifi_gw, wifi_mask)){
            Serial.print(F("\n[ERROR]: Unable to configure wifi. Using Dynamic IP."));
            }
            Serial.print(F("\n[INFO]: Using static IP..."));
        }
    }

    WiFi.disconnect();
    delay(500);

    WiFi.mode(WIFI_STA);

    Serial.print(F("\n[INFO]: Connecting to WiFi..."));
    WiFi.begin(ssid, password);

    if(WiFi.waitForConnectResult(WIFI_RETRY_TIMEOUT) != WL_CONNECTED) {
        Serial.print(F("\n[ERROR]: Failed to connect."));
        return false;
    }
    Serial.print(F("\n[SUCCESS]: CONNECTED: Mode: STA, SSID: ")); Serial.print(WiFi.SSID());
    Serial.print(F(" IP: ")); Serial.print(WiFi.localIP());
    return true;
}


void dnsProcessNext()
{
    dnsServer.processNextRequest();
}