#include <webServer.h>

AsyncWebServer server(HTTP_PORT);

// Request Handlers
void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}


String index_processor(const String& var){
  if(var == "HOST_NAME"){
    return host_name;
  }
  else if(var == "DEVICE_IP"){
    return WiFi.localIP().toString();
  }
  else if(var == "DHT_T"){
    return dht_temp;
  }
  else if(var == "DHT_H"){
    return dht_hum;
  }
  else if(var == "SOIL_H"){
    return soil_hum;
  }
  return String();
}


String config_processor(const String& var){
  if(var == "MODE"){
    switch (WiFi.getMode())
    {
    case WIFI_STA:
      return "Conectado : Cliente";
      break;
    case WIFI_AP:
      return "Ponto de Acesso";
      break;       
    default:
      return "";
      break;
    }
  }
  else if(var == "SSID"){
    return WiFi.SSID();
  }
    else if(var == "HOSTNAME"){
    return host_name;
  }
  else if(var == "DEVICE_IP"){
    return WiFi.localIP().toString();
  }
  else if (var == "MAC")
  {
    return WiFi.macAddress();
  }
  else if (var == "AIR_VAL")
  {
    return air_val;
  }
  else if (var == "WAT_VAL")
  {
    return wat_val;
  }  
  return String();
}


void setupRoutes()
{
    server.serveStatic("/", LittleFS, "/");

    //  *******    INDEX PAGE HANDLERS

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(LittleFS, "/index.html", "text/html", false);
    });

    //  *******    CONFIG PAGE HANDLERS

    server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(LittleFS, "/config.html", "text/html", false, config_processor);
    });

    server.on("/delete-config", HTTP_GET, [](AsyncWebServerRequest *request){
      if(removeDeviceConfig())
        request->send_P(200, "text/plain", "success");
      request->send_P(400, "text/plain", "error");
    });

    server.on("/delete-wifi", HTTP_GET, [](AsyncWebServerRequest *request){
      if(removeWifiConfig())
        request->send_P(200, "text/plain", "success");
      request->send_P(400, "text/plain", "error");
    });

    server.on("/update-config", HTTP_GET, [] (AsyncWebServerRequest *request) {
      bool ap_mode = false;

      if (request->hasParam("hostname") && request->hasParam("air_v") && request->hasParam("wat_v")) 
      {
        if (request->hasParam("ap_mode"))
        {
          ap_mode = true;
        }

        if (storeDeviceConfig(request->getParam("hostname")->value(), request->getParam("air_v")->value(), \
                              request->getParam("wat_v")->value(), ap_mode))
        {
          request->send_P(200, "text/plain", "success");
        }
        request->send_P(400, "text/plain", "error");
      }
      request->send_P(400, "text/plain", "error");
    });

    //  *******    WIFI PAGE HANDLERS

    server.on("/connect-wifi", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(LittleFS, "/wifi.html", "text/html", false);
    }).setFilter(ON_AP_FILTER);

    server.on("/scan-wifi", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "application/json", scanNetworks());
    });

    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "application/json", "{\"status\": \"" + String(WiFi.status()) + "\", \"network\": \""+ WiFi.SSID() + "\"}");
    });

    server.on("/connect", HTTP_GET, [] (AsyncWebServerRequest *request) {
      bool result = false;

      if (request->hasParam("auto-ip") && request->hasParam("ssid") && request->hasParam("password")) {
        result = connectToWifi(request->getParam("ssid")->value(), request->getParam("password")->value());
      } 
      else if (request->hasParam("ip") && request->hasParam("gateway") && request->hasParam("mask") && request->hasParam("ssid") && request->hasParam("password"))
      {
        setStaticIp(request->getParam("ip")->value(), request->getParam("gateway")->value(), request->getParam("mask")->value());
        result = connectToWifi(request->getParam("ssid")->value(), request->getParam("password")->value());
      }

      if (result)
        request->send(200, "application/json", "{\"status\": \"connected\" }");

      request->send(200, "application/json", "{\"status\": \"error\" }");
    });
}